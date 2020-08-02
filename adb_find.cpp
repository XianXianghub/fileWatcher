#include "adb_find.h"

//构造函数
adb_find::adb_find()
{


     ADBPath = GetAPPPath();

    // EditorIP("192.168.1.1");
   // qDebug()<<"asdsa";
   // ScanIP();


}
//建立连接
bool adb_find::Connect(QString ip)
{
//    QString cmd = "adb ";
//    SendCmd();
}
//扫描
QString adb_find::ScanIP()
{
    int i,Ip_last = 1;

    QString cmd,ret;

  //  cmd = "192.168.1."+QString::number(Ip_last,10);
   // qDebug()<<cmd;
    for(Ip_last=0;Ip_last<256;Ip_last++)
    {
       // cmd.clear();
       cmd = "adb connect 192.168.1."+QString::number(Ip_last,10);

       qDebug()<<"cmd ip:"+cmd;
       ret = SendCmd(cmd,1);
       qDebug()<<"scan ret:"+ret;


       ret = ret.section(' ', 0, 0);
       qDebug()<<"tiqu:"+ret;



       if(ret.compare("connected")==0)
       {
            qDebug()<<"connected";
         //  return cmd;
       }
       if(ret.compare("unable")==0)
       {
            qDebug()<<"unbale";
       }
    }
    return NULL;

}

QString adb_find::GetAPPPath()
{
    QString dir =  QCoreApplication::applicationDirPath();

 //   QString strCmd ="C:\\Windows\\SysWOW64\\"+cmd;

  //  qDebug()<<"debug1:"+dir;


    dir =  dir.replace(QString("/"), QString("\\\\"))+"\\\\";


 //   qDebug()<<"debug2:"+dir;
    return dir;
}

void adb_find::EditorIP(QString ip )
{
    QString cmd = "ifconfig eth0 ";
    cmd += ip;
    SendCmd(cmd,1);
    qDebug()<<cmd;
}

//命令 adb  flag=1; 标准信息
//命令 adb  flag=0; 错误信息
QString adb_find::SendCmd(QString cmd,int flag )
{
    QString str;
    QProcess adbProcess;
    QString strCmd =ADBPath+cmd;


    adbProcess.start( strCmd);
   qDebug()<<"strCmd"+strCmd;

    if(!adbProcess.waitForFinished(-1))
    {
      return NULL;
    }
    else
    {
        if(flag == 1)   //标准信息
        {
           str =QString::fromLocal8Bit(adbProcess.readAllStandardOutput());
        }
        else  if(flag==0)  //错误信息
        {
            str =QString::fromLocal8Bit(adbProcess.readAllStandardError());

        }
        else if(flag==2)
        {

            qDebug()<<"dsfs";
            str =QString::fromLocal8Bit(adbProcess.readLine());
        }

        return str;
    }

    //adbProcess.waitForFinished();
    //adbProcess.finished();
}


void adb_find::OnlySendCMD(QString cmd)
{

    QProcess adbProcess;
    QString strCmd ="C:\\Windows\\SysWOW64\\"+cmd;
    adbProcess.start( strCmd);
    if(adbProcess.waitForFinished(-1))
    {
      qDebug()<<"sdfsfddf";
    }

}

QString adb_find::ADBSendCMD(QString cmd, int flag)
{
    QProcess adbProcess;
    QString str;
    QString CmdRoad ="C:/Windows/SysWOW64/adb.exe";

    QStringList CmdList = cmd.split(" ");


    adbProcess.start(CmdRoad,CmdList);

        if(flag == 1)   //标准信息
        {
            qDebug()<<"aaaa";
           str =QString::fromLocal8Bit(adbProcess.readAllStandardOutput());
        }
        else  if(flag==0)  //错误信息
        {
            str =QString::fromLocal8Bit(adbProcess.readAllStandardError());

        }
        else if(flag==2)
        {


            str =QString::fromLocal8Bit(adbProcess.readLine());
        }

        return str;

}


adb_find::~adb_find()
{

}

