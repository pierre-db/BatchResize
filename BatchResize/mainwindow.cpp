#include "mainwindow.h"
#include "ui_mainwindow.h"

//Checks if a file exists
/*bool fexists (const std::string& name)
{
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}*/


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->initvars();
    //We read the ini file and set it up by adding any missing data
    settings.magickSetup(pathexec + CMD_PATH, "");

    server = new TcpServer(this);
    server->startServer(settings.ipaddress, settings.port);
    connect(server, SIGNAL(receivedSocket(const QString&)), this, SLOT(launchCmd(const QString&)));

    ui->setupUi(this);
    this->setAcceptDrops(true);
    //setUnifiedTitleAndToolBarOnMac(true);

    model = new QStandardItemModel(this);
    ui->listView->setModel(model);
}

void MainWindow::initvars()
{
    pathexec = QCoreApplication::applicationDirPath();
    pathresize = pathexec + RESIZE_PATH;

    pathlock = QDir::tempPath() + LOCKFILE;

    iconwait = QIcon(pathexec + ICON_WAIT_PATH);
    iconsucc = QIcon(pathexec + ICON_SUCC_PATH);
    iconerr = QIcon(pathexec + ICON_ERR_PATH);
}

void MainWindow::launchCmd(const QString& str)
{
    if(str == "stop")
    {//Stop signal: we stop the server
        std::cout << "Stop signal sent" << std::endl;
        this->close();
    }
    else
    {
        QStringList strSplit = str.split("::");
        QString command = "";
        QString arg = "";
        if(strSplit.length() >= 2)
        {
            command = strSplit[0];
            arg = strSplit[1];
        }
        if(command == "done")
        {//A resize was done, the format is done:rowNb
            int rowNb = arg.toInt()-1;
            if(rowNb >= 0 && rowNb <= model->rowCount())
            {
               model->item(rowNb,0)->setIcon(iconsucc);
               std::cout << "Item " << arg.toStdString() << " done" << std::endl;
            }
            else
            {
               std::cout << "Item unknown done:"<< arg.toStdString() << std::endl;
            }
        }
        else if(command == "error")
        {//An error has occured, the format is error:rowNb
            QString errtype = "0";
            if(strSplit.length() >= 3)
                errtype = strSplit[2];

            int rowNb = arg.toInt()-1;
            if(rowNb >= 0 && rowNb <= model->rowCount())
            {
             model->item(rowNb,0)->setIcon(iconerr);
             std::cout << "Item " << arg.toStdString() << " error type " << errtype.toStdString() << std::endl;
            }
            else
            {
             std::cout << "Item unknown error: " << arg.toStdString() << std::endl;
            }
        }
        else if(command == "run")
        {//One file to resize received
            QString cmd;
            QProcess resize; //new QProcess(this);

            std::cout << "Resizing " << arg.toStdString() << " ..." << std::endl;
            model->appendRow(new QStandardItem(iconwait, arg));

            //We launch the resize commande
            resize.startDetached(pathresize, QStringList() << arg << QString::number(model->rowCount()));
            //connect(resize, SIGNAL(finished()),this, SLOT(processFinished()));
        }
        else
            std::cout << "Unknown command: " << str.toStdString() << std::endl;
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *evt)
{
    if (evt->mimeData()->hasUrls())
        evt->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent* evt)
{
    const QMimeData* mimeData = evt->mimeData();

       // check for our needed mime type, here a file or a list of files
       if (mimeData->hasUrls())
       {
         QStringList pathList;
         QList<QUrl> urlList = mimeData->urls();

         // extract the local paths of the files
         for (int i = 0; i < urlList.size() && i < 32; i++)
         {
             launchCmd("run::" + QDir::toNativeSeparators(urlList.at(i).toLocalFile()));
         }
       }
}

MainWindow::~MainWindow()
{
    delete ui;
    if(model != nullptr)
        delete model;
    if(server != nullptr)
        delete server;

    if (QFileInfo::exists(pathlock))
    {
        if(!QFile::remove(pathlock))
            std::cerr << "Error deleting file" << std::endl;
    }
}

void MainWindow::on_pushButton_clicked()
{
    model->clear();
}
