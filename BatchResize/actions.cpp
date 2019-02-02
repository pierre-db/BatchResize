#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"

void MainWindow::initActions()
{
    QAction *openFilesAct = new QAction(QIcon(settings.iconspath + ICON_SELECT_PATH), "Open...", this);
    openFilesAct->setStatusTip("Open files");
    openFilesAct->setData(QVariant(ICON_SELECT_PATH));
    connect(openFilesAct, &QAction::triggered, this, &MainWindow::openFiles);

    //const QIcon openFileIcon = QIcon::fromTheme("image-x-generic", QIcon(settings.iconspath + "/openfile.png"));
    QAction *openSelectedItemAct = new QAction(QIcon(settings.iconspath + ICON_OPEN_PATH), "Open File", this);
    //openAct->setShortcuts(QKeySequence::Open);
    openSelectedItemAct->setStatusTip("Open the file");
	openSelectedItemAct->setData(QVariant(ICON_OPEN_PATH));
    connect(openSelectedItemAct, &QAction::triggered, this, &MainWindow::openSelectedItem);
    //fileMenu->addAction(openAct);

    QAction *openSelectedFolderAct = new QAction(QIcon(settings.iconspath + ICON_OPENF_PATH), "Open Folder", this);
    openSelectedFolderAct->setStatusTip("Open the file\'s location");
	openSelectedFolderAct->setData(QVariant(ICON_OPENF_PATH));
    connect(openSelectedFolderAct, &QAction::triggered, this, &MainWindow::openSelectedFolder);

    QAction *rerunSelectedImageAct = new QAction(QIcon(settings.iconspath + ICON_MAGICK_PATH), "Re-run item", this);
    rerunSelectedImageAct->setStatusTip("Re-run Magick on selected item");
	rerunSelectedImageAct->setData(QVariant(ICON_MAGICK_PATH));
    connect(rerunSelectedImageAct, &QAction::triggered, this, &MainWindow::rerunSelectedItem);

    QAction *removeSelectedItemAct = new QAction(QIcon(settings.iconspath + ICON_DEL_PATH), "Remove item", this);
    removeSelectedItemAct->setStatusTip("Remove selected item from this list");
	removeSelectedItemAct->setData(QVariant(ICON_DEL_PATH));
    connect(removeSelectedItemAct, &QAction::triggered, this, &MainWindow::removeSelectedItem);

    QAction *clearListAct = new QAction(QIcon(settings.iconspath + ICON_CLR_PATH), "Clear list", this);
    clearListAct->setStatusTip("Clear the list of items");
	clearListAct->setData(ICON_CLR_PATH);
    connect(clearListAct, &QAction::triggered, this, &MainWindow::clearList);

    QAction *checkInfoAct = new QAction(QIcon(settings.iconspath + ICON_INFO_PATH), "Magick information", this);
    checkInfoAct->setStatusTip("Display Magick\'s information");
	checkInfoAct->setData(ICON_INFO_PATH);
    connect(checkInfoAct, &QAction::triggered, this, &MainWindow::checkInfo);

    QAction *togglePauseAct = new QAction(QIcon(settings.iconspath + ICON_PAUSE_PATH), "Pause / Unpause", this);
    togglePauseAct->setStatusTip("Pause / Unpause magick");
	togglePauseAct->setData(ICON_PAUSE_PATH);
    togglePauseAct->setCheckable(true);
    connect(togglePauseAct, &QAction::triggered, this, &MainWindow::togglePause);

    QAction *toggleSimultaneousModeAct = new QAction(QIcon(settings.iconspath + ICON_SIM_PATH), "Simultaneous mode", this);
    toggleSimultaneousModeAct->setStatusTip("Toggle simultaneous mode");
	toggleSimultaneousModeAct->setData(ICON_SIM_PATH);
    toggleSimultaneousModeAct->setCheckable(true);
    toggleSimultaneousModeAct->setChecked(settings.simultaneous);
    connect(toggleSimultaneousModeAct, &QAction::triggered, this, &MainWindow::toggleSimultaneousMode);

    QAction *openSettingsAct = new QAction(QIcon(settings.iconspath + ICON_SETTINGS_PATH), "Settings...", this);
	openSettingsAct->setData(ICON_SETTINGS_PATH);
    openSettingsAct->setStatusTip("Settings");
    connect(openSettingsAct, &QAction::triggered, this, &MainWindow::openSettings);

    //Actions in toolbar:
    actionsList << openFilesAct << clearListAct << openSelectedItemAct << openSelectedFolderAct << rerunSelectedImageAct \
                 << removeSelectedItemAct << togglePauseAct << toggleSimultaneousModeAct << openSettingsAct << checkInfoAct;
    ui->mainToolBar->addActions(actionsList);
	
	//Actions in the context menu:
    actionsMenuList << openSelectedItemAct << openSelectedFolderAct << rerunSelectedImageAct << removeSelectedItemAct;
    listContextualMenu.addActions(actionsMenuList);
}

void MainWindow::reloadIconsActions(const QString& iconspath)
{
	/*QList<QIcon> listIcons;
	listIcons << ICON_OPEN_PATH << ICON_OPENF_PATH << ICON_MAGICK_PATH << ICON_DEL_PATH \
				<< ICON_CLR_PATH << ICON_INFO_PATH << ICON_PAUSE_PATH << ICON_SIM_PATH << ICON_SETTINGS_PATH;*/
    const int actionsListLen = actionsList.length();
    for(int i = 0; i < actionsListLen; i++)
        actionsList.at(i)->setIcon(QIcon(iconspath + actionsList.at(i)->data().toString()));
}

QString MainWindow::selectedItemFileOut()
{
    QString item = "";

    const QModelIndexList indexes = ui->listView->selectionModel()->selectedIndexes();
    if(indexes.length() < 1)
    {
        QMessageBox msgBox;
        msgBox.setText("No item selected");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }
    else
    {
        QString fileIn = model->itemFromIndex(indexes.at(0))->text();
        QString outputFolder = model->itemFromIndex(indexes.at(0))->data(DATA_OUTPATH).toString();
        if(outputFolder != "")
            item = outputFolder + QDir::separator() + QFileInfo(fileIn).fileName();
        else
            item = fileIn;


        if(!QFileInfo::exists(item))
        {
            QMessageBox msgBox;
            msgBox.setText("This file doesn\'t exist. It may have been moved or renamed.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Warning);
            //msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();

            //We return an empty text to prevent further action
            item = "";
        }
    }

    return item;
}

QModelIndex MainWindow::selectedItemIndex()
{
    const QModelIndexList indexes = ui->listView->selectionModel()->selectedIndexes();
    QModelIndex index;
    if(indexes.length() < 1)
    {
        QMessageBox msgBox;
        msgBox.setText("No item selected");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }
    else
        index = indexes.at(0);

    return index;
}

void MainWindow::openFiles()
{
    static QString path = QDir::homePath();
    QStringList fileNames;

    QFileDialog dialog(this);
    dialog.setNameFilter(OPENDIALOG_FILE_TYPE);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setDirectory(path);

    if (dialog.exec())
    {
        fileNames = dialog.selectedFiles();

        if(fileNames.length() > 0)
            path = QFileInfo(fileNames.at(0)).path();

        for(int i = 0; i < fileNames.length(); i++)
            launchCmd("run::" + fileNames.at(i));
    }
}

void MainWindow::openSelectedItem()
{
    const QString fileOut = selectedItemFileOut();

    if(fileOut != "")
        QDesktopServices::openUrl(QUrl("file:///"+fileOut, QUrl::TolerantMode));
}

void MainWindow::openSelectedFolder()
{
    const QString fileOut = selectedItemFileOut();

    if(fileOut != "")
        QDesktopServices::openUrl(QUrl("file:///"+QFileInfo(fileOut).path(), QUrl::TolerantMode));
}

void MainWindow::rerunSelectedItem()
{
    /*const QString file = selectedItemText();

    if(file != "")
       this->launchCmd("run::" + file);
    */

    const QModelIndex index = selectedItemIndex();

    if(model->itemFromIndex(index) != nullptr)
    {
        const QString file = model->itemFromIndex(index)->text();
        QString force = model->itemFromIndex(index)->data(DATA_FORCE).toString();

        if(force != "")
            force = "::" + force;

        this->launchCmd("run::" + file + force);
    }
}

void MainWindow::removeSelectedItem()
{
    const QModelIndex index = selectedItemIndex();

    if(model->itemFromIndex(index) != nullptr)
    {
        const int itemID = model->itemFromIndex(index)->data(DATA_ID).toInt();
        model->removeItem(itemID);
    }
}

void MainWindow::clearList()
{
    model->clearAll();
}

void MainWindow::checkInfo()
{
    doResizeOnRow(-1, "-version");
}

void MainWindow::toggleSimultaneousMode()
{
    settings.toggleSimultaneous();

    if(!pause)
    {
        //We launch all undone items
        const int nbItems = model->rowCount();
        for(int i = 0; i < nbItems; i++)
        {
            if(model->item(i, 0)->data(DATA_STATUS).toInt() == status_wait)
                doResizeOnRow(i);
        }
    }
}

void MainWindow::togglePause()
{
    pause = !pause;

    if(!pause)
    {
        if(settings.simultaneous)
        {
            //We launch all undone items
            const int nbItems = model->rowCount();
            for(int i = 0; i < nbItems; i++)
            {
                if(model->item(i, 0)->data(DATA_STATUS).toInt() == status_wait)
                    doResizeOnRow(i);
            }
        }
        else
        {
            const int nextItemRow = model->nextWaitingItemRow();
            if(nextItemRow > -1)
                doResizeOnRow(nextItemRow);
        }
    }
}

void MainWindow::openSettings()
{
    SettingsDialog dialog(settings, this);
    connect(&dialog, &SettingsDialog::applyChanges, this, &MainWindow::reloadSettings);
    dialog.exec();
}

void MainWindow::reloadSettings(const QMap<QString, QString>& newsettings)
{

    if(model->hasItemsRunning())
    {// If some items are still running, we don't allow to apply the changes
        QMessageBox msgBox;
        msgBox.setText("Can\'t change the settings while some items are still running.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    //We reload the elements of the GUI affected:
    const int newiconssize = newsettings.value("iconssize").toInt();
    if(settings.iconssize != newiconssize)
        ui->mainToolBar->setIconSize(QSize(newiconssize, newiconssize));

    if(settings.iconspath != newsettings.value("iconspath"))
    {
        reloadIconsActions(newsettings.value("iconspath"));
        model->reloadIcons(newsettings.value("iconspath"));
    }


    //If some changes were done to the server's settings, we reload it
    const quint16 newport = newsettings.value("port").toInt();
    if(settings.ipaddress != newsettings.value("ipaddress") || settings.port != newport)
        server->restartServer(newsettings.value("ipaddress"), newport);
    /*else if(settings.ipaddress != newipaddress)
        server->restartServer(newipaddress, settings.port);
    else if(settings.port != newport)
        server->restartServer(settings.ipaddress, (quint16)newport);*/

    //Settings changes triggered: we need to reload the settings
    settings.saveSettings(newsettings);
}

void MainWindow::serverFailed()
{
    QMessageBox msgBox;
    msgBox.setText("The server failed to start. Please check the IP Address and Port again.");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.exec();
}
