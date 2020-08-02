#ifndef ADB_FIND_H
#define ADB_FIND_H
#include <QDebug>
#include <QString>
#include <QProcess>
#include <QCoreApplication>


class adb_find
{
public:
    adb_find();
    void EditorIP(QString ip);

    bool Connect(QString ip);
    QString ScanIP();
    QString SendCmd(QString cmd,int flag);
    void OnlySendCMD(QString cmd);
    QString ADBSendCMD(QString cmd, int flag);
    QString GetAPPPath();
    ~adb_find();
private:
    QString ADBPath;
};






#endif // ADB_FIND_H
