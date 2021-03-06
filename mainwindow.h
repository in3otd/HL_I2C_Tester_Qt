#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtCore/QtGlobal>
#include <QLabel>

#include "settingsdialog.h"
#include "banddialog.h"

namespace Ui {
class MainWindow;
}

class SettingsDialog;
class bandDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:


private slots:
    void readData();
    void writeData(const QByteArray &data);
    void openSerialPort();
    void closeSerialPort();
    void bandData(int band, int value);
    void getBtnText(int band, QString btnLabel);

//    void about();
    void handleError(QSerialPort::SerialPortError error);

    void on_actionsetBands_triggered();

    void on_pushButton_clicked();

    void on_pushButton_1_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_mox_clicked();

    void on_actionConnect_triggered();

    void on_actionDisconnect_triggered();

    void on_actionSettings_triggered();

    void on_actionExit_triggered();

    void on_actionClear_Display_triggered();

private:
    Ui::MainWindow *ui;

    void showStatusMessage(const QString &message);
    QLabel *status;
    SettingsDialog *settings;
//    bandDialog band;
    QSerialPort *serial;
    QByteArray myData;
    QString msg;
    int lValues[8];
    int rValues[8];
    void writeSettings();
    void readSettings();
};

#endif // MAINWINDOW_H
