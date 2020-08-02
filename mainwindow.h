#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QFileSystemWatcher>
#include"./include/curl/curl.h"
#include "adb_find.h"
#include <QFile>
#include "qxtglobalshortcut.h"
#include "tipsdlg.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    void addWatchPath(QString path);
    void DirDefault();
    explicit MainWindow(QWidget *parent = 0);
         ~MainWindow();
    void resizeEvent(QResizeEvent*event);
    void moveEvent(QMoveEvent *event);
    void setForceinit();
    void printLog(QString str);
    void ReadLogShow();
    QRect curGemRect;
public slots:
    void directoryUpdated(const QString &path); // 目录更新时调用，
    void fileUpdated(const QString &path); // 文件被修改时调用，
    void on_pushButton_clicked();
    void autoScroll();
    void Timeout();

private slots:
    void on_chosedir_clicked();

    void on_listen_clicked();

    void on_startmm_clicked();

    void on_checkBox_clicked();

private:

  QFile *defautarg;

private:
    Ui::MainWindow *ui;
    QMap<QString, QStringList> m_currentContentsMap;
    QFileSystemWatcher *m_pSystemWatcher;
    QString listenPath;
    QString  mifile;
     adb_find mfind;
     QRect WindowRect;
     QRect BrowerRect;
     QTimer *timer;
     int SHOWTIME = 500;
};






#endif // MAINWINDOW_H
