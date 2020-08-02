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
    m_pSystemWatcher = new QFileSystemWatcher();
    this->setWindowTitle("fileWatcher");
    DirDefault();
    connect(m_pSystemWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(directoryUpdated(QString)));
    connect(m_pSystemWatcher, SIGNAL(fileChanged(QString)), this, SLOT(fileUpdated(QString)));

    QxtGlobalShortcut *shortcut = new QxtGlobalShortcut(this);

    if(shortcut->setShortcut(QKeySequence("Ctrl+d")))
    {
        connect(shortcut, SIGNAL(activated()), this, SLOT(on_pushButton_clicked()));
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
                         QString cmd = "adb push " +ui->listen_path->text()+"\\"+mifile+ " system/app";
                         mfind.SendCmd(cmd,1);
                         //mfind.SendCmd("adb shell monkey -p com.unionman.settings 1",1);

                         QString startapp = ui->cmd_input->text().trimmed();
                         mfind.SendCmd(startapp,1);
                    }else{
                         printLog("编译失败");
                         ui->brower_status->setFixedSize(600, 500);
                         this->setFixedSize(700,600);
                         qDebug() << "complie failed "  ;
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
     if(temlist.length() >=2){
         qDebug()<<"0=="+temlist[0];
         qDebug()<<"1=="+temlist[1];
         ui->cmd_input->setText(temlist.at(1).trimmed().simplified());
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
    QString writestring = ui->listen_path->text().trimmed()+"/"+mifile+"&&"+ui->cmd_input->text().trimmed();
  //      QString writestring = errorValue[0].append("&&").append(ui->cmd_input->text().trimmed());
    qDebug()<<writestring;
     out<<writestring;
//    txtOutput<<ui->listen_path->text().trimmed()+"&&"<<ui->cmd_input->text().trimmed()<<endl;
//    qDebug()<<ui->listen_path->text().trimmed()+"&&"<<ui->cmd_input->text().trimmed()<<endl;

    addWatchPath(listenPath);
    printLog("监听中...");
    ui->listen->setEnabled(false);

    setForceinit();
}

void MainWindow::on_startmm_clicked()
{
    listenPath  =  ui->listen_path->text();
    QString delfile= listenPath.trimmed()+"/btmp";
    QFileInfo file(delfile);
    qDebug()<<"delfile"+delfile;
    qDebug()<<"window satte="+windowState();

    if(file.exists()==false){
          qDebug()<<"文件不存在";
          if(isActiveWindow()){
              setWindowState(Qt::WindowNoState);
              hide();
          }else{
              setWindowState(Qt::WindowActive);
              setGeometry(curGemRect);
              activateWindow();
              show();
          }

    }else{
        QFile fileTemp(delfile);
        fileTemp.remove();
        fileTemp.close();

       setWindowState(Qt::WindowActive);
       setGeometry(curGemRect);
       activateWindow();
       show();
       printLog("编译中...");
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