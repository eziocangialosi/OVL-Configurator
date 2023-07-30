#ifndef DIGITSPINBOX_H
#define DIGITSPINBOX_H

#include <QObject>
#include <QSpinBox>

class digitSpinBox : public QSpinBox
{
    Q_OBJECT
    public:
        digitSpinBox( QWidget * parent = 0) :
            QSpinBox(parent)
        {
        }

        virtual QString textFromValue ( int value ) const
        {
            /* 4 - number of digits, 10 - base of number, '0' - pad character*/
            return QString("%1").arg(value, this->digits , this->numBase, this->padChar);
        }

        void setDigits(int aDigits){
            this->digits = aDigits;
        }

        void setNumBase(int aNumBase){
            this->numBase = aNumBase;
        }

        void setPadChar(QChar aPadChar){
            this->padChar = aPadChar;
        }
    private:
        int digits = 4;
        int numBase = 10;
        QChar padChar = '0';
};

#endif // DIGITSPINBOX_H
