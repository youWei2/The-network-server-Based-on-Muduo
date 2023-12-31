/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   09_tcpclient
* @brief         mainwindow.cpp
* @author        WeiTe
*******************************************************************/
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置主窗体的位置与大小 */
    this->setGeometry(0, 0, 800, 480);

    /* tcp套接字 */
    tcpSocket = new QTcpSocket(this);

    /* 开始监听按钮 */
    pushButton[0] = new QPushButton();
    /* 停止监听按钮 */
    pushButton[1] = new QPushButton();
    /* 清空聊天文本按钮 */
    pushButton[2] = new QPushButton();
    /* 发送消息按钮 */
    pushButton[3] = new QPushButton();

    /* 水平布局一 */
    hBoxLayout[0] = new QHBoxLayout();
    /* 水平布局二 */
    hBoxLayout[1] = new QHBoxLayout();
    /* 水平布局三 */
    hBoxLayout[2] = new QHBoxLayout();
    /* 水平布局四 */
    hBoxLayout[3] = new QHBoxLayout();

    /* 水平容器一 */
    hWidget[0] =  new QWidget();
    /* 水平容器二 */
    hWidget[1] =  new QWidget();
    /* 水平容器三 */
    hWidget[2] =  new QWidget();


    vWidget = new QWidget();
    vBoxLayout = new QVBoxLayout();

    /* 标签实例化 */
    label[0] = new QLabel();
    label[1] = new QLabel();

    lineEdit = new QLineEdit();
    comboBox = new QComboBox();
    spinBox = new QSpinBox();
    textBrowser = new QTextBrowser();

    label[0]->setText("服务器地址：");
    label[1]->setText("服务器端口：");

    /* 设置标签根据文本文字大小自适应大小  */
    label[0]->setSizePolicy(QSizePolicy::Fixed,
                            QSizePolicy::Fixed);
    label[1]->setSizePolicy(QSizePolicy::Fixed,
                            QSizePolicy::Fixed);

    /* 设置端口号的范围，注意不要与主机的已使用的端口号冲突 */
    spinBox->setRange(8000, 10000);

    pushButton[0]->setText("连接服务器");
    pushButton[1]->setText("断开连接");
    pushButton[2]->setText("清空文本");
    pushButton[3]->setText("发送消息");

    /* 设置停止监听状态不可用 */
    pushButton[1]->setEnabled(false);

    /* 设置输入框默认的文本 */
    lineEdit->setText("MyMuduo网络服务器的连接：");

    /* 水平布局一添加内容 */
    hBoxLayout[0]->addWidget(pushButton[0]);
    hBoxLayout[0]->addWidget(pushButton[1]);
    hBoxLayout[0]->addWidget(pushButton[2]);

    /* 设置水平容器的布局为水平布局一 */
    hWidget[0]->setLayout(hBoxLayout[0]);

    hBoxLayout[1]->addWidget(label[0]);
    hBoxLayout[1]->addWidget(comboBox);
    hBoxLayout[1]->addWidget(label[1]);
    hBoxLayout[1]->addWidget(spinBox);

    /* 设置水平容器的布局为水平布局二 */
    hWidget[1]->setLayout(hBoxLayout[1]);

    /* 水平布局三添加内容 */
    hBoxLayout[2]->addWidget(lineEdit);
    hBoxLayout[2]->addWidget(pushButton[3]);

    /* 设置水平容器三的布局为水平布局一 */
    hWidget[2]->setLayout(hBoxLayout[2]);

    /* 垂直布局添加内容 */
    vBoxLayout->addWidget(textBrowser);
    vBoxLayout->addWidget(hWidget[1]);
    vBoxLayout->addWidget(hWidget[0]);
    vBoxLayout->addWidget(hWidget[2]);

    /* 设置垂直容器的布局为垂直布局 */
    vWidget->setLayout(vBoxLayout);

    /* 居中显示 */
    setCentralWidget(vWidget);

    /* 获取本地ip */
    getLocalHostIP();

    /* 信号槽连接 */
    connect(pushButton[0], SIGNAL(clicked()),
            this, SLOT(toConnect()));
    connect(pushButton[1], SIGNAL(clicked()),
            this, SLOT(toDisConnect()));
    connect(pushButton[2], SIGNAL(clicked()),
            this, SLOT(clearTextBrowser()));
    connect(pushButton[3], SIGNAL(clicked()),
            this, SLOT(sendMessages()));
    connect(tcpSocket, SIGNAL(connected()),
            this, SLOT(connected()));
    connect(tcpSocket, SIGNAL(disconnected()),
            this, SLOT(disconnected()));
    connect(tcpSocket, SIGNAL(readyRead()),
            this, SLOT(receiveMessages()));
    connect(tcpSocket,
            SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this,
            SLOT(socketStateChange(QAbstractSocket::SocketState)));
}

MainWindow::~MainWindow()
{
}

void MainWindow::toConnect()
{
    /* 如果连接状态还没有连接 */
    if (tcpSocket->state() != tcpSocket->ConnectedState) {
        /* 指定IP地址和端口连接 */
        tcpSocket->connectToHost(IPlist[comboBox->currentIndex()],
                spinBox->value());
    }
}

void MainWindow::toDisConnect()
{
    /* 断开连接 */
    tcpSocket->disconnectFromHost();

    /* 关闭socket*/
    tcpSocket->close();
}

void MainWindow::connected()
{
    /* 显示已经连接 */
    textBrowser->append("已经连上服务端");

    /* 设置按钮与下拉列表框的状态 */
    pushButton[0]->setEnabled(false);
    pushButton[1]->setEnabled(true);
    comboBox->setEnabled(false);
    spinBox->setEnabled(false);
}

void MainWindow::disconnected()
{
    /* 显示已经断开连接 */
    textBrowser->append("已经断开服务端");

    /* 设置按钮与下拉列表框的状态  */
    pushButton[1]->setEnabled(false);
    pushButton[0]->setEnabled(true);
    comboBox->setEnabled(true);
    spinBox->setEnabled(true);
}

/* 获取本地IP */
void MainWindow::getLocalHostIP()
{
    /* 获取所有的网络接口，
     * QNetworkInterface类提供主机的IP地址和网络接口的列表 */
    QList<QNetworkInterface> list
            = QNetworkInterface::allInterfaces();

    /* 遍历list */
    foreach (QNetworkInterface interface, list) {

        /* QNetworkAddressEntry类存储IP地址子网掩码和广播地址 */
        QList<QNetworkAddressEntry> entryList
                = interface.addressEntries();

        /* 遍历entryList */
        foreach (QNetworkAddressEntry entry, entryList) {
            /* 过滤IPv6地址，只留下IPv4 */
            if (entry.ip().protocol() ==
                    QAbstractSocket::IPv4Protocol) {
                comboBox->addItem(entry.ip().toString());
                /* 添加到IP列表中 */
                IPlist<<entry.ip();
            }
        }
    }
}

/* 清除文本浏览框里的内容 */
void MainWindow::clearTextBrowser()
{
    /* 清除文本浏览器的内容 */
    textBrowser->clear();
}

/* 客户端接收消息 */
void MainWindow::receiveMessages()
{
    /* 读取接收到的消息 */
    QString messages = tcpSocket->readAll();
    textBrowser->append("服务端：" + messages);
}

/* 客户端发送消息 */
void MainWindow::sendMessages()
{
    if(NULL == tcpSocket)
        return;

    if(tcpSocket->state() == tcpSocket->ConnectedState) {
        /* 客户端显示发送的消息 */
        textBrowser->append("客户端：" + lineEdit->text());

        /* 发送消息 */
        tcpSocket->write(lineEdit->text().toUtf8().data());
    }
}

/* 客户端状态改变 */
void MainWindow::socketStateChange(QAbstractSocket::SocketState state)
{
    switch (state) {
    case QAbstractSocket::UnconnectedState:
        textBrowser->append("scoket状态：UnconnectedState");
        break;
    case QAbstractSocket::ConnectedState:
        textBrowser->append("scoket状态：ConnectedState");
        break;
    case QAbstractSocket::ConnectingState:
        textBrowser->append("scoket状态：ConnectingState");
        break;
    case QAbstractSocket::HostLookupState:
        textBrowser->append("scoket状态：HostLookupState");
        break;
    case QAbstractSocket::ClosingState:
        textBrowser->append("scoket状态：ClosingState");
        break;
    case QAbstractSocket::ListeningState:
        textBrowser->append("scoket状态：ListeningState");
        break;
    case QAbstractSocket::BoundState:
        textBrowser->append("scoket状态：BoundState");
        break;
    default:
        break;
    }
}
