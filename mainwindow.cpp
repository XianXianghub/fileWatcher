#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QFileDialog>


#include "smtp.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(400,240); //设置窗体固定大小
    WindowRect = this->geometry();
    BrowerRect=ui->brower_status->geometry();
    ui->brower_status->setStyleSheet("font-size : 16px");
    m_pSystemWatcher = new QFileSystemWatcher();
    timer = new QTimer( this );

    thread2 = new QThread();

    mSendTread = new sendthread();       //发送命令线程，用于延时较大的命令
    mSendTread->moveToThread(thread2);
    thread2->start();
    timer->setSingleShot(true);
    this->setWindowTitle("fileWatcher");
    DirDefault();
    getCmdlist();
    connect(timer, SIGNAL(timeout()), this, SLOT(Timeout()) );
    connect(m_pSystemWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(directoryUpdated(QString)));
    connect(m_pSystemWatcher, SIGNAL(fileChanged(QString)), this, SLOT(fileUpdated(QString)));
    connect(ui->brower_status, SIGNAL(textChanged()), this, SLOT(autoScroll()));
    connect(this,SIGNAL(StartSendCMD(QStringList , int)),mSendTread,SLOT(SendCmd(QStringList,int)));  //发送耗时命令到线程执行

    QxtGlobalShortcut *shortcut = new QxtGlobalShortcut(this);
    QxtGlobalShortcut *shortcutClose = new QxtGlobalShortcut(this);
    if(shortcut->setShortcut(QKeySequence("Ctrl+d")))
    {  
        connect(shortcut, SIGNAL(activated()), this, SLOT(on_pushButton_clicked()));
    }
    else
    {
        qDebug()<<"快捷键已占用";
        QMessageBox::information(NULL, "Title", "快捷键已占用", QMessageBox::Yes, QMessageBox::Yes);

    }

    if(shortcutClose->setShortcut(QKeySequence("Ctrl+e")))
    {
        connect(shortcutClose, SIGNAL(activated()), this, SLOT(on_close_slot()));
    }
    else
    {
        qDebug()<<"快捷键已占用";
        QMessageBox::information(NULL, "Title", "快捷键已占用", QMessageBox::Yes, QMessageBox::Yes);

    }



}

void MainWindow::resizeEvent(QResizeEvent*event)
{
    QMainWindow::resizeEvent(event);
    curGemRect = geometry();
}

void MainWindow::moveEvent(QMoveEvent *event)
{
    QMainWindow::moveEvent(event);
    curGemRect = geometry();
}

void MainWindow::on_pushButton_clicked()
{
  qDebug()<<"yyy";
  on_startmm_clicked();
}

void MainWindow::on_close_slot()
{
    setWindowState(Qt::WindowActive);
    setGeometry(curGemRect);
    activateWindow();
    show();
}

void MainWindow::autoScroll()
{
    QTextCursor cursor =  ui->brower_status->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->brower_status->setTextCursor(cursor);
}

void MainWindow::Timeout()
{
      qDebug()<<"timeout once";
      setWindowState(Qt::WindowNoState);
      hide();

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::addWatchPath(QString path)
{
    qDebug() << QString("Add to watch: %1").arg(path);
    m_pSystemWatcher->addPath(path);
    // 如果添加路径是一个目录，保存当前内容列表
    QFileInfo file(path);
    if (file.isDir())
    {
        const QDir dirw(path);
        m_currentContentsMap[path] = dirw.entryList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files, QDir::DirsFirst);
    }
    QTextStream txtOutput(defautarg);

    defautarg->close();
    defautarg->open(QIODevice::Truncate);
    defautarg->close();

    defautarg->open(QIODevice ::ReadWrite);

    txtOutput<<path<<endl;
}

void MainWindow::directoryUpdated(const QString &path)
{
    qDebug() << QString("Directory updated: %1").arg(path);

    // 比较最新的内容和保存的内容找出区别(变化)
    QStringList currEntryList = m_currentContentsMap[path];
    const QDir dir(path);

    QStringList newEntryList = dir.entryList(QDir::NoDotAndDotDot  | QDir::AllDirs | QDir::Files, QDir::DirsFirst);

    QSet<QString> newDirSet = QSet<QString>::fromList(newEntryList);
    QSet<QString> currentDirSet = QSet<QString>::fromList(currEntryList);

    // 添加了文件
    QSet<QString> newFiles = newDirSet - currentDirSet;
    QStringList newFile = newFiles.toList();

    // 文件已被移除
    QSet<QString> deletedFiles = currentDirSet - newDirSet;
    QStringList deleteFile = deletedFiles.toList();

    // 更新当前设置
    m_currentContentsMap[path] = newEntryList;

    if (!newFile.isEmpty() && !deleteFile.isEmpty())
    {
        // 文件/目录重命名
        if ((newFile.count() == 1) && (deleteFile.count() == 1))
        {
            qDebug() << QString("File Renamed from %1 to %2").arg(deleteFile.first()).arg(newFile.first());
        }
    }
    else
    {
        // 添加新文件/目录至Dir
        if (!newFile.isEmpty())
        {
            qDebug() << "New Files/Dirs added: " << newFile;
            qDebug() << "mifile: " << mifile;

            for(int i=0; i<newFile.length();i++){
                if(newFile.at(i).compare("btmp")==0){
                    QFile file(listenPath+"/btmp");
                    file.open(QIODevice::ReadOnly | QIODevice::Text);
                    QString ret = QString(file.readAll()).trimmed();
                    file.close();
                    qDebug() << "mifile ret=" << ret<<"===";
                    if(ret.compare("1")==0){
                         qDebug() << "complie sucess "  ;
                         printLog("编译成功");
                         setWindowState(Qt::WindowActive);
                         setGeometry(curGemRect);
                         activateWindow();
                         show();

                         if(timer->isActive())
                             timer->stop();   //停止定时器
                         timer->start(SHOWTIME);
                         emit StartSendCMD(cmdlist , 0);
                    }else{
                         qDebug() << "complie failed "  ;
                         ui->brower_status->setFixedSize(600, 500);
                         this->setFixedSize(700,600);
                         ui->brower_status->clear();
                         printLog("编译失败");
                         setWindowState(Qt::WindowActive);
                         setGeometry(curGemRect);
                         activateWindow();
                         show();
                         ReadLogShow();
                    }
                }
            }
        }

        // 从Dir中删除文件/目录
        if (!deleteFile.isEmpty())
        {
            qDebug() << "Files/Dirs deleted: " << deleteFile;

            foreach(QString file, deleteFile)
            {
                // 处理操作每个被删除的文件....
            }
        }
    }
}
void MainWindow::DirDefault()
{

    QStringList temlist;
    QString dir =  QCoreApplication::applicationDirPath()+"/defaut.txt";
    defautarg = new QFile(dir);
    if (defautarg->open(QIODevice ::ReadWrite))
    {
        QByteArray line = defautarg->readLine();
        QString str(line);
        qDebug()<< str;
        temlist = str.split("&&");
    }

    if(temlist.length() >=1){

        QString fd = temlist.at(0).trimmed().simplified();
        int index = fd.lastIndexOf("/");
        qDebug()<<fd.mid(0,index);
        qDebug()<<fd.mid(index+1,fd.length());
        mifile = fd.mid(index+1,fd.length());
        ui->listen_path->setText(fd.mid(0,index));
    }
}

QStringList MainWindow::getCmdlist()
{
    QString path =  QCoreApplication::applicationDirPath()+"/cmdlist.txt";
    QFile file(path);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      while (!file.atEnd())
      {
          QByteArray line = file.readLine();
          QString str(line);
          qDebug() << str.trimmed();
          cmdlist << str.trimmed();
      }
      file.close();
    }
}

void MainWindow::fileUpdated(const QString &path)
{
    QFileInfo file(path);
    QString strPath = file.absolutePath();
    QString strName = file.fileName();

    qDebug() << QString("The file %1 at path %2 is updated").arg(strName).arg(strPath);
}

void MainWindow::on_chosedir_clicked()
{
     QString filename = QFileDialog::getOpenFileName(
                   this, "选择文件",
                   ui->listen_path->text(),
                   "所有文件 (*.*);; ");
       qDebug() << "path=" << filename;
       if (!filename.isEmpty())
       {
            int index = filename.lastIndexOf("/");
            qDebug()<<filename.mid(0,index);
            qDebug()<<filename.mid(index+1,filename.length());
            mifile = filename.mid(index+1,filename.length());
            ui->listen_path->setText(filename.mid(0,index));
       }


}

void MainWindow::on_listen_clicked()
{
    listenPath  =  ui->listen_path->text();
    qDebug()<<"listenPath"+listenPath;
    if(listenPath.isEmpty())   {
        QMessageBox::information(this,"warning", "请选择目录");
        return;
    }

    QString dir =  QCoreApplication::applicationDirPath()+"/defaut.txt";
    qDebug()<<dir;
    QFile *file = new QFile(dir);

    QTextStream out(file);

    file->close();
    file->open(QIODevice::Truncate);
    file->close();

    file->open(QIODevice ::ReadWrite|QFile::Truncate);
   // QStringList errorValue = ui->listen_path->text().trimmed().split(QRegExp("[\r\n]"),QString::SkipEmptyParts);
    QString writestring = ui->listen_path->text().trimmed()+"/"+mifile;
  //      QString writestring = errorValue[0].append("&&").append(ui->cmd_input->text().trimmed());
    qDebug()<<writestring;
     out<<writestring;
    addWatchPath(listenPath);
    printLog("监听中...");
    ui->listen->setEnabled(false);
    isListenning = true;
    setForceinit();
}

void MainWindow::on_startmm_clicked()
{
    if(!isListenning) {
        QMessageBox::about(NULL, "error", "plz open listen");
        return;
    }
    listenPath  =  ui->listen_path->text();
    QString delfile= listenPath.trimmed()+"/btmp";
    QFileInfo file(delfile);
    qDebug()<<"delfile"+delfile;
    qDebug()<<"window satte="+windowState();

    if(file.exists()==false){
          qDebug()<<"文件不存在";
    }else{
       QFile fileTemp(delfile);
       fileTemp.remove();
       fileTemp.close();

       setWindowState(Qt::WindowActive);
       setGeometry(curGemRect);
       activateWindow();
       show();
       printLog("编译中...");
       setWindowState(Qt::WindowNoState);
       hide();
    }
    ui->brower_status->setFixedSize(BrowerRect.width(),BrowerRect.height());
    this->setFixedSize(WindowRect.width(),WindowRect.height());

}
void MainWindow::setForceinit(){
    listenPath  =  ui->listen_path->text();
    QString forcefile= listenPath.trimmed()+"/bforce";
    QFileInfo finfo(forcefile);
    qDebug()<<"forcefile=="+forcefile;
    if(finfo.exists()==false){
          qDebug()<<"文件不存在";
    }else{
        if(ui->checkBox->isChecked())
        {
            qDebug("ok,unchecked!");
            QFile file(forcefile);
            file.open(QIODevice::WriteOnly | QIODevice::Text);
            file.write("1");
            file.close();
        }
        else
        {
            QFile file(forcefile);
            file.open(QIODevice::WriteOnly | QIODevice::Text);
            file.write("0");
            file.close();
            qDebug("no,unchecked!");
        }
    }

}

void MainWindow::printLog(QString str)
{
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("hh:mm:ss");
    ui->brower_status->append(current_date+": "+str);
}

void MainWindow::ReadLogShow()
{
    QString outpath = listenPath.trimmed()+"/out_last100.log";
    QFile file(outpath);
    QFileInfo finfo(outpath);
    qDebug()<<"forcefile=="+outpath;
    if(finfo.exists()==false){
          qDebug()<<"out.log 文件不存在";
    }else{
        if (!file.open(QIODevice::ReadOnly|QIODevice::Text))//打开指定文件
        {
            QMessageBox::about(NULL, "文件", "文件打开失败");
        }
        QTextStream txtInput(&file);
        QString lineStr;
        while (!txtInput.atEnd())
        {
            lineStr = txtInput.readLine();  //读取数据
            if(lineStr.contains("Error",Qt::CaseInsensitive) ||
                    lineStr.contains("failed",Qt::CaseInsensitive)){
                if(lineStr.contains("Error",Qt::CaseInsensitive)){
                    QRegExp valueRegExp(QString("(%1)").arg("Error"));
                    valueRegExp.setCaseSensitivity(Qt::CaseInsensitive);
                    lineStr = lineStr.replace(valueRegExp, "<font style='font-size:23px; background-color:white; color:red;'>\\1</font>");
                }else if(lineStr.contains("failed",Qt::CaseInsensitive)){
                    QRegExp valueRegExp(QString("(%1)").arg("failed"));
                    valueRegExp.setCaseSensitivity(Qt::CaseInsensitive);
                    lineStr = lineStr.replace(valueRegExp, "<font style='font-size:23px; background-color:white; color:red;'>\\1</font>");
                }
            }
                ui->brower_status->append(lineStr);
        }
        file.close();
    }
}

void MainWindow::on_checkBox_clicked()
{
    listenPath  =  ui->listen_path->text();
    QString forcefile= listenPath.trimmed()+"/bforce";
    QFileInfo finfo(forcefile);
    qDebug()<<"forcefile=="+forcefile;
    if(finfo.exists()==false){
          qDebug()<<"文件不存在";
    }else{
        if(ui->checkBox->isChecked())
        {
            qDebug("ok,unchecked!");
            QFile file(forcefile);
            file.open(QIODevice::WriteOnly | QIODevice::Text);
            file.write("1");
            file.close();
        }
        else
        {
            QFile file(forcefile);
            file.open(QIODevice::WriteOnly | QIODevice::Text);
            file.write("0");
            file.close();
            qDebug("no,unchecked!");
        }
    }

}
