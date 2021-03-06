#include "smtp.h"

smtp::smtp(QString smtphost,QString smtpusername,QString smtppass)
{
    this->smtphost = smtphost;
    this->smtpusername = smtpusername;
    this->smtppass = smtppass;
}



bool smtp::Send(  const QString &to, const QString &subject, const QString &body )
{
    qDebug()<<"####"<<Q_FUNC_INFO;
    bool res = false;
    int waittime = 5 * 1000;
    this->from = smtpusername;
    rcpt = to;
    ErrorMSG.clear();
    Timeout = waittime;
    linesend = 0;
    isconnect = false;


    QString tmp = "=?utf-8?B?"+ QByteArray().append(subject).toBase64()+"?=";
    message.append("from:<" + smtpusername + ">\n");
    message.append("to:<" + to + ">\n");
    message.append("Subject:" + tmp + "\n");
    message.append("Content-Type: text/html; charset=UTF8;\n");   /* or txt */
    message.append("Content-transfer-encoding: 7BIT\n\n\n\n");
    message.append(body);
    message.replace( tr( "\n" ), tr( "\r\n" ) );
    message.replace( tr( "\r\n.\r\n" ),tr( "\r\n..\r\n" ) );

    smtpsocket = new QTcpSocket(this);
    connect( this, SIGNAL(SendLine()), this ,SLOT(PutSendLine()));
    if (smtphost.size() > 0)
    {
        smtpsocket->connectToHost(smtphost,25);
    } else {
        smtpsocket->connectToHost("localhost",25);
    }
    if (smtpsocket->waitForConnected(Timeout))
    {
        if (smtpsocket->waitForReadyRead(Timeout))
        {
            isconnect = true;
            qDebug()<<"start ReadLiner";
           return  ReadLiner();
        }
    }
    else
    {
         ErrorCloseAll();
    }
   return res;
}

bool  smtp::ReadLiner()
{
    bool res = false;
    if (isconnect)
    {
        QTextCodec *codecx;
        codecx = QTextCodec::codecForMib(106);
        t = new QTextStream( smtpsocket );
        t->setCodec(codecx);

        int loops = 0;
        while (!t->atEnd())
        {
            loops++;
            response = t->readLine();
        }
        if (response.size() > 0)
        {
            RemoteServerName = response;
            mailstatus = response.left(3);
             qDebug()<<"mailstatus: "+mailstatus;
            if (mailstatus == "220")
            {
                response="";
                linesend = 1;
                res = true;
            }
        }
        else
        {
            ErrorCloseAll();
        }
    }
    return res;
}

smtp::~smtp()

{
    delete smtpsocket;
    delete t;
}

/* LINE SENDER  */
bool smtp::PutSendLine()
{
    static bool res = true;
    int current = linesend;
    switch(current)
    {
        case 1:
            response = SendLineAndGrab("HELO hiboy");
            if (response.size() > 0)
            {
                ErrorMSG.append(response);
                linesend = 2;
                emit SendLine();
            }
            else
            {
                res=false;
            }
            response ="";

            break;
        case 2:
            response = SendLineAndGrab("auth login");
            if (response.size() > 0)
            {
                ErrorMSG.append(response);
                linesend = 3;
                emit SendLine();
            }
            else
            {
                res= false;
            }
            response ="";

            break;
        case 3:
            response = SendLineAndGrab(encodeBase64(smtpusername));   /* username send */
            if (response.size() > 0)
            {
                ErrorMSG.append(response);

                if (response.contains("334", Qt::CaseInsensitive))
                {
                linesend = 4;
                emit SendLine();
                }
            }
            else
            {
                res=false;
            }
            response ="";

            break;
        case 4:
            response = SendLineAndGrab(encodeBase64(smtppass));     /* pass send */
            if (response.size() > 0)
            {
                ErrorMSG.append(response);
                for(int i=0; i<ErrorMSG.size();i++){
                    qDebug()<<"4 :"<<ErrorMSG.at(i);
                }
                if (response.contains("235", Qt::CaseInsensitive))
                {
                    linesend = 5;
                    emit SendLine();
                }
                else
                {
                    res= false;
                }
            }
            else
            {
                res= false;
            }
            response ="";

            break;
        case 5:
        response = SendLineAndGrab(tr("mail from:<%1> ").arg(smtpusername));

            if (response.size() > 0)
            {
               ErrorMSG.append(response);
                linesend = 6;
                emit SendLine();
            }
            for(int i=0; i<ErrorMSG.size();i++){
                qDebug()<<"5 :"<<ErrorMSG.at(i);
            }

            break;
        case 6:
//            response = SendLineAndGrab("RCPT TO:<%1> "+rcpt);
                response = SendLineAndGrab(tr("rcpt to:<%1> ").arg(rcpt));
            if (response.size() > 0)
            {
                ErrorMSG.append(response);
                response ="";
                response = SendLineAndGrab("DATA");
                if (!response.contains("not", Qt::CaseInsensitive))
                {
                    ErrorMSG.append(response);
                    response ="";
                    linesend = 7;
                    emit SendLine();
                }
            }
            response ="";

            break;
        case 7:
            response = SendLineAndGrab(message+"\r\n.");
            if (response.size() && response.contains("ok", Qt::CaseInsensitive) )
            {
                ErrorMSG.append(response);
                linesend = 8;
                emit SendLine();
            }
            response ="";

            break;
        case 8:
            SendLineAndGrab("QUIT");

            break;
        default:

            break;
    }
    return res;
}

/* SENDER AND RECIVER  */
QString smtp::SendLineAndGrab(QString senddata)
{
    QString incommingData = "";
    qDebug()<<"senddata ="+senddata;
    if (isconnect)
    {
        int current = linesend;
        int loops = 0;

        *t << senddata << "\r\n";
        t->flush();
        if (senddata != "QUIT") {
            if (smtpsocket->waitForReadyRead(Timeout))
            {
                while (!t->atEnd())
                {
                    loops++;
                    QString opera = t->readLine()+"\n";
                    incommingData = opera + incommingData;
                }
            }
        } else
        {
            delete smtpsocket;
            delete t;
            isconnect = false;
            return incommingData;
        }
    }
    else
    {
         ErrorCloseAll();
    }
    return incommingData;
}


QString smtp::encodeBase64( QString xml )
{
    QByteArray text;
    text.append(xml);
    return text.toBase64();
}

QString smtp::decodeBase64( QString xml )
{
    QByteArray xcode("");;
    xcode.append(xml);
    QByteArray precode(QByteArray::fromBase64(xcode));
    QString notetxt = precode.data();
    return notetxt;
}



int smtp::dateswap(QString form, uint unixtime )
{
    QDateTime fromunix;
    fromunix.setTime_t(unixtime);
    QString numeric = fromunix.toString((const QString)form);
    bool ok;
    return (int)numeric.toFloat(&ok);
}


QString smtp::TimeStampMail()
{
    /* mail rtf Date format! http://www.faqs.org/rfcs/rfc788.html */
    QDateTime timer1( QDateTime::currentDateTime() );

    uint unixtime = timer1.toTime_t();
    QDateTime fromunix;
    fromunix.setTime_t(unixtime);


    QStringList RTFdays = QStringList() << "giorno_NULL" << "Mon" << "Tue" << "Wed" << "Thu" << "Fri" << "Sat" << "Sun";
    QStringList RTFmonth = QStringList() << "mese_NULL" << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun" << "Jul" << "Aug" << "Sep" << "Oct" << "Nov" << "Dec";
    QDate timeroad(dateswap("yyyy",unixtime),dateswap("M",unixtime),dateswap("d",unixtime));

    QStringList rtfd_line;
    rtfd_line.clear();
    rtfd_line.append("Date: ");
    rtfd_line.append(RTFdays.at(timeroad.dayOfWeek()));
    rtfd_line.append(", ");
    rtfd_line.append(QString::number(dateswap("d",unixtime)));
    rtfd_line.append(" ");
    rtfd_line.append(RTFmonth.at(dateswap("M",unixtime)));
    rtfd_line.append(" ");
    rtfd_line.append(QString::number(dateswap("yyyy",unixtime)));
    rtfd_line.append(" ");
    rtfd_line.append(fromunix.toString("hh:mm:ss"));
    rtfd_line.append(" +0100");
    return QString(rtfd_line.join(""));
}

void smtp::ErrorCloseAll()
{
    delete t;
    smtpsocket->close();
}
