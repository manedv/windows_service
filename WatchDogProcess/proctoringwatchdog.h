#ifndef PROCTORINGWATCHDOG_H
#define PROCTORINGWATCHDOG_H

#include <QWidget>
#include <QMainWindow>
#include <proctortrackthread.h>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTime>
#include "WaitingScreen.h"

namespace Ui {
class ProctoringWatchDog;
}

class ProctoringWatchDog : public QWidget
{
    Q_OBJECT

public:
    explicit ProctoringWatchDog(QWidget *parent = 0);
    ~ProctoringWatchDog();

    ProctortrackThread *watchThreaD;
    QString HistoryPath;
    QString Step;
    QString AppRunningPath;
    QString AppDataPath;
    QString ThreadStatusPath;
    long counter;
    QTime WaitForAppToLaunch;
    bool isFirst;
    int timer;
    int i;
    bool isAnyStepWatchdog;
    QStringList whiteListedTabs;
    WaitingScreen *waiting_sceen;
    int appLaunchRetryCountre;
    bool isSceenCoverReq;

public slots:
    void CheckRunningApplication();
    void OnAppCrash(bool isRunning);
    void RestartProctortrack();
    void LaunchApp();
    void KillProctortrackIfNotResponding();
    void QuitWatchDog();
    void ProcessRecordingThread();
    void ProcessChunkCreationThread();
    void CameraGrabberThread();

    QString GetAppCurrentStep();
    QString GetAppDataPath();
    QString GetAppRunningLocation();
    QString GetThreadHistoryPath();
    QString GetApplicationName();
    void MoveWindow();
    QString GetWhiteListedTabForWD();
    bool GetisAnyStepWatchdog();
    bool GetisSceenCoverReq();
    bool GetIsClose_BL_App_using_wd();

    void StartMonitoring();
    void StopMonitoring();

private:
    Ui::ProctoringWatchDog *ui;
};

#endif // PROCTORINGWATCHDOG_H
