#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QPushButton>
#include <QTextBrowser>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QGridLayout>
#include <QMessageBox>
#include <QDebug>
#include <QSpacerItem>
#include <QTimer>
#include <QFile>
#include <QFileDialog>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* 垂直间隔 */
    QSpacerItem *vSpacer;

    /* 串口对象 */
    QSerialPort *serialPort;

    /* 用作接收数据 */
    QTextBrowser *textBrowser;
    /* 用作发送数据 */
    QTextEdit *textEdit;

    /* 按钮 */
    QPushButton *pushButton[6];

    /* 下拉选择盒子 */
    QComboBox *comboBox[7];

    /* 标签 */
    QLabel *label[6];

    /* QFile 类型对象 */
    QFile file;

    /* 垂直布局 */
    QVBoxLayout *vboxLayout;

    /* 垂直布局 */
    QVBoxLayout *vboxLayout1;

    /* 垂直布局 */
    QVBoxLayout *vboxLayout2;

    /*水平布局*/
    QHBoxLayout *hboxlayout;

    /*水平布局*/
    QHBoxLayout *hboxlayout1;

    /* 网络布局 */
    QGridLayout *gridLayout;

    /* 主布局 */
    QWidget *mainWidget;

    /* 设置功能区域 */
    QWidget *funcWidget;

    /*设置数据接收区*/
    QWidget *readdataWidget;

    /*设置接收功能区*/
    QWidget *readdatafunWidget;

    /*设置发送数据区*/
    QWidget *senddataWidget;

    /*设置发送功能区*/
    QWidget *senddatafunWidget;

    /*定时器定时更新自动更新串口号*/
    QTimer *timer;

    /* 布局初始化 */
    void layoutInit();

    /* 扫描系统可用串口 */
    void scanSerialPort();

    /* 波特率项初始化 */
    void baudRateItemInit();

    /* 数据位项初始化 */
    void dataBitsItemInit();

    /* 检验位项初始化 */
    void parityItemInit();

    /* 停止位项初始化 */
    void stopBitsItemInit();

    /*接收数据格式初始化*/
    void readdataFormatInit();

    /*发送数据格式初始化*/
    void senddataFormatInit();
private slots:
    void sendPushButtonClicked();
    void openSerialPortPushButtonClicked();
    void serialPortReadyRead();
    void slotTextTcpChanged();
    void timerTimeOut();
    bool savedataPushButtonClicked();
    void deleteReaddataPushButtonClicked();
    void deleteSenddataPushButtonClicked();
    void updataSerialPushButtonClicked();

};
#endif // MAINWINDOW_H
