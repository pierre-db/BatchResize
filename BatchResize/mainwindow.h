#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define ICON_WAIT_PATH "/icons/wait.png"
#define ICON_SUCC_PATH "/icons/success.png"
#define ICON_ERR_PATH "/icons/error.png"
#define RESIZE_PATH "/resize.exe"
#define CMD_PATH "/magick.exe"
#define LOCKFILE "/batchresize.lock"

#include "socket.h"
#include "resizesettings.h"

#include <QMainWindow>
#include <QtWidgets>

#include <QStandardItemModel>
#include <QObject>
#include <QProcess>
#include <QDir>
#include <QMimeData>


bool fexists (const std::string& name);

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initvars();

protected:
    void dragEnterEvent(QDragEnterEvent *evt);
    void dropEvent(QDropEvent* evt);

public slots:
     void launchCmd(const QString &str);

private slots:
     void on_pushButton_clicked();

private:
    QStandardItemModel *model;
    Ui::MainWindow *ui;

    TcpServer *server;

    QString pathlock;
    QString pathexec;
    QString pathresize;

    QIcon iconsucc;
    QIcon iconwait;
    QIcon iconerr;

    ResizeSettings settings;
};
#endif // MAINWINDOW_H
