#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QUdpSocket;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_SaveButton_clicked();
    void on_SendPacketButton_clicked();
    void manualWriteSettings(const QString setting_name);
    void writeSettings(const QString setting_name, const QString setting_value);
    void readSettings(const QString setting_name);
    void on_actionEditPort_triggered();
    void on_actionEditBroadcast_IP_triggered();
    void on_actionRestore_Defaults_triggered();
    QByteArray create_magic_packet(QString mac_address);
private:
    Ui::MainWindow *ui;
    QUdpSocket *udpSocket = nullptr;
};
#endif // MAINWINDOW_H
