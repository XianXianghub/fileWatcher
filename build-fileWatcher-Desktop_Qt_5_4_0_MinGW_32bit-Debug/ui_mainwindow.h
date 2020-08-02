/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLineEdit *listen_path;
    QLabel *label;
    QPushButton *chosedir;
    QPushButton *listen;
    QLabel *label_2;
    QLineEdit *cmd_input;
    QCheckBox *checkBox;
    QPushButton *startmm;
    QTextBrowser *brower_status;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->setMinimumSize(QSize(402, 242));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        listen_path = new QLineEdit(centralWidget);
        listen_path->setObjectName(QStringLiteral("listen_path"));
        listen_path->setGeometry(QRect(100, 110, 171, 21));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(40, 110, 31, 21));
        chosedir = new QPushButton(centralWidget);
        chosedir->setObjectName(QStringLiteral("chosedir"));
        chosedir->setGeometry(QRect(290, 110, 75, 23));
        listen = new QPushButton(centralWidget);
        listen->setObjectName(QStringLiteral("listen"));
        listen->setGeometry(QRect(30, 20, 61, 31));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(40, 150, 31, 21));
        cmd_input = new QLineEdit(centralWidget);
        cmd_input->setObjectName(QStringLiteral("cmd_input"));
        cmd_input->setGeometry(QRect(100, 150, 171, 21));
        checkBox = new QCheckBox(centralWidget);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setGeometry(QRect(40, 70, 41, 16));
        startmm = new QPushButton(centralWidget);
        startmm->setObjectName(QStringLiteral("startmm"));
        startmm->setGeometry(QRect(290, 150, 75, 23));
        brower_status = new QTextBrowser(centralWidget);
        brower_status->setObjectName(QStringLiteral("brower_status"));
        brower_status->setGeometry(QRect(100, 10, 271, 81));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 402, 23));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        label->setText(QApplication::translate("MainWindow", "\350\267\257\345\276\204:", 0));
        chosedir->setText(QApplication::translate("MainWindow", "\351\200\211\346\213\251\346\226\207\344\273\266", 0));
        listen->setText(QApplication::translate("MainWindow", "\345\274\200\345\247\213\347\233\221\345\220\254", 0));
        label_2->setText(QApplication::translate("MainWindow", "CMD:", 0));
        checkBox->setText(QApplication::translate("MainWindow", "-B", 0));
        startmm->setText(QApplication::translate("MainWindow", "\345\274\200\345\247\213\347\274\226\350\257\221", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
