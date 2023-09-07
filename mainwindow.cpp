#include "mainwindow.h"
#include <QDebug>
#include <QGuiApplication>
#include <QScreen>
#include <QRect>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 布局初始化 */
    layoutInit();

    /* 扫描系统的串口 */
    scanSerialPort();

    /* 波特率项初始化 */
    baudRateItemInit();

    /* 数据位项初始化 */
    dataBitsItemInit();

    /* 检验位项初始化 */
    parityItemInit();

    /* 停止位项初始化 */
    stopBitsItemInit();

    /*接收数据格式初始化*/
    readdataFormatInit();

    /*发送数据格式初始化*/
    senddataFormatInit();
}

void MainWindow::layoutInit()
    {
    /* 获取屏幕的分辨率，Qt 官方建议使用这
    * 种方法获取屏幕分辨率，防上多屏设备导致对应不上
    * 注意，这是获取整个桌面系统的分辨率
    */
    QList <QScreen *> list_screen = QGuiApplication::screens();

    /* 如果是 ARM 平台，直接设置大小为屏幕的大小 */
    #if __arm__
    /* 重设大小 */
    this->resize(list_screen.at(0)->geometry().width(),
    list_screen.at(0)->geometry().height());
    #else
    /* 否则则设置主窗体大小为 800x480 */
    this->resize(800, 480);
    #endif
    /* 初始化 */
    serialPort = new QSerialPort(this);
    textBrowser = new QTextBrowser();
    textEdit = new QTextEdit();
    vboxLayout = new QVBoxLayout();
    vboxLayout1 = new QVBoxLayout();
    vboxLayout2 = new QVBoxLayout();
    hboxlayout = new QHBoxLayout();
    hboxlayout1 = new QHBoxLayout();
    funcWidget = new QWidget();
    readdataWidget = new QWidget();
    readdatafunWidget = new QWidget();
    senddataWidget = new QWidget();
    senddatafunWidget = new QWidget();
    mainWidget = new QWidget();
    gridLayout = new QGridLayout();
    vSpacer = new QSpacerItem(10, 10, QSizePolicy::Minimum,QSizePolicy::Expanding);
    timer = new QTimer(this);

    /*定时器1000ms*/
    timer->start(4000);


    /* QList 链表，字符串类型 */
    QList <QString> list1;
    list1<<"波特率:"<<"数据位:"<<"检验位:"<<"停止位:"<<"接收数据格式"<<"发送数据格式";

    for (int i = 0; i < 6; i++) {
        label[i] = new QLabel(list1[i]);
        /* 设置最小宽度与高度 */
        label[i]->setMinimumSize(80, 30);
        if(i<4){
            /* 自动调整 label 的大小 */
            label[i]->setSizePolicy(
            QSizePolicy::Expanding,
            QSizePolicy::Expanding
            );
            /* 将 label[i]添加至网格的坐标(0, i) */
            gridLayout->addWidget(label[i], 0, i+1);
        }
        else
        {
            //label[i]->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
        }

    }

    for (int i = 0; i < 7; i++) {
        comboBox[i] = new QComboBox();
        comboBox[i]->setMinimumSize(80, 30);
        if(i<5){
            /* 自动调整 combobox 的大小 */
            comboBox[i]->setSizePolicy(
            QSizePolicy::Expanding,
            QSizePolicy::Expanding
            );
            /* 将 comboBox[i]添加至网格的坐标(1, i) */
            if(i<5)
            gridLayout->addWidget(comboBox[i], 1, i);
        }else
        {
            //comboBox[i]->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
        }
    }

    /* QList 链表，字符串类型 */
    QList <QString> list2;
    list2<<"发送"<<"打开串口"<<"保存"<<"清空接收区"<<"清空发送区"<<"更新串口号:";

    for (int i = 0; i < 6; i++) {
        pushButton[i] = new QPushButton(list2[i]);
        pushButton[i]->setMinimumSize(80, 30);
        if(i<2){
            /* 自动调整 label 的大小 */
            pushButton[i]->setSizePolicy(
            QSizePolicy::Expanding,
            QSizePolicy::Expanding
            );
            /* 将 pushButton[0]添加至网格的坐标(i, 5) */
            gridLayout->addWidget(pushButton[i], i, 5);
        }
    }
    gridLayout->addWidget(pushButton[5],0,0);
    pushButton[0]->setEnabled(false);

    /*数据接收功能区布局*/
    vboxLayout1->addWidget(label[4]);
    vboxLayout1->addWidget(comboBox[5]);
    vboxLayout1->addWidget(pushButton[2]);
    vboxLayout1->addWidget(pushButton[3]);
    vboxLayout1->addSpacerItem(vSpacer);
    readdatafunWidget->setLayout(vboxLayout1);

    /*数据区布局*/
    hboxlayout->addWidget(textBrowser);
    hboxlayout->addWidget(readdatafunWidget);
    readdataWidget->setLayout(hboxlayout);

    /*数据发送功能区布局*/
    vboxLayout2->addWidget(label[5]);
    vboxLayout2->addWidget(comboBox[6]);
    vboxLayout2->addWidget(pushButton[4]);
    vboxLayout2->addSpacerItem(vSpacer);
    senddatafunWidget->setLayout(vboxLayout2);

    /*数据发送区布局*/
    hboxlayout1->addWidget(textEdit);
    hboxlayout1->addWidget(senddatafunWidget);
    senddataWidget->setLayout(hboxlayout1);
    //senddataWidget->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

    /* 整体布局 */
    vboxLayout->addWidget(readdataWidget);
    vboxLayout->addWidget(senddataWidget);
    funcWidget->setLayout(gridLayout);
    vboxLayout->addWidget(funcWidget);
    vboxLayout->setStretchFactor(readdataWidget,5);
    vboxLayout->setStretchFactor(senddataWidget,2);
    vboxLayout->setStretchFactor(funcWidget,1);
    mainWidget->setLayout(vboxLayout);
    this->setCentralWidget(mainWidget);

    /* 占位文本 */
    textBrowser->setPlaceholderText("接收到的消息");
    textEdit->setText("www.openedv.com");

    /* 信号槽连接 */
    connect(pushButton[0], SIGNAL(clicked()),this, SLOT(sendPushButtonClicked()));
    connect(pushButton[1], SIGNAL(clicked()),this, SLOT(openSerialPortPushButtonClicked()));
    connect(pushButton[2], SIGNAL(clicked()),this, SLOT(savedataPushButtonClicked()));
    connect(pushButton[3], SIGNAL(clicked()),this, SLOT(deleteReaddataPushButtonClicked()));
    connect(pushButton[4], SIGNAL(clicked()),this, SLOT(deleteSenddataPushButtonClicked()));
    connect(pushButton[5], SIGNAL(clicked()),this, SLOT(updataSerialPushButtonClicked()));

    /*串口信息接收*/
    connect(serialPort, SIGNAL(readyRead()),this, SLOT(serialPortReadyRead()));

    /*文本浏览器更新*/
    connect(textBrowser,SIGNAL(textChanged()),SLOT(slotTextTcpChanged()));

    /* 定时器信号槽连接 */
    connect(timer, SIGNAL(timeout()), this,SLOT(timerTimeOut()));

}

void MainWindow::scanSerialPort()
{
   /* 查找可用串口 */
    foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        int count = comboBox[0]->count();
         for(int i = 0; i < count + 1; i++)
         {
             if(comboBox[0]->itemText(i) == info.portName())
             {
                 //查找到了你需要的查找的字符串在combox中的位置
                 qDebug()<<"hh"<<endl;
             }
             else
             {
                 if(i == count)
                 comboBox[0]->addItem(info.portName());
             }
         }
        //comboBox[0]->addItem(info.portName());
    }

}

void MainWindow::baudRateItemInit()
{
    /* QList 链表，字符串类型 */
    QList <QString> list;
    list<<"1200"<<"2400"<<"4800"<<"9600"
    <<"19200"<<"38400"<<"57600"
    <<"115200"<<"230400"<<"460800"
    <<"921600";
    for (int i = 0; i < 11; i++) {
        comboBox[1]->addItem(list[i]);
    }
    comboBox[1]->setCurrentIndex(3);
}

void MainWindow::dataBitsItemInit()
{
    /* QList 链表，字符串类型 */
    QList <QString> list;
    list<<"5"<<"6"<<"7"<<"8";
    for (int i = 0; i < 4; i++) {
        comboBox[2]->addItem(list[i]);
    }
    comboBox[2]->setCurrentIndex(3);
}

void MainWindow::parityItemInit()
{
/* QList 链表，字符串类型 */
QList <QString> list;
list<<"None"<<"Even"<<"Odd"<<"Space"<<"Mark";
for (int i = 0; i < 5; i++) {
    comboBox[3]->addItem(list[i]);
    }
    comboBox[3]->setCurrentIndex(0);
}

void MainWindow::stopBitsItemInit()
{
    /* QList 链表，字符串类型 */
    QList <QString> list;
    list<<"1"<<"2";
    for (int i = 0; i < 2; i++) {
        comboBox[4]->addItem(list[i]);
    }
    comboBox[4]->setCurrentIndex(0);
}

void MainWindow::sendPushButtonClicked()
{
    if(comboBox[5]->currentText() == "utf8")
    {
        /* 获取 textEdit 数据,转换成 utf8 格式的字节流 */
        QByteArray data = textEdit->toPlainText().toUtf8();
        serialPort->write(data);
    }
    else if(comboBox[5]->currentText() == "Hex")
    {
        /* 获取 textEdit 数据,转换成 utf8 格式的字节流 */
        QByteArray data = textEdit->toPlainText().toUtf8().toHex();
        serialPort->write(data);
    }
    else if(comboBox[5]->currentText() == "二进制")
    {
        /* 获取 textEdit 数据,转换成 utf8 格式的字节流 */
        QByteArray data = textEdit->toPlainText().toUtf8();
        serialPort->write(data);
    }

}

void MainWindow::openSerialPortPushButtonClicked()
    {
    if (pushButton[1]->text() == "打开串口") {
    /* 设置串口名 */
    serialPort->setPortName(comboBox[0]->currentText());
    /* 设置波特率 */
    serialPort->setBaudRate(comboBox[1]->currentText().toInt());
    /* 设置数据位数 */
    switch (comboBox[2]->currentText().toInt()) {
            case 5:
            serialPort->setDataBits(QSerialPort::Data5);
            break;
            case 6:
            serialPort->setDataBits(QSerialPort::Data6);
            break;
            case 7:
            serialPort->setDataBits(QSerialPort::Data7);
            break;
            case 8:
            serialPort->setDataBits(QSerialPort::Data8);
            break;
            default: break;
        }
        /* 设置奇偶校验 */
        switch (comboBox[3]->currentIndex()) {
            case 0:
            serialPort->setParity(QSerialPort::NoParity);
            break;
            case 1:
            serialPort->setParity(QSerialPort::EvenParity);
            break;
            case 2:
            serialPort->setParity(QSerialPort::OddParity);
            break;
            case 3:
            serialPort->setParity(QSerialPort::SpaceParity);
            break;
            case 4:
            serialPort->setParity(QSerialPort::MarkParity);
            break;
            default: break;
        }
        /* 设置停止位 */
        switch (comboBox[4]->currentText().toInt()) {
            case 1:
            serialPort->setStopBits(QSerialPort::OneStop);
            break;
            case 2:
            serialPort->setStopBits(QSerialPort::TwoStop);
            break;
            default: break;
        }
        /* 设置流控制 */
        serialPort->setFlowControl(QSerialPort::NoFlowControl);
        if (!serialPort->open(QIODevice::ReadWrite))
        QMessageBox::about(NULL, "错误",
        "串口无法打开！可能串口已经被占用！");
        else {
            for (int i = 0; i < 5; i++)
            comboBox[i]->setEnabled(false);
            pushButton[1]->setText("关闭串口");
            pushButton[0]->setEnabled(true);
        }
    } else {
        serialPort->close();
        for (int i = 0; i < 5; i++)
        comboBox[i]->setEnabled(true);
        pushButton[1]->setText("打开串口");
        pushButton[0]->setEnabled(false);
    }
}

void MainWindow::serialPortReadyRead()
{
    /* 接收缓冲区中读取数据 */
    QByteArray buf = serialPort->readAll();
    textBrowser->insertPlainText(QString(buf));
}

void MainWindow::slotTextTcpChanged()
{
    //槽函数里的内容
    textBrowser->moveCursor(QTextCursor::End);
}

void MainWindow::readdataFormatInit()
{
    /* QList 链表，字符串类型 */
    QList <QString> list;
    list<<"Hex"<<"utf8"<<"二进制";
    for (int i = 0; i < 3; i++) {
        comboBox[5]->addItem(list[i]);
    }
    comboBox[5]->setCurrentIndex(1);
}

void MainWindow::senddataFormatInit()
{
    /* QList 链表，字符串类型 */
    QList <QString> list;
    list<<"Hex"<<"utf8"<<"二进制";
    for (int i = 0; i < 3; i++) {
        comboBox[6]->addItem(list[i]);
    }
    comboBox[6]->setCurrentIndex(1);
}

void MainWindow::timerTimeOut()
{
    //scanSerialPort();
}

bool MainWindow::savedataPushButtonClicked()
{
    // 指定目录和文件名
        QString directory = "path/to/directory";
        QString filename = "data.txt";


        // 创建文件路径
        QString filePath = directory + "/" + filename;

        // 创建文件
        QFile file(filePath);
    /* 获取文件的路径 */
    QString fileName = QFileDialog::getSaveFileName(this);

    /* 指向文件 */
    file.setFileName(fileName);

    /* 判断文件是否存在 */
    if (!file.exists())
    return false;

    /* 以读写的方式打开 */
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    return false;

    /* 使用文本流读取文件 */
    QTextStream stream(&file);

    /* 获取 textEdit 的文本内容，转为字符串 */
    QString str = textBrowser->toPlainText();

    /* 使用流提取运算符，写入文本流 */
    stream<<str;

    /* 关闭文件 */
    file.close();

    return true;
}

void MainWindow::deleteReaddataPushButtonClicked()
{
    textBrowser->clear();
}

void MainWindow::deleteSenddataPushButtonClicked()
{
    textEdit->clear();
}

void MainWindow::updataSerialPushButtonClicked()
{
    scanSerialPort();
}

MainWindow::~MainWindow()
{
}

