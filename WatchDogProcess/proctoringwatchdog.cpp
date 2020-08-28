#include "proctoringwatchdog.h"
#include "ui_proctoringwatchdog.h"
#include <QDir>
#include <QDebug>
#include <QProcess>
#include <QTimer>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QDesktopWidget>

///basic watchdog document
/** step 0 = specialy for MAC Catalina on system check
   * step 1 = before begin proctoring
   * step 2 = after begin proctorig
   * step 3 = on data upload
   * step 4 = not used
   * isAnyStepWatchdog = For RTRD for every step / at system startup
   * isSceenCoverReq = for waiting/ restarting screen
   * whiteListedTabs = used in isAnyStepWatchdog for closing other than this tabs when app is not running
   * is_close_bl_apps_using_wd = for closig non whitelited app/tabs at frist time of wd launch
   * step  =*/

ProctoringWatchDog::ProctoringWatchDog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProctoringWatchDog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_MacNoShadow);
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    setWindowFlags( Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint | Qt::WindowStaysOnTopHint);

    counter = 0;
    isFirst = true;
    appLaunchRetryCountre = 0;
    WaitForAppToLaunch.start();
    HistoryPath = QDir::fromNativeSeparators(QStandardPaths::writableLocation(QStandardPaths::TempLocation));
    HistoryPath      += "/TempHistory.json";
    whiteListedTabs <<"WatchDogProcess"<<"RemoteDesk"<<"Proctortrack"<<"Submit a ticket"<<"Untitled - Google Chrome"<<"freshdesk"<<": Support"<<"Verificient"<<"Program Manager";
    QString configList = GetWhiteListedTabForWD();
    whiteListedTabs.append( configList.split(","));
    whiteListedTabs.removeDuplicates();
    whiteListedTabs.removeAll("");
    ProctortrackThread::whiteListedTabList = whiteListedTabs;
    isAnyStepWatchdog = GetisAnyStepWatchdog();
    isSceenCoverReq = GetisSceenCoverReq();
    //qDebug()<<"======== GetWhiteListedTabForWD : "<<GetWhiteListedTabForWD();

    ui->m_close->hide();
    i = 2;
    ui->m_restart->setText("Proctortrack");
    //connect(ui->m_restart,SIGNAL(clicked(bool)),this,SLOT(RestartProctortrack()));
    connect(ui->m_close,SIGNAL(clicked(bool)),this,SLOT(QuitWatchDog()));
    MoveWindow();

    QTimer interval;
    connect(&interval,SIGNAL(timeout()),this,SLOT(MoveWindow()));
    interval.start(1000);
    //this->hide();
    //show();
}

void ProctoringWatchDog::MoveWindow()
{
    //widget.move(QApplication::desktop()->screen()->rect().center() - widget.rect().center());
    //qDebug() <<"111111111111 : " << i;
    QDesktopWidget * desktop = QApplication::desktop();
    QWidget * s = desktop->screen(0);

    //QList<QScreen*> screens = QApplication::screens();
    QRect r = s->geometry();

    if(i%2 == 0)
        this->move(0,100);
    else if(i%3 ==0)
        this->move(r.width()- 300,100);
    else if(i%5 ==0)
        this->move(r.width()- 300,r.height()-300);
    else if(i%7 ==0)
        this->move(0,r.height()-300);
    if(i==7)
        i=2;
    i++;

}

QString ProctoringWatchDog::GetWhiteListedTabForWD()
{
    QString list="";
    QFile fp(HistoryPath);
    if(fp.exists())
    {
        fp.open(QIODevice::ReadWrite);
        list = QJsonDocument::fromJson(fp.readAll()).object()["white_listed_tabs_for_wd"].toString();
        fp.close();
    }
    return list;
}

bool ProctoringWatchDog::GetisAnyStepWatchdog()
{
    bool value = false;
    QFile fp(HistoryPath);
    if(fp.exists())
    {
        fp.open(QIODevice::ReadWrite);
        value = QJsonDocument::fromJson(fp.readAll()).object()["launch_app_in_all_conditions"].toBool();
        fp.close();
    }
    return value;
}

bool ProctoringWatchDog::GetisSceenCoverReq()
{
    bool value = false;
    QFile fp(HistoryPath);
    if(fp.exists())
    {
        fp.open(QIODevice::ReadWrite);
        value = QJsonDocument::fromJson(fp.readAll()).object()["is_sceen_cover_required"].toBool();
        fp.close();
    }
    return value;
}

bool ProctoringWatchDog::GetIsClose_BL_App_using_wd()
{
    bool value = false;
    QFile fp(HistoryPath);
    if(fp.exists())
    {
        fp.open(QIODevice::ReadWrite);
        value = QJsonDocument::fromJson(fp.readAll()).object()["is_close_bl_apps_using_wd"].toBool();
        fp.close();
    }
    return value;
}

void ProctoringWatchDog::StartMonitoring()
{
    watchThreaD->start();
}

void ProctoringWatchDog::StopMonitoring()
{
    watchThreaD->quit();;
}

void ProctoringWatchDog::CheckRunningApplication()
{
    watchThreaD = new ProctortrackThread();
    watchThreaD->appName = GetApplicationName();
    watchThreaD->isAnyStepWatchdog = GetisAnyStepWatchdog();
    watchThreaD->IsClose_BL_App_using_wd = GetIsClose_BL_App_using_wd();

    watchThreaD->start();
    connect(watchThreaD,SIGNAL(moveWindow()),this,SLOT(MoveWindow()));
    connect(watchThreaD,SIGNAL(isApllicationRunning(bool)),this,SLOT(OnAppCrash(bool)));

    waiting_sceen = new WaitingScreen(NULL);
    waiting_sceen->isSceenCoverReq = GetisSceenCoverReq();
}

void ProctoringWatchDog::OnAppCrash(bool isRunning)
{
    counter++;
    //isAnyStepWatchdog = false;
    AppDataPath = GetAppDataPath();
    AppRunningPath = GetAppRunningLocation();
    Step = GetAppCurrentStep();
    //KillProctortrackIfNotResponding();
    if(counter>=7)
    {
        counter = 0;
        ProcessRecordingThread();
    }

    if(!isRunning)
    {

        //if(GetApplicationName()=="RemoteDesk")
        {
            isAnyStepWatchdog = GetisAnyStepWatchdog();
        }

        this->hide();
        if(Step.compare("STEP2")==0 || Step.compare("STEP3")==0 || isAnyStepWatchdog)
        {
            isSceenCoverReq = GetisSceenCoverReq();
            if(isSceenCoverReq)
            {
                appLaunchRetryCountre++;
                if(appLaunchRetryCountre > 5)
                {
                    waiting_sceen->showErrorPage("Unable to start RemoteDesk");
                    return ;
                }
                else
                {
                    waiting_sceen->ShowPAge(appLaunchRetryCountre);
                }
            }

            RestartProctortrack();
        }
        else
        {
            QuitWatchDog();
        }
    }
    else
    {
        appLaunchRetryCountre = 0;
        waiting_sceen->hide();
    }

}

void ProctoringWatchDog::QuitWatchDog()
{
    qDebug() << "Quiting watchdog...";
    qApp->closeAllWindows();
    QApplication::quit();
}

void ProctoringWatchDog::RestartProctortrack()
{
    if(isFirst)
        timer = 10000;
    else
        timer = 180000;
    isFirst = false;
#ifdef Q_OS_WIN
    if(WaitForAppToLaunch.elapsed() >= 10000)
    {
        WaitForAppToLaunch.restart();
        QTimer::singleShot(100,this,SLOT(KillProctortrackIfNotResponding()));
        QTimer::singleShot(1000,this,SLOT(LaunchApp()));
    }
#else
    if(WaitForAppToLaunch.elapsed() >= timer)
    {
        WaitForAppToLaunch.restart();
        QTimer::singleShot(100,this,SLOT(KillProctortrackIfNotResponding()));
        QTimer::singleShot(1000,this,SLOT(LaunchApp()));
    }
#endif
}

void ProctoringWatchDog::KillProctortrackIfNotResponding()
{
    QString appName=GetApplicationName();
    qDebug() << "Force kill app since it is freezed/not responding... ";
#ifdef Q_OS_WIN
    QProcess::execute("taskkill /im WerFault.exe /f");

    if(appName=="Proctortrack")
    {
        QProcess::execute("taskkill /im Proctortrack.exe /f");
        return;
    }
    else if(appName=="RemoteDesk")
    {
        QProcess::execute("taskkill /im RemoteDesk.exe /f");
        return;
    }
    else if(appName=="FreshHire")
    {
        QProcess::execute("taskkill /im FreshHire.exe /f");
        return;
    }
    else
    {
        QProcess::execute("taskkill /im Proctortrack.exe /f");
        QProcess::execute("taskkill /im RemoteDesk.exe /f");
        QProcess::execute("taskkill /im FreshHire.exe /f");
    }
#else
    if(appName=="Proctortrack")
    {
        QProcess::execute("killall Proctortrack");
        return;
    }
    else if(appName=="RemoteDesk")
    {
        QProcess::execute("killall RemoteDesk");
        return;
    }
    else if(appName=="FreshHire")
    {
        QProcess::execute("killall FreshHire");
        return;
    }
    else
    {
        QProcess::execute("killall Proctortrack");
        QProcess::execute("killall RemoteDesk");
        QProcess::execute("killall FreshHire");
    }
#endif
}

void ProctoringWatchDog::LaunchApp()
{
#ifdef Q_OS_MAC
    QString S = GetAppRunningLocation() ;
    QString  p = "\"";
    p +=S;
    p +="\"";
    p +=" argument -macUpdate";
    if(QProcess::startDetached(p))
    {
    }
    else
    {
        //ui->label->setText("We are not able to run the Proctortrack.\n Please check if Proctortrack availbale");
    }

#else
    QString S = GetAppRunningLocation();
    QString path = S;
    path = QDir::fromNativeSeparators("\"" + path + "\"");

    path += " -macUpdate";
    //path=path+ "\"";
    bool returnApp = QProcess::startDetached(path);
    //    if(!returnApp)
    //    {
    //        ui->label->setText("We are not able to run the Proctortrack.\n Please check if Proctortrack.exe availbale");
    //    }
#endif
}


QString ProctoringWatchDog::GetAppCurrentStep()
{
    QString time;
    QFile fp(HistoryPath);
    if(fp.exists())
    {
        fp.open(QIODevice::ReadWrite);
        time = QJsonDocument::fromJson(fp.readAll()).object()["app_current_step"].toString();
        fp.close();
    }
    else
    {
        qDebug() << "Does not exist";
    }
    return time;
}

QString ProctoringWatchDog::GetThreadHistoryPath()
{
    QString time;
    QFile fp(HistoryPath);
    if(fp.exists())
    {
        fp.open(QIODevice::ReadWrite);
        time = QJsonDocument::fromJson(fp.readAll()).object()["thread_history_path"].toString();
        fp.close();
    }
    else
    {
        qDebug() << "Does not exist";
    }
    return time;
}

QString ProctoringWatchDog::GetAppDataPath()
{
    QString time;
    QFile fp(HistoryPath);
    if(fp.exists())
    {
        fp.open(QIODevice::ReadWrite);
        time = QJsonDocument::fromJson(fp.readAll()).object()["app_data_path"].toString();
        fp.close();
    }
    return time;
}

QString ProctoringWatchDog::GetAppRunningLocation()
{
    QString time;
    QFile fp(HistoryPath);
    if(fp.exists())
    {
        fp.open(QIODevice::ReadWrite);
        time = QJsonDocument::fromJson(fp.readAll()).object()["app_location"].toString();
        fp.close();
    }
    return time;
}

QString ProctoringWatchDog::GetApplicationName()
{
    QString time;
    QFile fp(HistoryPath);
    if(fp.exists())
    {
        fp.open(QIODevice::ReadWrite);
        time = QJsonDocument::fromJson(fp.readAll()).object()["app_name"].toString();
        fp.close();
    }
    return time;
}

void ProctoringWatchDog::ProcessRecordingThread()
{
    ThreadStatusPath = GetThreadHistoryPath();

    QFile fp(ThreadStatusPath);
    bool val = false;
    if(fp.exists())
    {
        fp.open(QIODevice::ReadWrite);
        val = QJsonDocument::fromJson(fp.readAll()).object()["recording_process_state"].toBool();

        fp.close();
    }
    if(val)
    {
        //qDebug() << " True status";
        val = false;
        QJsonObject object;
        if(fp.exists())
        {
            fp.open(QIODevice::ReadWrite);
            object = QJsonDocument::fromJson(fp.readAll()).object();
            fp.close();
        }
        object["recording_process_state"] = val;
        //object["chunk_process_state"] = ChunkProcess;
        //object["activity_process_state"] = ActivityProcess;
        //object["ds_process_state"] = DSProcess;
        //object["monitoring_process_state"] = MonitoringProcess;
        //object["camera_process_state"] = CameraProcess;

        fp.open(QIODevice::WriteOnly | QIODevice::Truncate);
        fp.write(QJsonDocument(object).toJson());
        fp.close();
    }
    else
    {
        qDebug() << "False state";
        // Thread is broken. Proctortrack need to restart again.
        Step = GetAppCurrentStep();
        if(Step.compare("STEP2")==0 )
        {
            //ui->label->setText("Proctortrack app stooped responding. To continue with your exam, please force quit the app once. Proctortrack will relaunched automatically in few second , if not , Please launch the app manually.");
            //Restart the app
            RestartProctortrack();
            //this->show();
        }
    }
}

void ProctoringWatchDog::CameraGrabberThread()
{
    ThreadStatusPath = GetThreadHistoryPath();

    QFile fp(ThreadStatusPath);
    bool val;
    if(fp.exists())
    {
        fp.open(QIODevice::ReadWrite);
        val = QJsonDocument::fromJson(fp.readAll()).object()["camera_process_state"].toBool();

        fp.close();
    }
    if(val)
    {
        //qDebug() << " True status";
        val = false;
        QJsonObject object;
        if(fp.exists())
        {
            fp.open(QIODevice::ReadWrite);
            object = QJsonDocument::fromJson(fp.readAll()).object();
            fp.close();
        }
        object["recording_process_state"] = val;
        //object["chunk_process_state"] = ChunkProcess;
        //object["activity_process_state"] = ActivityProcess;
        //object["ds_process_state"] = DSProcess;
        //object["monitoring_process_state"] = MonitoringProcess;
        //object["camera_process_state"] = CameraProcess;

        fp.open(QIODevice::WriteOnly | QIODevice::Truncate);
        fp.write(QJsonDocument(object).toJson());
        fp.close();
    }
    else
    {
        //qDebug() << "False state";
        // Restart proctortrack
        //RestartProctortrack();
        // Thread is broken. Proctortrack need to restart again.
        //this->show();
    }
}

void ProctoringWatchDog::ProcessChunkCreationThread()
{

}

ProctoringWatchDog::~ProctoringWatchDog()
{
    delete ui;
}
