#include "tipsdlg.h"
#include "ui_tipsdlg.h"

TipsDlg::TipsDlg(TipsType type, const QString &msg, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TipsDlg),
    m_type(type)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    initFrame(msg);

    m_pTimer = new QTimer(this);
    m_pTimer->setSingleShot(true);
    connect(m_pTimer, &QTimer::timeout, this, [=](){this->close();});
}

TipsDlg::~TipsDlg()
{
    if(m_pTimer != Q_NULLPTR)
        m_pTimer->deleteLater();

    delete ui;
}

void TipsDlg::startTimer()
{
    m_pTimer->start(3000);
}

void TipsDlg::initFrame(const QString &msg)
{
    if(TipsType_Warnning == m_type)//警告
    {
        ui->iconLabel->setStyleSheet("QLabel{background-image: url(:/qc/warnning);background-repeat:no-repeat;}");
    }
    else if(TipsType_Ok == m_type)//成功
    {
        ui->iconLabel->setStyleSheet("QLabel{background-image: url(:/qc/tips_ok);background-repeat:no-repeat;}");
    }

    ui->tipsLabel->setText(msg);

}
