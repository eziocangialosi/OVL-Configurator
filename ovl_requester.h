#ifndef OVL_REQUESTER_H
#define OVL_REQUESTER_H

#include <QtNetwork/QNetworkAccessManager>
#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>

class ovl_requester : public QObject
{
        Q_OBJECT

    public:
//        explicit ovl_requester(QObject *parent = nullptr);

        void setInstanceAddr(QString aInstAddr);
        void login(QString aEmail, QString aPswd);
        void new_user(QString aEmail, QString aPswd);
        void get_tracker();
        QJsonObject readJson(QString inputJsonStr);
        void resetRequest();
        void disconnectUser();
        void setUserToken(QString* paUserToken);

        QNetworkAccessManager manager;
        QString userToken;
        bool userConnected = false;
    private:
        QString instanceAddr;
        QNetworkRequest* pRq;

        void requestHeading(QString endpoint);

};

#endif // OVL_REQUESTER_H
