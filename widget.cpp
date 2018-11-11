#include "widget.h"
#include "ui_widget.h"
#include "controller.h"
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

void Widget::stream()
{
    qDebug()<<"Widget::Stream() IN";
    if (isStream)
    {
        isStream = false;
        ui->startButton->setText(QString::fromLocal8Bit("开始推流"));
        Controller::Get()->Stop();
    }
    else
    {
        isStream = true;
        QString url = ui->cameraLineEdit->text();
        qDebug()<<"cameraLineEdit "<<url;
        bool ok = false;
        int camIndex = url.toInt(&ok);
        if (!ok)
        {
            Controller::Get()->inUrl = url;
        }
        else
        {
            Controller::Get()->camIndex = camIndex;
        }
        Controller::Get()->outUrl = ui->uploadStreamLineEdit->text();
        qDebug()<<"uploadStreamLineEdit  "<<Controller::Get()->outUrl;
        Controller::Get()->Set((ui->faceComboBox->currentIndex() + 1) * 3 );
        qDebug()<<"faceComboBox->currentIndex()  "<<ui->faceComboBox->currentIndex();
        Controller::Get()->Start();
        ui->startButton->setText(QString::fromLocal8Bit("停止推流"));
    }
}

void Widget::closeEvent(QCloseEvent *event)
{
    qDebug()<<"Widget::closeEvent IN";
    Controller::Get()->Stop();
}

Widget::~Widget()
{
    delete ui;
}
