#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QWebEngineProfile>
#include <QWebEngineSettings>



//正常
#include <QWebEngineDownloadItem>
#include <QFileDialog>
#include <QMessageBox>

#include <QSystemTrayIcon>
#include <QIcon>
#include<QScreen>
#include<QCloseEvent>
#include <QDebug>
//-----------------

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //屏幕中间

    // 获取屏幕几何信息并居中
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    move(x, y);


    //-----------------

    // 系统托盘图标
    //trayIcon = new QSystemTrayIcon(this);
    //trayIcon->setIcon(QIcon::fromTheme("chat")); // Linux 下用系统图标主题，Win 下可换成自定义图标
   // trayIcon->show();


    //-----------
    // 构造函数里
    trayIcon = new QSystemTrayIcon(this);
    //trayIcon->setIcon(QIcon(":/icons/app.png")); // 你的图标资源
  //trayIcon->setIcon(QIcon(QPixmap(":/icons/app.png")));
    trayIcon->setIcon(QIcon::fromTheme("message"));  // 尝试不同的内置图标名称
    trayIcon->setToolTip("WhatApps");

    // 托盘菜单
    QMenu *trayMenu = new QMenu(this);

    // 显示/隐藏
    QAction *toggleAction = new QAction("显示/隐藏", this);
    connect(toggleAction, &QAction::triggered, this, [this](){
        if (isMinimized() || !isVisible()) {
            showNormal();
            raise();
            activateWindow();
        } else {
            hide();
        }
    });
    trayMenu->addAction(toggleAction);

    // 退出
    QAction *quitAction = new QAction("退出", this);
    connect(quitAction, &QAction::triggered, this, [this](){
        qApp->quit();
    });
    trayMenu->addAction(quitAction);

    // 设置托盘菜单
    trayIcon->setContextMenu(trayMenu);

    // 显示托盘
    trayIcon->show();

    // 托盘双击也能显示窗口
    connect(trayIcon, &QSystemTrayIcon::activated, this,
            [this](QSystemTrayIcon::ActivationReason reason){
                if (reason == QSystemTrayIcon::Trigger || reason == QSystemTrayIcon::DoubleClick) {
                    if (isMinimized() || !isVisible()) {
                        showNormal();
                        raise();
                        activateWindow();
                    } else {
                        hide();
                    }
                }
            });


    //--------

    //-----------webengine---------

    QWebEngineView *view = new QWebEngineView(this);

    // 设置 User-Agent 伪装成 Chrome
    QString userAgent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/90.0.4430.212 Safari/537.36";
    view->page()->profile()->setHttpUserAgent(userAgent);

    // 启用必要功能
    view->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    view->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);

    // 处理下载事件
    connect(view->page()->profile(), &QWebEngineProfile::downloadRequested,
            this, [this](QWebEngineDownloadItem *download){
                if (!download)
                    return;

                // 弹出保存文件对话框
                QString path = QFileDialog::getSaveFileName(
                    this,
                    tr("保存文件"),
                    download->path(),  // 默认文件名
                    tr("所有文件 (*.*)")
                    );

                if (path.isEmpty()) {
                    download->cancel();
                } else {
                    download->setPath(path);
                    download->accept();  // 开始下载
                    QMessageBox::information(this, tr("下载开始"),
                                             tr("文件正在下载到:\n%1").arg(path));
                }
            });



    // 处理下载事件结束







    // 测试通知：启动时弹一条
    trayIcon->showMessage("WhatsApp", "程序已启动，等待消息", QSystemTrayIcon::Information, 3000);

    // 加载 WhatsApp Web
    view->load(QUrl("https://web.whatsapp.com"));
    setCentralWidget(view);

    // 使用 JS hook 捕获标题变化 (未读消息时 WhatsApp 标题会变化)
    connect(view, &QWebEngineView::titleChanged, this, [this](const QString &title){
        if (title.contains("(")) { // WhatsApp Web 有未读消息时标题会变成 "(1) WhatsApp"
            trayIcon->showMessage("WhatsApp", "你有新消息", QSystemTrayIcon::Information, 5000);
        }
    });




    // 加载 WhatsApp Web
    view->load(QUrl("https://web.whatsapp.com"));
    setCentralWidget(view);
}

MainWindow::~MainWindow()
{
    delete ui;
}


//---------------
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (trayIcon && trayIcon->isVisible()) {
        qDebug() << "Close event intercepted, hiding window.";
        hide();           // 隐藏窗口
        event->ignore();  // 阻止真正退出
    } else {
        event->accept();  // 没托盘时正常退出
    }
}
