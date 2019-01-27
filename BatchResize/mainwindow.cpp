#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    /*QTime myTimer;
    myTimer.start();*/

    this->initVars();
	
    server = new TcpServer(this);
    connect(server, &TcpServer::receivedSocket, this, &MainWindow::launchCmd);
    connect(server, &TcpServer::serverFailStart, this, &MainWindow::serverFailed);
    server->startServer(settings.ipaddress, settings.port);

    ui->setupUi(this);
    this->setAcceptDrops(true);
    //setUnifiedTitleAndToolBarOnMac(true);
    //We disable the menu on toolbar to prevent hidding it
    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    ui->mainToolBar->setIconSize(QSize(settings.iconssize, settings.iconssize));

    model = new CustomItemModel(this);
    model->initIconsAndTips(settings.iconspath);
    ui->listView->setModel(model);
    //We add the custom Context Menu to the QListView
    ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);
    this->initActions();
    //QtConcurrent::run(initActions, "");

    // Action of the doubleclick on an item
    connect(ui->listView, &QListView::doubleClicked,this, &MainWindow::openSelectedItem);

    // Action of the click right on an item
    connect(ui->listView, &QWidget::customContextMenuRequested,this, &MainWindow::ShowContextMenu);

    //Add icon
    setWindowIcon(QIcon(":/icon.png"));
    /*int nMilliseconds = myTimer.elapsed();
    std::cout << nMilliseconds << std::endl;*/
}

void MainWindow::initVars()
{
    pathexec = QCoreApplication::applicationDirPath();
    pathresize = pathexec + RESIZE_PATH;
	
	//We read the ini file and set it up by adding any missing data
    settings.magickSetup(pathexec + CMD_PATH, "", QDir::homePath());
    settings.iconsSetup(pathexec + "/icons");	    
    settings.simultaneousSetup(false);
    pause = false;
	
}

void MainWindow::launchCmd(const QString& cmd)
{
    if(cmd == "stop")
    {//Stop signal: we stop the server
        std::cout << "Stop signal sent" << std::endl;
        this->close();
    }
    else
    {
        QStringList cmdSplit = cmd.split("::");
        QString command = "";
        QString arg = "";
        if(cmdSplit.length() >= 2)
        {
            command = cmdSplit[0];
            arg = cmdSplit[1];
        }
		
        if(command == "done" || command == "error")
        {//A resize was done, the format is [done|error]::itemID
            QString errtype = "";
            status_t status = status_done;
            if(command == "error")
            {
                status = status_error;

                if(cmdSplit.length() >= 3)
                    errtype = cmdSplit[2];
				else
					errtype = "Unknown";
            }
			
			const int itemID = arg.toInt();
			
			//We update the item in the list
			model->updateItem(itemID, status, errtype);

            ui->statusBar->showMessage("Total done: "+ QString::number(model->getTotalDone()) + " / " + QString::number(model->rowCount()));
			
			if(status == status_error)
				std::cout << "Item " << arg.toStdString() << " error: " << errtype.toStdString() << std::endl;
			else
				std::cout << "Item " << arg.toStdString() << " done" << std::endl;

			if(!settings.simultaneous)
			{//If there are more items on the waiting list, we launch the next one
				const int nextItemRow = model->nextWaitingItemRow();
				if(nextItemRow > -1)
					doResizeOnRow(nextItemRow);
			}

        }
        else if(command == "run")
        {//One file to resize received
            arg = QDir::toNativeSeparators(arg);

            //We define the forced magick arguments if any
            QString force = "";
            if(cmdSplit.length() >= 3)
                force = cmdSplit[2];

            //If we passed a dir as an argument
            if(QFileInfo(arg).isDir())
            {//We list all of the image files and run them
                runCmdDir(arg, force);
                return;
            }

            //We define the output folder if different
            QString outpufolder = "";
            if(settings.outputfolderon)
                outpufolder = QDir::toNativeSeparators(settings.outputfolder);

            //We add a new item to the list, -1 adds the item at the end of the list
            model->addItem(-1, status_wait, arg, force, outpufolder);

            if(settings.simultaneous)//Simultaneous mode, we launch the resize immediatly
                doResizeOnRow(model->itemRowFromID(model->getLastestItemID()));
            else
            {//no simultaenous mode: we only lauch the first item
                if(model->nextWaitingItemRow() == model->rowCount()-1)
                {//No items waiting: we launch the resize commande on the latest item
                    doResizeOnRow(model->nextWaitingItemRow());
                    //connect(resize, SIGNAL(finished()),this, SLOT(processFinished()));
                }
            }
        }
        else if(command == "version")
        {//We want to display Magick's info            
            QMessageBox msgBox;
            const QString info = "Front end: BatchResize (build: " + QString(BUILD_VERSION) + ")\nBuilt with Qt version: " + QString(QT_VERSION_STR) + "\n\n";
            if(arg == "error")
            {
                QString errtype;
				if(cmdSplit.length() >= 3)
                    errtype = cmdSplit[2];
				else
                    errtype = "Unknown";
			
              msgBox.setText(info + errtype +"\nLocation: " + settings.magickpath);
              msgBox.setIcon(QMessageBox::Warning);
            }
            else
            {
              msgBox.setText(info + arg + "\nLocation: " + settings.magickpath);
              msgBox.setIcon(QMessageBox::Information);
            }

            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }
        else
            std::cout << "Unknown command: " << cmd.toStdString() << std::endl;
    }
}

void MainWindow::doResizeOnRow(const int rowNb, const QString& argument)
{
    if(rowNb > -1)
    {//If the row is a valid row
        //We don't do anything in case of pause
        if(pause)
            return;		
		
		//We do the resize based on info retrieved from that row
		QProcess resize;
        //QStandardItem* itemAtRow = model->item(rowNb, 0);
        const int itemID = model->item(rowNb, 0)->data(DATA_ID).toInt();
        model->updateItem(itemID, status_run);

        //We build the list of arguments to pass to resize
        QStringList arguments = {model->item(rowNb, 0)->text(),  QString::number(itemID)};
        if(model->item(rowNb, 0)->data(DATA_FORCE).isValid())
            arguments << model->item(rowNb, 0)->data(DATA_FORCE).toString();

        //We launch resize with the relevant arguments
        resize.startDetached(pathresize, arguments);

    }
	else
	{//Not a valid row, we launch the command based on arguments passed
		QProcess resize;
		resize.startDetached(pathresize, QStringList() << argument << QString::number(rowNb));
	}
}

void MainWindow::runCmdDir(const QString& argument, const QString& force)
{
//We list all of the image files and run them
    QDir directory(argument);
    QStringList allFiles = directory.entryList(LIST_NAME_FILTERS, QDir::Files);
    QStringList allSubDirs = directory.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
    QString forcearg = "";

    if(force != "")
        forcearg = "::" + force;

    //std::cout << allSubDirs.join(", ").toStdString() << std::endl;

    //We add all of the subdirrectories to the list of files
    allFiles << allSubDirs;

    const int allFileLen = allFiles.length();
    for(int i = 0; i < allFileLen; i++)
        launchCmd("run::" + argument + QDir::separator() + allFiles.at(i) + forcearg);
}


void MainWindow::ShowContextMenu(const QPoint& pos)
{
    //We determine the position of the menu
    QPoint globalPos = ui->listView->mapToGlobal(pos);

    //We display the menu if we clicked on an item
    if(ui->listView->indexAt(pos).isValid())
        listContextualMenu.exec(globalPos);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *evt)
{
	const QMimeDatabase db;
	const QStringList acceptedMimeTypes = LIST_ACCEPTED_FORMATS;
	const QMimeData* mimeData = evt->mimeData();
	if (mimeData->hasUrls())
	{
		QList<QUrl> urlList = mimeData->urls();
		const int urlListLen = urlList.length();

		//We check all dragged url
		for (int i = 0; i < urlListLen; i++)
        {//Each url must be a local file and contain an accepted mime type or a  directory
            if(!urlList.at(i).isLocalFile())
                return;
            else
            {
                if(acceptedMimeTypes.indexOf(db.mimeTypeForUrl(urlList.at(i)).preferredSuffix()) == -1
                   && !QFileInfo(urlList.at(i).toLocalFile()).isDir())
                    return;

				//evt->setDropAction(Qt::DropAction::IgnoreAction);
				//evt->ignore();

			}
		}
		evt->acceptProposedAction();
	}
}

void MainWindow::dropEvent(QDropEvent* evt)
{
    const QMimeData* mimeData = evt->mimeData();
	//mimeData.setParrent(this);

       // check for our needed mime type, here a file or a list of files
    if (mimeData->hasUrls())
    {
        //QStringList pathList;
        QList<QUrl> urlList = mimeData->urls();
		const int urlListLen = urlList.length();
        // extract the local paths of the files
        for (int i = 0; i < urlListLen; i++)
        {
			if(urlList.at(i).isLocalFile())
				launchCmd("run::" + urlList.at(i).toLocalFile());
        }
    }
}

MainWindow::~MainWindow()
{
	/*
    qDeleteAll(actionsList);
    actionsList.clear();

    actionsMenuList.clear();
	*/
    if(ui != nullptr)
        delete ui;
	/*
    if(model != nullptr)
        delete model;
	*/
    if(server != nullptr)
        delete server;
}
