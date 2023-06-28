#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QImage>
#include <QWidget>
#include <QInputDialog>
#include <QMessageBox>
#include <QRegularExpression>
#include <QValidator>

namespace Ui {
class homewindow;
}

class homewindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit homewindow(QWidget *parent = nullptr);
    ~homewindow();
    void rqInstAddr();

private slots:
    void on_pB_send_clicked();

    void on_lE_email_textChanged(const QString &arg1);

    void on_lE_pswd_textChanged(const QString &arg1);

    void on_pB_chgMode_clicked();

private:
    Ui::homewindow *ui;
    QString instAddr;
    bool signupMode = false;

    void checkCredentialsFilled();
};

#endif // HOMEWINDOW_H
