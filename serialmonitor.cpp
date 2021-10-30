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

#include "serialmonitor.h"
#include "ui_serialmonitor.h"

#include <QMessageBox>
#include <QSerialPortInfo>
#include <QScrollBar>
#include <QDebug>

SerialMonitor::SerialMonitor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SerialMonitor),
      m_serial(new QSerialPort(this)),
      move_flag(false),
      Enc1(0),
      Enc2(0),
      absE1(0),
      absE2(0),
      init_flag(false)
{
    ui->setupUi(this);

    connect(m_serial, &QSerialPort::errorOccurred, this, &SerialMonitor::handleError);
    connect(m_serial, &QSerialPort::readyRead, this, &SerialMonitor::readData);

    connect(ui->send, &QPushButton::clicked, this, &SerialMonitor::writeData);
    connect(ui->connect, &QPushButton::clicked, this, &SerialMonitor::openSerialPort);
    connect(ui->move, &QPushButton::clicked, this, &SerialMonitor::move_motors);
    connect(ui->wheelmode, &QPushButton::clicked, this, &SerialMonitor::Wheel_Mode);
    connect(ui->positionmode, &QPushButton::clicked, this, &SerialMonitor::Position_Mode);
    connect(ui->freedrive, &QPushButton::clicked, this, &SerialMonitor::FreeDrive_Mode);

    fillPortsInfo();

}

SerialMonitor::~SerialMonitor()
{
    closeSerialPort();
    delete ui;
    delete m_serial;
}


void SerialMonitor::openSerialPort()
{
    m_serial->setPortName(ui->portlist->currentText());
    m_serial->setBaudRate(500000);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setParity(QSerialPort::Parity::NoParity);
    m_serial->setStopBits(QSerialPort::StopBits::OneStop);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);
    if (not m_serial->open(QIODevice::ReadWrite))
        QMessageBox::critical(this, tr("Error"), m_serial->errorString());
}

void SerialMonitor::closeSerialPort()
{
    if (m_serial->isOpen())
        m_serial->close();
}

void SerialMonitor::writeData()
{
    m_serial->write(ui->command->text().toLocal8Bit());
}

void SerialMonitor::readData()
{
    const QByteArray data = m_serial->readLine();
    putData(data);
}

void SerialMonitor::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), m_serial->errorString());
        closeSerialPort();
    }
}

void SerialMonitor::putData(const QByteArray &data)
{
    int E1, E2;
    const int N = 4096;
    QString msg = QString(data);
    QList<QString> Item = msg.split(",");
    if (Item.size()>2)
    {
        E1 = Item[0].toInt();
        E2 = Item[1].toInt();
        // This step is to pass the initial condition for converting between Absolute and Incremental encoders.
        if (init_flag==false)
        {
            absE1 = E1;
            absE2 = E2;
            init_flag = true;
        }
        // Do the conversions
        Absolute2Increment(Enc1, absE1, E1, N);
        Absolute2Increment(Enc2, absE2, E2, N);
        // qDebug() << E1 << E2 << Enc1 << Enc2;
        ui->Q1->setText(QString::number(Enc1));
        ui->Q2->setText(QString::number(Enc2));
    }
    ui->console->insertPlainText(data);
    QScrollBar *bar = ui->console->verticalScrollBar();
    bar->setValue(bar->maximum());
}

void SerialMonitor::fillPortsInfo()
{
    ui->portlist->clear();
    QString description;
    QString manufacturer;
    QString serialNumber;
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        QStringList list;
        list << info.portName();

        ui->portlist->addItem(list.first(), list);
    }

    ui->portlist->addItem(tr("Custom"));
}

void SerialMonitor::move_motors()
{
    move_flag = not move_flag;
    if(move_flag)
    {
        ui->command->setText("M");
        ui->move->setText("Stop");
        writeData();
    }
    else
    {
        ui->command->setText("S");
        ui->move->setText("Move");
        writeData();
    }
}

void SerialMonitor::Wheel_Mode()
{
    ui->command->setText("V");
    writeData();
}
void SerialMonitor::Position_Mode()
{
    ui->command->setText("P");
    writeData();
}
void SerialMonitor::FreeDrive_Mode()
{
    ui->command->setText("F");
    writeData();
}

void SerialMonitor::Absolute2Increment(int &Enc, int &absE, int E, const int N)
{
    int e;
    e = E - absE;
    if (e > N/2)             // eg: 4095 - 5  , decreasing pass a round
        Enc = Enc - N + e;
    else if (e < -N/2)       // eg: 5 - 4095, increasing pass a round
        Enc = Enc + N + e;
    else                     // eg:200 - 250
        Enc = Enc + e;
    absE = E;
}
