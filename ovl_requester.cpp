#include "ovl_requester.h"

void ovl_requester::setInstanceAddr(QString aInstAddr)
{
    this->instanceAddr = aInstAddr;
}

void ovl_requester::login(QString aEmail, QString aPswd)
{
    QString login_url = "https://" + this->instanceAddr + "/user/" + aEmail + "/" + aPswd;
    this->requestHeading(login_url);
    this->manager.get(*pRq);
}

void ovl_requester::new_user(QString aEmail, QString aPswd)
{
    QString newuser_url = "https://" + this->instanceAddr + "/user/";
    QJsonObject jsonObject;
    jsonObject.insert("mail",aEmail);
    jsonObject.insert("password",aPswd);
    jsonObject.insert("notif","NaN");
    QString json_str = "{\"mail\":\"" + aEmail + "\",\"password\":\"" + aPswd + "\",\"notif\":\"undefined\"}";
    this->requestHeading(newuser_url);
    this->pRq->setRawHeader("Accept", "application/json");
    this->pRq->setRawHeader("Content-Type", "application/json");
    QByteArray jsonByteArray = QByteArray::fromStdString(json_str.toStdString());
    qDebug()<< jsonByteArray.toStdString();
    this->manager.post(*pRq,jsonByteArray);
}

void ovl_requester::requestHeading(QString endpoint)
{
    this->pRq = new QNetworkRequest(QUrl(endpoint));
}

QJsonObject ovl_requester::readJson(QString inputJsonStr)
{
    QJsonDocument d = QJsonDocument::fromJson(inputJsonStr.toUtf8());
    QJsonObject jsonExtracted = d.object();

    return jsonExtracted;
}

void ovl_requester::resetRequest(){
    delete this->pRq;
}

void ovl_requester::disconnectUser(){
    if(userConnected){
        this->resetUserCredentials();
    }
}
void ovl_requester::setUserToken(QString* paUserToken){
    this->userToken = *paUserToken;
    this->userConnected = true;
}

void ovl_requester::setUserMail(QString* paUserName){
    this->userMail = *paUserName;
}

QString ovl_requester::getUserMail(){
    return this->userMail;
}

void ovl_requester::resetUserCredentials(){
    this->userMail = "";
    this->userToken = "";
    this->userConnected = false;
}
