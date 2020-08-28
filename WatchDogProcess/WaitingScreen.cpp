#include "WaitingScreen.h"
#include "ui_WaitingScreen.h"
#include <Windows.h>
#include <QDebug>
#include <QDesktopWidget>
WaitingScreen::WaitingScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WaitingScreen)
{
    ui->setupUi(this);


    SetForegroundWindow((HWND)winId());
    setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    //WaitingScreen::setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    this->setWindowState(Qt::WindowFullScreen);
    //this->show();
    // connect(ui->label,SIGNAL(linkActivate(QString)),this,SLOT(linkClicked(QString)));
    //connect(ui->label,SIGNAL(linkActivated(QString)),this,SLOT(linkClicked(QString)));
}

WaitingScreen::~WaitingScreen()
{
    delete ui;
}

void WaitingScreen::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void WaitingScreen::ShowPAge(int retry)
{
    //        setAttribute(Qt::WA_TranslucentBackground);
    //        setAttribute(Qt::WA_MacNoShadow);
    //        setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    //        setWindowFlags( Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint | Qt::WindowStaysOnTopHint);
    this->setWindowState(Qt::WindowFullScreen);
    ui->label ->setText(" Please wait while we start RemoteDesk for you");
    show();
}

void WaitingScreen::showErrorPage(QString message)
{
    QRect rec = QApplication::desktop()->screenGeometry();
    int height = rec.height() /2;
    int width = rec.width() / 2;

    this->setGeometry(width-450,height-250,900,500);
    QString supportLink = "<a href=\"https://www.remotedesk.com/support/\">";
    message.append(supportLink+" contact support.</a>");

    //    setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
    //this->setWindowState(Qt::WindowMinimized);
    ui->label->setFixedWidth(ui->label->width()+20);
    ui->label ->setText(message);
}



