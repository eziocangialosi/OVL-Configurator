#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include <QMessageBox>
#include <QRegularExpression>
#include <QValidator>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonArray>

#include "ovl_requester.h"

namespace Ui {
class homewindow;
}

class homewindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit homewindow(QWidget *parent = nullptr);
        ~homewindow();
        bool rqInstAddr();
        void setRequester(ovl_requester* apApi);

    private slots:
        void on_pB_send_clicked();

        void on_lE_email_textChanged(const QString &arg1);

        void on_lE_pswd_textChanged(const QString &arg1);

        void on_pB_chgMode_clicked();

        void on_login_rq_finished(QNetworkReply *reply);

        void on_signup_rq_finished(QNetworkReply *reply);

    private:
        Ui::homewindow *ui;
        QString instAddr;
        ovl_requester* pApi;
        bool signupMode = false;
        QWidget *pMainWin;

        void checkCredentialsFilled();

    signals:
        void sig_requestMainWindow();
};

#endif // HOMEWINDOW_H
