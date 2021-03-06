#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "banddialog.h"

#include <QMessageBox>
#include <QtSerialPort/QSerialPort>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    myData.resize(1); //Setup to transmit a single byte
    serial = new QSerialPort(this);
    settings = new SettingsDialog;

    status = new QLabel;
    ui->statusBar->addWidget(status);
    connect(serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &MainWindow::handleError);
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);
    readSettings();
}

MainWindow::~MainWindow()
{
    writeSettings();
    delete ui;
}
//    connect(ui->actionConfigure, &QAction::triggered, settings, &SettingsDialog::show);
void MainWindow::openSerialPort()
{
    SettingsDialog::Settings p = settings->settings();
    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
    if (serial->open(QIODevice::ReadWrite)) {
//        console->setEnabled(true);
//        console->setLocalEchoEnabled(p.localEchoEnabled);
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
//        ui->actionConfigure->setEnabled(false);
        ui->actionSettings->setEnabled(false);
        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                          .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                          .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());

        showStatusMessage(tr("Open error"));
    }
}
//! [4]

//! [5]
void MainWindow::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
//    console->setEnabled(false);
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
//    ui->actionConfigure->setEnabled(true);
    ui->actionSettings->setEnabled(true);
    showStatusMessage(tr("Disconnected"));
}

void MainWindow::writeData(const QByteArray &data)
{
    serial->write(data);
}

void MainWindow::on_actionClear_Display_triggered()
{
    ui->plainTextEdit->clear();
}

void MainWindow::readData()
{
    QByteArray data = serial->readAll();
//    console->putData(data);
    ui->plainTextEdit->insertPlainText(data);
    ui->plainTextEdit->ensureCursorVisible();
}


void MainWindow::showStatusMessage(const QString &message)
{
    status->setText(message);
}

void MainWindow::on_actionSettings_triggered()
{
    settings->show();
}

void MainWindow::on_actionsetBands_triggered()
{
    bandDialog band;
    connect(&band, SIGNAL(sendBandData(int,int)), this, SLOT(bandData(int,int)));
    connect(&band, SIGNAL(setBtnText(int,QString)), this, SLOT(getBtnText(int,QString)));
    band.exec();

}

void MainWindow::bandData(int band, int value)
{
    static int lastBand = 999;

    if(lastBand == band) {
        rValues[band] = value;
//        qDebug() << "rValues[" << band << "] = " <<rValues[band];
    } else {
        lValues[band] = value;
//        qDebug() << "lValues[" << band << "] = " <<lValues[band];
    }
    lastBand = band;
}

void MainWindow::getBtnText(int band, QString btnLabel)
{
    switch (band) {
    case 0:
        ui->pushButton->setText(btnLabel);
        break;
    case 1:
        ui->pushButton_1->setText(btnLabel);
        break;
    case 2:
        ui->pushButton_2->setText(btnLabel);
        break;
    case 3:
        ui->pushButton_3->setText(btnLabel);
        break;
    case 4:
        ui->pushButton_4->setText(btnLabel);
        break;
    case 5:
        ui->pushButton_5->setText(btnLabel);
        break;
    case 6:
        ui->pushButton_6->setText(btnLabel);
        break;
    case 7:
        ui->pushButton_7->setText(btnLabel);
        break;
    default:
        qDebug() << Q_FUNC_INFO << "button Label out of range";
        break;
    }
}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}

void MainWindow::on_pushButton_clicked()
{
    if(ui->pushButton_mox->isChecked()) {
        msg = QString("Band 0 clicked: data sent = %1 in binary and %2 decimal").arg(lValues[0], 8, 2, QChar('0'))
                                                                                .arg(lValues[0], 0, 10);
        showStatusMessage(msg);
        myData[0] = lValues[0];
        writeData(myData);
    } else {
        msg = QString("Band 0 clicked: data sent = %1 in binary and %2 decimal").arg(rValues[0], 8, 2, QChar('0'))
                                                                                .arg(rValues[0], 0, 10);
        showStatusMessage(msg);
        myData[0] = rValues[0];
        writeData(myData);
    }
    ui->pushButton->setChecked(true);
    ui->pushButton_1->setChecked(false);
    ui->pushButton_2->setChecked(false);
    ui->pushButton_3->setChecked(false);
    ui->pushButton_4->setChecked(false);
    ui->pushButton_5->setChecked(false);
    ui->pushButton_6->setChecked(false);
    ui->pushButton_7->setChecked(false);
}

void MainWindow::on_pushButton_1_clicked()
{
//    myData.resize(1);
    if(ui->pushButton_mox->isChecked()) {
        msg = QString("Band 1 clicked: data sent = %1 in binary and %2 decimal").arg(lValues[1], 8, 2, QChar('0'))
                                                                                .arg(lValues[1], 0, 10);
        showStatusMessage(msg);
        myData[0] = lValues[1];
        writeData(myData);
    } else {
        msg = QString("Band 1 clicked: data sent = %1 in binary and %2 decimal").arg(rValues[1], 8, 2, QChar('0'))
                                                                                .arg(rValues[1], 0, 10);
        showStatusMessage(msg);
        myData[0] = rValues[1];
        writeData(myData);
    }
    ui->pushButton->setChecked(false);
    ui->pushButton_1->setChecked(true);
    ui->pushButton_2->setChecked(false);
    ui->pushButton_3->setChecked(false);
    ui->pushButton_4->setChecked(false);
    ui->pushButton_5->setChecked(false);
    ui->pushButton_6->setChecked(false);
    ui->pushButton_7->setChecked(false);
}

void MainWindow::on_pushButton_2_clicked()
{
    if(ui->pushButton_mox->isChecked()) {
        msg = QString("Band 2 clicked: data sent = %1 in binary and %2 decimal").arg(lValues[2], 8, 2, QChar('0'))
                                                                                .arg(lValues[2], 0, 10);
        showStatusMessage(msg);
        myData[0] = lValues[2];
        writeData(myData);
    } else {
        msg = QString("Band 2 clicked: data sent = %1 in binary and %2 decimal").arg(rValues[2], 8, 2, QChar('0'))
                                                                                .arg(rValues[2], 0, 10);
        showStatusMessage(msg);
        myData[0] = rValues[2];
        writeData(myData);
    }
    ui->pushButton->setChecked(false);
    ui->pushButton_1->setChecked(false);
    ui->pushButton_2->setChecked(true);
    ui->pushButton_3->setChecked(false);
    ui->pushButton_4->setChecked(false);
    ui->pushButton_5->setChecked(false);
    ui->pushButton_6->setChecked(false);
    ui->pushButton_7->setChecked(false);
}

void MainWindow::on_pushButton_3_clicked()
{
    if(ui->pushButton_mox->isChecked()) {
        msg = QString("Band 3 clicked: data sent = %1 in binary and %2 decimal").arg(lValues[3], 8, 2, QChar('0'))
                                                                                .arg(lValues[3], 0, 10);
        showStatusMessage(msg);
        myData[0] = lValues[3];
        writeData(myData);
    } else {
        msg = QString("Band 3 clicked: data sent = %1 in binary and %2 decimal").arg(rValues[3], 8, 2, QChar('0'))
                                                                                .arg(rValues[3], 0, 10);
        showStatusMessage(msg);
        myData[0] = rValues[3];
        writeData(myData);
    }
    ui->pushButton->setChecked(false);
    ui->pushButton_1->setChecked(false);
    ui->pushButton_2->setChecked(false);
    ui->pushButton_3->setChecked(true);
    ui->pushButton_4->setChecked(false);
    ui->pushButton_5->setChecked(false);
    ui->pushButton_6->setChecked(false);
    ui->pushButton_7->setChecked(false);
}

void MainWindow::on_pushButton_4_clicked()
{
    if(ui->pushButton_mox->isChecked()) {
        msg = QString("Band 4 clicked: data sent = %1 in binary and %2 decimal").arg(lValues[4], 8, 2, QChar('0'))
                                                                                .arg(lValues[4], 0, 10);
        showStatusMessage(msg);
        myData[0] = lValues[4];
        writeData(myData);
    } else {
        msg = QString("Band 4 clicked: data sent = %1 in binary and %2 decimal").arg(rValues[4], 8, 2, QChar('0'))
                                                                                .arg(rValues[4], 0, 10);
        showStatusMessage(msg);
        myData[0] = rValues[4];
        writeData(myData);
    }
    ui->pushButton->setChecked(false);
    ui->pushButton_1->setChecked(false);
    ui->pushButton_2->setChecked(false);
    ui->pushButton_3->setChecked(false);
    ui->pushButton_4->setChecked(true);
    ui->pushButton_5->setChecked(false);
    ui->pushButton_6->setChecked(false);
    ui->pushButton_7->setChecked(false);
}

void MainWindow::on_pushButton_5_clicked()
{
    if(ui->pushButton_mox->isChecked()) {
        msg = QString("Band 5 clicked: data sent = %1 in binary and %2 decimal").arg(lValues[5], 8, 2, QChar('0'))
                                                                                .arg(lValues[5], 0, 10);
        showStatusMessage(msg);
        myData[0] = lValues[5];
        writeData(myData);
    } else {
        msg = QString("Band 5 clicked: data sent = %1 in binary and %2 decimal").arg(rValues[5], 8, 2, QChar('0'))
                                                                                .arg(rValues[5], 0, 10);
        showStatusMessage(msg);
        myData[0] = rValues[5];
        writeData(myData);
    }
    ui->pushButton->setChecked(false);
    ui->pushButton_1->setChecked(false);
    ui->pushButton_2->setChecked(false);
    ui->pushButton_3->setChecked(false);
    ui->pushButton_4->setChecked(false);
    ui->pushButton_5->setChecked(true);
    ui->pushButton_6->setChecked(false);
    ui->pushButton_7->setChecked(false);
}

void MainWindow::on_pushButton_6_clicked()
{
    if(ui->pushButton_mox->isChecked()) {
        msg = QString("Band 6 clicked: data sent = %1 in binary and %2 decimal").arg(lValues[6], 8, 2, QChar('0'))
                                                                                .arg(lValues[6], 0, 10);
        showStatusMessage(msg);
        myData[0] = lValues[6];
        writeData(myData);
    } else {
        msg = QString("Band 6 clicked: data sent = %1 in binary and %2 decimal").arg(rValues[6], 8, 2, QChar('0'))
                                                                                .arg(rValues[6], 0, 10);
        showStatusMessage(msg);
        myData[0] = rValues[6];
        writeData(myData);
    }
    ui->pushButton->setChecked(false);
    ui->pushButton_1->setChecked(false);
    ui->pushButton_2->setChecked(false);
    ui->pushButton_3->setChecked(false);
    ui->pushButton_4->setChecked(false);
    ui->pushButton_5->setChecked(false);
    ui->pushButton_6->setChecked(true);
    ui->pushButton_7->setChecked(false);
}

void MainWindow::on_pushButton_7_clicked()
{
    if(ui->pushButton_mox->isChecked()) {
        msg = QString("Band 7 clicked: data sent = %1 in binary and %2 decimal").arg(lValues[7], 8, 2, QChar('0'))
                                                                                .arg(lValues[7], 0, 10);
        showStatusMessage(msg);
        myData[0] = lValues[7];
        writeData(myData);
    } else {
        msg = QString("Band 7 clicked: data sent = %1 in binary and %2 decimal").arg(rValues[7], 8, 2, QChar('0'))
                                                                                .arg(rValues[7], 0, 10);
        showStatusMessage(msg);
        myData[0] = rValues[7];
        writeData(myData);
    }
    ui->pushButton->setChecked(false);
    ui->pushButton_1->setChecked(false);
    ui->pushButton_2->setChecked(false);
    ui->pushButton_3->setChecked(false);
    ui->pushButton_4->setChecked(false);
    ui->pushButton_5->setChecked(false);
    ui->pushButton_6->setChecked(false);
    ui->pushButton_7->setChecked(true);
}

void MainWindow::on_pushButton_mox_clicked()
{
    if(ui->pushButton->isChecked()) {
        on_pushButton_clicked();
    } else if (ui->pushButton_1->isChecked()) {
        on_pushButton_1_clicked();
    } else if (ui->pushButton_2->isChecked()) {
        on_pushButton_2_clicked();
    } else if (ui->pushButton_3->isChecked()) {
        on_pushButton_3_clicked();
    } else if (ui->pushButton_4->isChecked()) {
        on_pushButton_4_clicked();
    } else if (ui->pushButton_5->isChecked()) {
        on_pushButton_5_clicked();
    } else if (ui->pushButton_6->isChecked()) {
        on_pushButton_6_clicked();
    } else if (ui->pushButton_7->isChecked()) {
        on_pushButton_7_clicked();
    }
}

void MainWindow::on_actionConnect_triggered()
{
    openSerialPort();
}

void MainWindow::on_actionDisconnect_triggered()
{
    if (serial->isOpen())
        serial->close();
//    console->setEnabled(false);
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
//    ui->actionConfigure->setEnabled(true);
    ui->actionSettings->setEnabled(true);
    showStatusMessage(tr("Disconnected"));
}

void MainWindow::on_actionExit_triggered()
{
    MainWindow::close();
}

void MainWindow::writeSettings()
{
    QSettings settings("Hermes-Lite", "HL_I2C_Tester_Qt");

    settings.setValue("lValue_0", lValues[0]);
    settings.setValue("lValue_1", lValues[1]);
    settings.setValue("lValue_2", lValues[2]);
    settings.setValue("lValue_3", lValues[3]);
    settings.setValue("lValue_4", lValues[4]);
    settings.setValue("lValue_5", lValues[5]);
    settings.setValue("lValue_6", lValues[6]);
    settings.setValue("lValue_7", lValues[7]);

    settings.setValue("rValue_0", rValues[0]);
    settings.setValue("rValue_1", rValues[1]);
    settings.setValue("rValue_2", rValues[2]);
    settings.setValue("rValue_3", rValues[3]);
    settings.setValue("rValue_4", rValues[4]);
    settings.setValue("rValue_5", rValues[5]);
    settings.setValue("rValue_6", rValues[6]);
    settings.setValue("rValue_7", rValues[7]);
}

void MainWindow::readSettings()
{
    QSettings settings("Hermes-Lite", "HL_I2C_Tester_Qt");
    ui->pushButton->setText(settings.value("band0", "Band 0").toString());
    ui->pushButton_1->setText(settings.value("band1", "Band 1").toString());
    ui->pushButton_2->setText(settings.value("band2", "Band 2").toString());
    ui->pushButton_3->setText(settings.value("band3", "Band 3").toString());
    ui->pushButton_4->setText(settings.value("band4", "Band 4").toString());
    ui->pushButton_5->setText(settings.value("band5", "Band 5").toString());
    ui->pushButton_6->setText(settings.value("band6", "Band 6").toString());
    ui->pushButton_7->setText(settings.value("band7", "Band 7").toString());

    lValues[0] = (settings.value("lValue_0", "0").toInt());
    lValues[1] = (settings.value("lValue_1", "0").toInt());
    lValues[2] = (settings.value("lValue_2", "0").toInt());
    lValues[3] = (settings.value("lValue_3", "0").toInt());
    lValues[4] = (settings.value("lValue_4", "0").toInt());
    lValues[5] = (settings.value("lValue_5", "0").toInt());
    lValues[6] = (settings.value("lValue_6", "0").toInt());
    lValues[7] = (settings.value("lValue_7", "0").toInt());

    rValues[0] = (settings.value("rValue_0", "0").toInt());
    rValues[1] = (settings.value("rValue_1", "0").toInt());
    rValues[2] = (settings.value("rValue_2", "0").toInt());
    rValues[3] = (settings.value("rValue_3", "0").toInt());
    rValues[4] = (settings.value("rValue_4", "0").toInt());
    rValues[5] = (settings.value("rValue_5", "0").toInt());
    rValues[6] = (settings.value("rValue_6", "0").toInt());
    rValues[7] = (settings.value("rValue_7", "0").toInt());
}
