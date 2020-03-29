#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define ICON_SELECT_PATH "/open.png"
#define ICON_OPEN_PATH "/openfile.png"
#define ICON_OPENF_PATH "/openfolder.png"
#define ICON_MAGICK_PATH "/runmagick.png"
#define ICON_CLR_PATH "/clear.png"
#define ICON_DEL_PATH "/delete.png"
#define ICON_INFO_PATH "/info.png"
#define ICON_SIM_PATH "/simultaneous.png"
#define ICON_PAUSE_PATH "/pause.png"
#define ICON_SETTINGS_PATH "/settings.png"

#define RESIZE_PATH "/resize"
#define CMD_PATH "/magick"

#define LOCKFILE "3e6291c3-671f-488b-9c59-b6774002479e.lockfile"

#define OPENDIALOG_FILE_TYPE "Image (*.jpg *.jpeg *.jpe *.jif *.jfif *.jfi"\
                             " *.jp2 *.j2k *.jpf *.jpx *.jpm *.mj2"\
                             " *.png *.gif *.webp *.heif *.heic"\
                             " *.bmp *dib *.tiff *tif *.psd *.svg)"
#define LIST_ACCEPTED_FORMATS {"jpg", "jpeg", "jpe", "jif", "jfif", "jfi",\
                               "jp2", "j2k", "jpf", "jpx", "jpm", "mj2",\
                               "png", "gif", "webp", "heif", "heic",\
                               "bmp", "dib", "tiff", "tif", "psd", "svg"}
#define LIST_NAME_FILTERS {"*.jpg", "*.jpeg", "*.jpe", "*.jif", "*.jfif", "*.jfi",\
                           "*.jp2", "*.j2k", "*.jpf", "*.jpx", "*.jpm", "*.mj2",\
                           "*.png", "*.gif","*.webp", "*.heif", "*.heic",\
                           "*.bmp", "*.dib", "*.tiff", "*.tif", "*.psd", "*.svg"}

#define DATA_ID Qt::UserRole + 1
#define DATA_STATUS Qt::UserRole + 2

#include "socket.h"
#include "resizesettings.h"
#include "model.h"

#include <QApplication>
#include <QMainWindow>
#include <QtWidgets>

#include <QStandardItemModel>
#include <QObject>
#include <QProcess>
#include <QDir>
#include <QMimeData>

#define BUILD_VERSION "202000329"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initVars();
    void initActions();

    ResizeSettings settings;

protected:
	void doResizeOnRow(const int rowNb, const QString& argument = "");
    void runCmdDir(const QString& argument, const QString& force = "");
	
    void dragEnterEvent(QDragEnterEvent *evt);
    void dropEvent(QDropEvent* evt);

    // Methods associated to QActions:
    QString selectedItemFileOut();
    QModelIndex selectedItemIndex();
    void reloadIconsActions(const QString& iconspath);

public slots:
     void launchCmd(const QString &cmd);

     void serverFailed();

     //Right click action menu:
     void ShowContextMenu(const QPoint& pos);

     // Methods associated to QActions:
     void openFiles();
     void openSelectedItem();
     void openSelectedFolder();
     void rerunSelectedItem();
     void removeSelectedItem();
     void clearList();
     void checkInfo();
     void toggleSimultaneousMode();
     void togglePause();
     void openSettings();
     void reloadSettings(const QMap<QString, QString>& newsettings);

private:
    Ui::MainWindow *ui;
    CustomItemModel *model;
    bool pause;

    TcpServer *server;

    QString pathexec;
    QString pathresize;
    
    QList<QAction *> actionsList;
    QList<QAction *> actionsMenuList;
    QMenu listContextualMenu;
};
#endif // MAINWINDOW_H
