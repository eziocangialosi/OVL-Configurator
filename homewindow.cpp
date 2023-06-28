#include "homewindow.h"
#include "ui_homewindow.h"

homewindow::homewindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::homewindow)
{
    ui->setupUi(this);
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

void homewindow::rqInstAddr()
{
    bool addrOk = false;
    QString aInstAddr = "127.0.0.1";
    do{
        QInputDialog dialAddr;
        dialAddr.setCancelButtonText("Quit");
        dialAddr.setTextValue(aInstAddr);
        dialAddr.setLabelText("Address of your instance :                      ");
        dialAddr.setWindowTitle("Instance address");
        dialAddr.exec();
        aInstAddr = dialAddr.textValue();
        if (dialAddr.result() && !aInstAddr.isEmpty()){
            QMessageBox dialWait;
            dialWait.setStandardButtons(QMessageBox::NoButton);
            dialWait.setWindowTitle("Instance address");
            QString labelWait = "Currently testing : " + aInstAddr + "\nPlease wait...";
            dialWait.setText(labelWait);
            dialWait.show();
            std::string cmdPing_std = "ping -c 1 " + aInstAddr.toStdString();
            if(system(cmdPing_std.c_str()) == 0){
                dialWait.setText("Connection valid !");
                dialWait.setStandardButtons(QMessageBox::Ok);
                dialWait.exec();
                addrOk = true;
                this->instAddr = aInstAddr;
            }else{
                dialWait.setText("The address is not valid, please retry");
                dialWait.setStandardButtons(QMessageBox::Retry);
                dialWait.exec();
                addrOk = false;
            }
        }else if(!dialAddr.result()){
            addrOk = true;
            delete this;
        }else{
            QMessageBox::warning(this,"Instance address","The program need an instance address to work...\nPlease retry",
                                 QMessageBox::Ok);
            addrOk = false;
        }
    }while(!addrOk);
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
    QMessageBox::information(this,"co","log ok",QMessageBox::Ok);
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

