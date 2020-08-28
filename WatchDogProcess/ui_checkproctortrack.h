/********************************************************************************
** Form generated from reading UI file 'checkproctortrack.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHECKPROCTORTRACK_H
#define UI_CHECKPROCTORTRACK_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CheckProctortrack
{
public:
    QWidget *centralWidget;
    QFrame *verticalFrame;
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QPushButton *m_restart;
    QSpacerItem *horizontalSpacer;
    QPushButton *m_close;
    QLabel *label;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *CheckProctortrack)
    {
        if (CheckProctortrack->objectName().isEmpty())
            CheckProctortrack->setObjectName(QStringLiteral("CheckProctortrack"));
        CheckProctortrack->resize(731, 464);
        centralWidget = new QWidget(CheckProctortrack);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalFrame = new QFrame(centralWidget);
        verticalFrame->setObjectName(QStringLiteral("verticalFrame"));
        verticalFrame->setGeometry(QRect(80, 50, 561, 241));
        verticalLayout = new QVBoxLayout(verticalFrame);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        frame = new QFrame(verticalFrame);
        frame->setObjectName(QStringLiteral("frame"));
        horizontalLayout = new QHBoxLayout(frame);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        m_restart = new QPushButton(frame);
        m_restart->setObjectName(QStringLiteral("m_restart"));
        m_restart->setMinimumSize(QSize(40, 60));

        horizontalLayout->addWidget(m_restart);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        m_close = new QPushButton(frame);
        m_close->setObjectName(QStringLiteral("m_close"));
        m_close->setMinimumSize(QSize(40, 60));

        horizontalLayout->addWidget(m_close);


        verticalLayout->addWidget(frame);

        label = new QLabel(verticalFrame);
        label->setObjectName(QStringLiteral("label"));
        label->setMinimumSize(QSize(550, 0));

        verticalLayout->addWidget(label);

        CheckProctortrack->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(CheckProctortrack);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 731, 22));
        CheckProctortrack->setMenuBar(menuBar);
        mainToolBar = new QToolBar(CheckProctortrack);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        CheckProctortrack->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(CheckProctortrack);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        CheckProctortrack->setStatusBar(statusBar);

        retranslateUi(CheckProctortrack);

        QMetaObject::connectSlotsByName(CheckProctortrack);
    } // setupUi

    void retranslateUi(QMainWindow *CheckProctortrack)
    {
        CheckProctortrack->setWindowTitle(QApplication::translate("CheckProctortrack", "CheckProctortrack", 0));
        m_restart->setText(QApplication::translate("CheckProctortrack", "Restart", 0));
        m_close->setText(QApplication::translate("CheckProctortrack", "Close", 0));
        label->setText(QApplication::translate("CheckProctortrack", "TextLabel", 0));
    } // retranslateUi

};

namespace Ui {
    class CheckProctortrack: public Ui_CheckProctortrack {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHECKPROCTORTRACK_H
