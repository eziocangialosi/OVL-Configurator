#include "homewindow.h"
#include "ui_homewindow.h"

homewindow::homewindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::homewindow)
{
    ui->setupUi(this);
    this->pMainWin = parent;
    this->setAttribute(Qt::WA_DeleteOnClose);
//    this->setWindowIcon(QIcon(":/ovl-icon.png"));//Set the application icon
    QPixmap pix_logo(":/OVL-Config_logo_name_white.png");
    ui->lb_logo->setPixmap(pix_logo);
    QRegularExpression emailRegex(
        "[-a-z0-9!#$%&'*+/=?^_`{|}~]+(?:\\.[-a-z0-9!#$%&'*+/=?^_`{|}~]+)*@(?:[a-z0-9](?:[-a-z0-9]*[a-z0-9])?\\.)+[a-z0-9](?:[-a-z0-9]*[a-z0-9])?");
    QValidator *validator_Email = new QRegularExpressionValidator(emailRegex,this);
    this->ui->lE_email->setValidator(validator_Email);
    QRegularExpression pswdRegex("\\S+");
    QValidator *validator_pswd = new QRegularExpressionValidator(pswdRegex,this);
    this->ui->lE_pswd->setValidator(validator_pswd);
}

homewindow::~homewindow()
{
    delete ui;
}

bool homewindow::rqInstAddr()
{
    bool addrOk = false;
    QString aInstAddr = "127.0.0.1";
    do{
        QInputDialog dialAddr;
        dialAddr.setCancelButtonText("Quit");
        dialAddr.setTextValue(aInstAddr);
        dialAddr.setLabelText("Address of your API instance :                      ");
        dialAddr.setWindowTitle("API Instance address");
        dialAddr.exec();
        aInstAddr = dialAddr.textValue();
        if (dialAddr.result() && !aInstAddr.isEmpty()){
            QMessageBox dialWait;
            dialWait.setStandardButtons(QMessageBox::NoButton);
            dialWait.setWindowTitle("API Instance address");
            QString labelWait = "Currently testing : " + aInstAddr + "\nPlease wait...";
            dialWait.setText(labelWait);
            dialWait.show();
            std::string cmdPing_std = "ping -c 1 " + aInstAddr.toStdString();
            if(system(cmdPing_std.c_str()) == 0){
                dialWait.setText("Address valid !");
                dialWait.setStandardButtons(QMessageBox::Ok);
                dialWait.exec();
                addrOk = true;
                this->api.setInstanceAddr(aInstAddr);
            }else{
                dialWait.setText("The address is not valid, please retry");
                dialWait.setStandardButtons(QMessageBox::Retry);
                dialWait.exec();
                addrOk = false;
            }
        }else if(!dialAddr.result()){
            this->close();
            return false;
        }else{
            QMessageBox::warning(this,"API Instance address","The program need an api instance address to work...\nPlease retry",
                                 QMessageBox::Ok);
            addrOk = false;
        }
    }while(!addrOk);
    return true;
}

void homewindow::checkCredentialsFilled(){
    bool allOk = false;

    QString emailTxt = this->ui->lE_email->text();
    int i = 0;

    if(!this->ui->lE_email->text().isEmpty() && this->ui->lE_email->validator()->validate(emailTxt,i) != QValidator::Acceptable){
        this->ui->lE_email->setStyleSheet("color: red");
        allOk = false;
    }else{
        this->ui->lE_email->setStyleSheet("color: white");
        allOk = true;
    }

    if(this->ui->lE_pswd->text().isEmpty() || this->ui->lE_pswd->text().isEmpty()){
        allOk = false;
    }

    if(allOk){
        this->ui->pB_send->setEnabled(true);
    }else{
        this->ui->pB_send->setEnabled(false);
    }
}

/***********************************/
/*             SLOTS               */
/***********************************/

void homewindow::on_pB_send_clicked()
{
    if(!this->signupMode){
        this->api.login(this->ui->lE_email->text(),this->ui->lE_pswd->text());
        connect(&this->api.manager,&QNetworkAccessManager::finished,this,&homewindow::on_login_rq_finished);
    }else{
        this->api.new_user(this->ui->lE_email->text(),this->ui->lE_pswd->text());
        connect(&this->api.manager,&QNetworkAccessManager::finished,this,&homewindow::on_signup_rq_finished);
    }
}


void homewindow::on_lE_email_textChanged(const QString &arg1)
{
    this->checkCredentialsFilled();
}


void homewindow::on_lE_pswd_textChanged(const QString &arg1)
{
    this->checkCredentialsFilled();
}


void homewindow::on_pB_chgMode_clicked()
{
    if(!signupMode){
        this->ui->lb_login->setText("Sign Up");
        this->ui->pB_chgMode->setText("Login");
        this->ui->pB_send->setText("Sign Up");
        this->ui->lb_account->setText("If you already have an account on this instance :");
        this->setWindowTitle("OVL-Configurator - Sign Up");
        signupMode = true;
    }else{
        this->ui->lb_login->setText("Login");
        this->ui->pB_chgMode->setText("Sign Up");
        this->ui->pB_send->setText("Login");
        this->ui->lb_account->setText("If you don't already have an account on this instance :");
        this->setWindowTitle("OVL-Configurator - Login");
        signupMode = false;
    }
}

void homewindow::on_login_rq_finished(QNetworkReply *reply)
{
    if(reply->errorString() == "Unknown error"){
        QJsonObject response = api.readJson(QString::fromStdString(reply->readAll().toStdString()));
        QJsonObject errorBranch = response["error"].toObject();
        QString aUserToken = response["user"].toString();
        int errorCode = errorBranch["Code"].toInt();
        switch(errorCode){
            case 0:
                this->api.setUserToken(&aUserToken);
                this->close();
                emit sig_requestMainWindow();
                break;
            case 30:
                QMessageBox::warning(this,"Error",errorBranch["Message"].toString(),QMessageBox::Ok);
                break;
            case 32:
                QMessageBox::warning(this,"Error",errorBranch["Message"].toString(),QMessageBox::Ok);
                break;
            default:
                QMessageBox::warning(this,"Error","An API error occured...\nPlease retry...",QMessageBox::Ok);
        }
    }else{
        QMessageBox::warning(this, "Error", "An error occured during the login request...\nPlease retry...",QMessageBox::Ok);
    }
    disconnect(&this->api.manager,&QNetworkAccessManager::finished,this,&homewindow::on_login_rq_finished);
    this->api.resetRequest();
}

void homewindow::on_signup_rq_finished(QNetworkReply *reply)
{
    if(reply->errorString() == "Unknown error"){
        QJsonObject response = api.readJson(QString::fromStdString(reply->readAll().toStdString()));
        QJsonObject errorBranch = response["error"].toObject();
        int errorCode = errorBranch["Code"].toInt();
        switch(errorCode){
            case 0:
                QMessageBox::information(this,"Success","The account has been successfully created !",QMessageBox::Ok);
                this->on_pB_chgMode_clicked();
                break;
            case 31:
                QMessageBox::warning(this,"Error","An account already use this email...\nPlease try with another...",QMessageBox::Ok);
                break;
            case 32:
                QMessageBox::warning(this,"Error",errorBranch["Message"].toString(),QMessageBox::Ok);
                break;
            default:
                qDebug()<< errorBranch["Message"];
                qDebug()<< errorBranch["Code"];
                qDebug()<< response;
                QMessageBox::warning(this,"Error","An API error occured...\nPlease retry...",QMessageBox::Ok);
                break;
        }
    }else{
        QMessageBox::warning(this, "Error", "An error occured during the login request...\nPlease retry...",QMessageBox::Ok);
    }
    disconnect(&this->api.manager,&QNetworkAccessManager::finished,this,&homewindow::on_signup_rq_finished);
    this->api.resetRequest();
}
