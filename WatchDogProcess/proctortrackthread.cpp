#include "proctortrackthread.h"
#include <QTimer>
#include <QThread>
#include <QDateTime>
#include <QStandardPaths>
#include <QDebug>
#include <QFile>
#include <QDir>
#ifdef Q_OS_MAC
#include <Carbon/Carbon.h>
#include <sys/sysctl.h>
#include <pwd.h>
#elif defined(Q_OS_WIN)
#include <Windows.h>
#include <Psapi.h>
#include <windows.h>
#include <tlhelp32.h>
#endif

using namespace std;

#ifdef Q_OS_WIN
#pragma comment (lib, "User32.lib")
#pragma comment (lib, "kernel32.lib")
#pragma comment(lib, "pdh.lib")
bool processRunning = false;

QStringList ProctortrackThread::whiteListedTabList = QStringList();

bool isProctortrack(const PROCESSENTRY32W &entry)
{
    return std::wstring(entry.szExeFile) == L"Proctortrack.exe";
}
bool isRemoteDesk(const PROCESSENTRY32W &entry)
{
    return std::wstring(entry.szExeFile) == L"RemoteDesk.exe";
}
bool isFreshHire(const PROCESSENTRY32W &entry)
{
    return std::wstring(entry.szExeFile) == L"FreshHire.exe";
}
BOOL CALLBACK enumWindowsProc(HWND hwnd, LPARAM lParam)
{
    const auto &pids = *reinterpret_cast<std::vector<DWORD>*>(lParam);
    DWORD winId;
    GetWindowThreadProcessId(hwnd, &winId);
    if(pids.size()<=0)
    {
        return TRUE;
    }
    else
    {
        if(pids[0]==NULL)
            return TRUE;
        DWORD pid = pids[0];
        if (winId == pid)
        {
            std::wstring title(GetWindowTextLength(hwnd) + 1, L'\0');
            GetWindowTextW(hwnd, &title[0], title.size()); //note: C++11 only
            QString s = QString::fromStdWString(title);
            // if(s.contains("ActiveMovie Window") || s.contains("Google Hangouts - Google Chrome")|| s.contains("hangouts is sharing your screen",Qt::CaseInsensitive))
            {
                processRunning =1;
                return TRUE;
            }
        }
    }
    return TRUE;
}

BOOL CALLBACK EnumWindowProc2(HWND hwnd2, LPARAM lParam)
{
    DWORD pid = 0;
    Q_UNUSED(hwnd2); // hwnd2 can be used for closing all focus/non focus windows
    HWND hwnd=GetForegroundWindow(); //for only focused wind

    if (IsWindowVisible(hwnd))
    {

        std::wstring title(GetWindowTextLength(hwnd) + 1, L'\0');
        GetWindowTextW(hwnd, &title[0], title.size()); //note: C++11 only
        QString str = QString::fromWCharArray( title.c_str() );


        if(str.isEmpty())
        {
            qDebug()<<" ====== IFFFFF  "<<str;
            return  false;
        }

        if(hwnd != NULL) GetWindowThreadProcessId(hwnd, &pid);


        bool isWhitelisted = true;
        QStringList TabList= ProctortrackThread::whiteListedTabList;

        foreach (QString tab,TabList)
        {
            //qDebug()<<"TTTTTTTTTTTTTTTTTTTTTTTttt  tab : "<<tab;
            if(str.contains(tab,Qt::CaseInsensitive))
            {
                isWhitelisted = true ;
                qDebug()<<"found whitelisted Tab : "<<tab;
                break;
            }
            else
            {
                isWhitelisted = false;
            }
        }


        if(!isWhitelisted)
        {
            //qDebug()<<"========= if ===== title "<<str;
            //if(str.contains("Task Manager",Qt::CaseInsensitive))
            //QProcess::execute("taskkill /im Taskmgr.exe /f");
            //else
            //PostMessageW(hwnd, WM_CLOSE, FALSE, 0);


            GetWindowThreadProcessId(hwnd,&pid);
            HANDLE win_handle = OpenProcess(PROCESS_TERMINATE, FALSE, (DWORD) pid);

            //QProcess::execute("taskkill /im Proctortrack.exe /f");

            if (win_handle == 0 || win_handle == INVALID_HANDLE_VALUE || !TerminateProcess(win_handle, 0))
            {
                qDebug()<<"Close win_handle FALSE , STR "<<str;
            }
            else
            {
                qDebug()<<"Close win_handle TRue, STR "<<str;
            }
        }
        //else
        //qDebug()<<"=========  else ========= title "<<str;

    }
    //    Sleep(1000);
    return false;
}

#endif

ProctortrackThread::ProctortrackThread()
{
     IsClose_BL_App_using_wd = false;
      isFristTimeLaunch = true;
}

void ProctortrackThread::run()
{
   // isFristTimeLaunch = true;
    process(true);  //dummy call for checking is watchdog frist launch without app for
    QTimer interval;
    connect(&interval,SIGNAL(timeout()),this,SLOT(process()));
    interval.start(10000);

    QTimer interval2;
    connect(&interval2,SIGNAL(timeout()),this,SLOT(process2()));
    interval2.start(1000);


    QTimer killAppsTimer;
    connect(&killAppsTimer,SIGNAL(timeout()),this,SLOT(KillOtherApplications()));
    //if(isFristTimeLaunch)
    killAppsTimer.start(1000);
    exec();
}

void ProctortrackThread::process2()
{
    emit moveWindow();
}

void ProctortrackThread::KillOtherApplications()
{
#ifdef Q_OS_WIN
    if(isFristTimeLaunch && IsClose_BL_App_using_wd)
    {
        EnumWindows(EnumWindowProc2, NULL);

    }
#elif Q_OS_MAC
    QString aScript = " ";
    aScript += "tell application \"System Events\" to set quitapps to name of every application process whose visible is true and name is not \"Finder\" and name is not \"Proctortrack\" and name is not \"Proctorcall\" and name is not \"RemoteDesk\" and name is not \"FreshHire\" and name is not \"Google Chrome\" and name is not \"Firefox\" and name is not \"Safari\" and name is not \"TeamViewer\" and name is not \"ManyCam\" name is not \"Verificient Technologies\"\n"


               "repeat with closeall in quitapps \n"

               "try \n"

               "with timeout of 1 second \n"
               "quit application closeall \n"
               "end timeout \n"

               "on error errstr \n"
               "end try \n"

               "end repeat\n";

    QString osascript = "/usr/bin/osascript";
    QStringList processArguments;
    processArguments << "-l" << "AppleScript";

    QProcess p;

    //p.start(osascript, processArguments);// neet to test

    p.write(aScript.toUtf8());
    p.closeWriteChannel();

    p.waitForFinished(5000);
#endif
}

void ProctortrackThread::KillApplication(QString appname)
{
    QProcess::execute(QString("taskkill /im %1 /f").arg(appname));
}

void ProctortrackThread::process(int dummyCall )
{
#ifdef Q_OS_MAC
    struct kinfo_proc *result, *ptr;
    int name[] = { CTL_KERN, KERN_PROC, KERN_PROC_ALL, 0 };
    size_t length, i;

    result = NULL;

    length = 0;
    int flag =0;
    if (sysctl((int *) name,(sizeof(name) / sizeof(*name)) - 1,NULL,&length,NULL,0) == -1)
    { return; }

    if ((result = (kinfo_proc *) malloc(length)) == NULL)
    {  return; }

    if (sysctl((int *) name,(sizeof(name) / sizeof(*name)) - 1,result,&length,NULL,0) != -1)
    {
        int i = 0;
        int count = (length / sizeof(kinfo_proc));
        for (i = 0, ptr = result; i < count; ++i, ++ptr)
        {
            struct passwd *us = getpwuid(ptr->kp_eproc.e_ucred.cr_uid);
            QString user = us ? us->pw_name : "root";

            if(appName.compare("Proctortrack",Qt::CaseInsensitive)==0)
            {
                if(std::strcmp(ptr->kp_proc.p_comm,"Proctortrack")==0)
                {
                    flag = 1;
                }
            }
            else if(appName.compare("RemoteDesk",Qt::CaseInsensitive)==0)
            {
                if(std::strcmp(ptr->kp_proc.p_comm,"RemoteDesk")==0)
                {
                    flag = 1;
                }
            }
            else if(appName.compare("FreshHire",Qt::CaseInsensitive)==0)
            {
                if(std::strcmp(ptr->kp_proc.p_comm,"FreshHire")==0)
                {
                    flag = 1;
                }
            }
        }
    }
    if(flag==1)
    {
        emit isApllicationRunning(true);
    }
    else
    {
        emit isApllicationRunning(false);
    }

    free(result);
#else
    std::vector<DWORD> pids;
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32W entry;
    entry.dwSize = sizeof entry;
    if (!Process32FirstW(snap, &entry)) {
        return;
    }
    do
    {
        if(appName.compare("Proctortrack",Qt::CaseInsensitive)==0)
        {
            if (isProctortrack(entry))
            {
                pids.emplace_back(entry.th32ProcessID);
            }
        }
        else if(appName.compare("RemoteDesk",Qt::CaseInsensitive)==0)
        {
            if (isRemoteDesk(entry))
            {
                pids.emplace_back(entry.th32ProcessID);
            }
        }
        else if(appName.compare("FreshHire",Qt::CaseInsensitive)==0)
        {
            if (isFreshHire(entry))
            {
                pids.emplace_back(entry.th32ProcessID);
            }
        }

    } while (Process32NextW(snap, &entry));

    // while(1 && pids.size()>0)
    if(pids.size()<=0)
    {
        processRunning = false;
        //return;
    }
    else
    {
        processRunning = true;
        isFristTimeLaunch = false;
        //EnumWindows(enumWindowsProc, reinterpret_cast<LPARAM>(&pids));
    }
    //qDebug() << "Proctortrack Running :" << processRunning;

    if(!dummyCall)
        emit isApllicationRunning(processRunning);

#endif
}

