#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QtNetwork>
#include <QtCore>
#include <QSettings>
#include <QInputDialog>
#include <QMessageBox>

QSettings settings("WakeOnLAN", "WakeOnLAN");

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    // init parameters
    readSettings("broadcast");
    readSettings("port");
    delete ui;


}

QByteArray MainWindow::create_magic_packet(QString mac_address)
{
    // "F" * 12 + macaddress * 16
    if (mac_address.length() == 17){
        QString separator = mac_address[2];
        mac_address = mac_address.remove(separator);
    }
    else if (mac_address.length() != 12){
        QMessageBox::warning(this, tr("Wake on LAN"), tr("Wrong MAC Address format!"));
    }

    QString magic_packet("F");
    magic_packet = magic_packet.repeated(12);
    magic_packet.append(mac_address.repeated(16));
    return QByteArray::fromHex(magic_packet.toUtf8());;
}

void MainWindow::on_SaveButton_clicked()
{
    qDebug() << "SaveButton:" << ui->MACInput->text();
}

void MainWindow::on_SendPacketButton_clicked()
{
    settings.setValue("mac_address", ui->MACInput->text());

    udpSocket = new QUdpSocket(this);
    QByteArray datagram = MainWindow::create_magic_packet(settings.value("mac_address").toString());
    udpSocket->writeDatagram(datagram, QHostAddress(settings.value("broadcast").toString()), settings.value("port").toString().toInt());
}

void MainWindow::manualWriteSettings(const QString setting_name)
{
    std::string qstr = "Edit " + setting_name.toStdString();
    std::string qval = "Enter New value: (Current: " + settings.value(setting_name).toString().toStdString() + ")";
    const char* name = qstr.c_str();
    const char* value = qval.c_str();
    qDebug() << "Change" << setting_name<<":" << settings.value(setting_name).toString();
    QString userInput = QInputDialog::getText(this, tr(name), tr(value), QLineEdit::Normal);

    if (userInput.length() > 0){
        settings.setValue(setting_name, userInput);
    }
}

void MainWindow::writeSettings(const QString setting_name, const QString setting_value)
{
    qDebug() << "Change" << setting_name << settings.value(setting_name).toString() << "to" << setting_value;
    settings.setValue(setting_name, setting_value);
}

void MainWindow::readSettings(const QString setting_name)
{
    /**
     * Reads setting value from QSetting
     * If value does not exist, initializes it with default value
     */
    const auto setting = settings.value(setting_name, QByteArray()).toByteArray();
    if (setting.isEmpty()){
        if (setting_name == "broadcast")
            settings.setValue("broadcast", "192.168.1.255");
        else if (setting_name == "port")
            settings.setValue("port", "9");
    }
}

void MainWindow::on_actionEditPort_triggered()
{
    manualWriteSettings("port");
}

void MainWindow::on_actionEditBroadcast_IP_triggered()
{
    manualWriteSettings("broadcast");
}

void MainWindow::on_actionRestore_Defaults_triggered()
{
    writeSettings("broadcast", "192.168.1.255");
    writeSettings("port", "9");
}

