#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QWebEngineView>
#include <QWebEngineDownloadItem>
#include <QSystemTrayIcon>   // 记得加这个头文件

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;
       QSystemTrayIcon *trayIcon;   // ← 在这里声明
};


#endif // MAINWINDOW_H
