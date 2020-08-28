#include "proctoringwatchdog.h"
#include <QApplication>
#include <QDebug>
#include <QtSingleApplication>
#include "qtservice.h"
#include <QLabel>
#include <QDesktopWidget>

/////////////////////////////////////////////////////////////////////////////////
class ServiceControl : public QtService<QApplication>
{
public:
    ServiceControl(int argc, char **argv);
    ~ServiceControl();
//    ProctoringWatchDog *watchProcess;

protected:

    void start();
    void stop();
    void pause();
    void resume();
    void processCommand(int code);

};

ServiceControl::ServiceControl(int argc, char **argv)
    : QtService<QApplication>(argc, argv, "Watchdog_Service")
{
     qDebug()<<"======== Constructor  ======= ";
    setServiceDescription("A Watchdog service with RemoteDesk.");
    setServiceFlags(QtServiceBase::CanBeSuspended);

//    watchProcess = new ProctoringWatchDog;
//    watchProcess->CheckRunningApplication();
//    watchProcess->hide();
}

ServiceControl::~ServiceControl()
{
    qDebug()<<"======== ~ ServiceControl ======= ";
}

void ServiceControl::start()
{
    qDebug()<<"======== start ======= ";
//    watchProcess->StartMonitoring();
//    qApp->setQuitOnLastWindowClosed(false);

}

void ServiceControl::stop()
{
     qDebug()<<"======== stop ======= ";
//     watchProcess->StopMonitoring();
}

void ServiceControl::pause()
{
qDebug()<<"======== pause ======= ";
}

void ServiceControl::resume()
{
qDebug()<<"======== resume ======= ";
}

void ServiceControl::processCommand(int code)
{
qDebug()<<"======== processCommand ======= "<<code;
}
//////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);
    QtSingleApplication s(argc,argv);

    qDebug() << "App path :" << qApp->applicationDirPath();
    if((!qApp->applicationDirPath().contains("Proctortrack")) && (!qApp->applicationDirPath().contains("RemoteDesk")) && (!qApp->applicationDirPath().contains("FreshHire")))
    {
//        return 0;
    }

    if(s.isRunning())
    {
        return 1;
    }

    ServiceControl service(argc, argv);
    return service.exec();

//    ProctoringWatchDog *watchProcess = new ProctoringWatchDog;
//    watchProcess->CheckRunningApplication();
//    watchProcess->hide();
//    watchProcess->StartMonitoring();
//    return s.exec();
}
