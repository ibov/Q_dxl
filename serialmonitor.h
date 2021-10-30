/* This example is created for testing:
 * -  Serial connection
 * -  Test two dynamixel motors (MX-28AT)
 * -  Doing the conversion from absolute encoder (of dynamixel motors) to incremental encoder
 *
 * With this, one can use the dynamixel motors for several applications:
 * -  Control robotic arms (serial or parallel types) - which often use absolute encoders
 * -  Control autonomous mobile robots - which often use incremental encoders
 *    (eg. publishing odometry in ROS-based applications)
 *
 * Author: N.D.Quan - 30/10/2021
 */

#ifndef SERIALMONITOR_H
#define SERIALMONITOR_H

#include <QMainWindow>
#include <QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui { class SerialMonitor; }
QT_END_NAMESPACE

class SerialMonitor : public QMainWindow
{
    Q_OBJECT

public:
    SerialMonitor(QWidget *parent = nullptr);

    ~SerialMonitor();

private slots:
    void openSerialPort();
    void closeSerialPort();
    void writeData();
    void readData();
    void handleError(QSerialPort::SerialPortError error);
    void move_motors();
    void Wheel_Mode();
    void Position_Mode();
    void FreeDrive_Mode();

private:
    void fillPortsInfo();
    void exit();
    void putData(const QByteArray &data);
    void Absolute2Increment( int &Enc, int &absE, int E, const int N);
    Ui::SerialMonitor *ui;
    QSerialPort *m_serial = nullptr;
    QByteArray data;
    bool move_flag;
    int Enc1, Enc2;     // Store increment encoder values
    int absE1, absE2;   // Store previous Absolute encoder values
    bool init_flag;
};
#endif // SERIALMONITOR_H
