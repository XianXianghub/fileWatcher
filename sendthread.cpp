#include "sendthread.h"

sendthread::sendthread(QObject *parent) : QObject(parent)
{
    mfind =  adb_find();
    AppPath = mfind.GetAPPPath();


   // qDebug()<<"sdda444444: "+AppPath;
}

sendthread::~sendthread()
{


}

bool sendthread::SendCmdSingle(QString cmd,int flag)
{
    qDebug()<<"SendCmdSingle=="+ cmd;
    QProcess adbProcess;
    QString strCmd =AppPath+cmd;
    // qDebug()<<"path cmd="<<strCmd;
    adbProcess.start( strCmd);

    if(!adbProcess.waitForFinished(-1))
    {
      ret.clear();
    }
    else
    {
        if(flag == 1)   //标准信息
        {
           ret =QString::fromLocal8Bit(adbProcess.readAllStandardOutput());
        }
        else    //错误信息
        {
            ret =QString::fromLocal8Bit(adbProcess.readAllStandardError());
           // qDebug()<<ret;

        }


    }
    qDebug()<<"SendCmdSingle ret="<<ret;
    if(strCmd.contains("adb push")){
          if(ret.contains("Read-only file")){
             return false;
          }
    }
    return  true;
}


void sendthread::SendCmd(QStringList cmd, int flag)
{

 //   qDebug()<<cmd+"  "+QString::number(flag,10);

     for(int i=0;i<cmd.size();i++){
            qDebug()<<"cmd=="+ cmd.at(i);
            QProcess adbProcess;
            QString strCmd =AppPath+cmd.at(i);
            // qDebug()<<"path cmd="<<strCmd;
            adbProcess.start( strCmd);

            if(!adbProcess.waitForFinished(-1))
            {
              ret.clear();
            }
            else
            {
                if(flag == 1)   //标准信息
                {
                   ret =QString::fromLocal8Bit(adbProcess.readAllStandardOutput());
                }
                else    //错误信息
                {
                    ret =QString::fromLocal8Bit(adbProcess.readAllStandardError());
                   // qDebug()<<ret;

                }


            }
            if(strCmd.contains("adb push")){
                  if(ret.contains("Read-only file")){
                      SendCmdSingle("adb root",1);
                      SendCmdSingle("adb remount",1);
                      if(!SendCmdSingle(cmd.at(i),1)){
                           emit sigSendStatus("failed");
                          return;
                      }
                  }
            }
            qDebug()<<"SendCmd ret="<<ret;


   }
      emit sigSendStatus("success");
}
