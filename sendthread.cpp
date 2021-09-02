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
            ret += "ret";

   }
}
