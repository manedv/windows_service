#ifndef WAITINGSCREEN_H
#define WAITINGSCREEN_H

#include <QWidget>

namespace Ui {
class WaitingScreen;
}

class WaitingScreen : public QWidget
{
    Q_OBJECT

public:
    explicit WaitingScreen(QWidget *parent = 0);
    ~WaitingScreen();
    bool isSceenCoverReq;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::WaitingScreen *ui;
public slots:
    void ShowPAge(int retry =0);
    void showErrorPage(QString message/*=""*/);
    // void linkClicked(QString link);

    //private slots:
    //    void on_label_linkActivated(const QString &link);
    //    void on_label_linkHovered(const QString &link);
};

#endif // WAITINGSCREEN_H
