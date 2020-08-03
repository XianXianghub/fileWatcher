#ifndef SENDTHREAD_H
#define SENDTHREAD_H


#include "adb_find.h"

#include <QObject>
#include <QDebug>
#include <QString>
#include <QThread>
#include <QProcess>



#pragma comment(lib, "ws2_32.lib")
#include  <winsock2.h>

class sendthread : public QObject
{
    Q_OBJECT
public:
    explicit sendthread(QObject *parent = 0);
    ~sendthread();
    QString ret;
    SendApp(QString ip, QString info);
    adb_find mfind;

    QString AppPath;
signals:

public slots:
    void SendCmd(QString,int);


};



#endif // SENDTHREAD_H
