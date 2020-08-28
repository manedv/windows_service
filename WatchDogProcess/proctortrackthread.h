#ifndef PROCTORTRACKTHREAD_H
#define PROCTORTRACKTHREAD_H

#include <QThread>
#include<QProcess>

class ProctortrackThread : public QThread
{
    Q_OBJECT

public:
    ProctortrackThread();
    void run();
    QString HistoryPath;
    QString appName;
    bool isFristTimeLaunch;
    static QStringList whiteListedTabList;
    bool isAnyStepWatchdog;
    bool IsClose_BL_App_using_wd;

public slots:
    void process(int dummyCall = false);
    void process2();
    void KillOtherApplications();
    void KillApplication(QString appname);

signals:
    void isApllicationRunning(bool val);
    void moveWindow();
};

#endif // PROCTORTRACKTHREAD_H

