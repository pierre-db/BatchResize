#include "mainapplication.h"

MainApplication::MainApplication(const QStringList& arguments, QObject *parent) : QObject(parent)
{
    argc = arguments.length();
	//No need to initialize variables if there are no arguments
    if(argc <= 1)
		return;
	
	if(argc > 2)
        itemID = arguments.at(2);
    else
        itemID = "-1";

    if(argc > 3)
        force = arguments.at(3);
    else
        force = "";
	
    pathexec = QCoreApplication::applicationDirPath();
    settings.magickSetup(DEFAULT_CMD, "", QDir::homePath());
	
	//The file to execute the cmd on is passed in argument
    fileIn = arguments.at(1);
    if(fileIn == "-version")
	{
		checkVersion = true;
        listargs << fileIn;
	}
	else
	{
		checkVersion = false;

        //If the outputfolder is activated, we specify the output folder
        if(settings.outputfolderon)
            fileOut = settings.outputfolder + "/" + QFileInfo(fileIn).fileName();
        else
            fileOut = fileIn;

        if(force == "")
            listargs << fileIn << QString(settings.args).split(" ") << fileOut;
        else
        {
            //We pass the argument force to magick
            listargs << fileIn << force.split(" ") << fileOut;
        }
	}

}

void MainApplication::sendFeedback(const QString &feedback)
{
     TcpClient client;
     //connect(client, &TcpClient::finished, this, &MainApplication::kill);
	
    //We signal to the main instance that file needs to be processed
    client.doSend(settings.ipaddress, settings.port, feedback);
    emit finished();
}

/*void MainApplication::kill()
{
    emit finished();
}*/

void MainApplication::run()
{
	if(argc < 1)
	{
        std::cerr << "Invalid arguments: no files to process" << std::endl;
        emit finished();
        return;
    }
	
	if(!checkVersion)
    {//We want to resize a file
        //We retreive the argument list to be passed to magick
        if(settings.args == "")
        {
            std::cout << "Empty arguments!" << std::endl;
            sendFeedback("error::"+itemID+"::Empty argument");
            return;
        }
		
		//We check if the file exists
        if(!QFileInfo::exists(fileIn))
		{//Check if the file exists
            std::cout << "File does not exist: " << fileIn.toStdString() << " ..." << std::endl;
            sendFeedback("error::"+itemID+"::File does not exist");
            return;
		}

        //We check if the output folder exists if not we create it
        if(settings.outputfolderon)
        {
            if(!QFileInfo::exists(settings.outputfolder))
            {
                if(!QDir().mkpath(settings.outputfolder))
                {//if the output folder creation fails
                    std::cout << "Impossible to create the folder: " << settings.outputfolder.toStdString() << " ..." << std::endl;
                    sendFeedback("error::"+itemID+"::Impossible to create output folder");
                    return;
                }
            }
        }

		const QMimeDatabase db;
		const QStringList acceptedFormats = LIST_ACCEPTED_FORMATS;
        const QMimeType mime = db.mimeTypeForFile(fileIn, QMimeDatabase::MatchDefault);
		//We check that the file is in one of the accepted formats
		if(acceptedFormats.indexOf(mime.preferredSuffix()) == -1)//(!mime.inherits("image/jpeg"))
		{
            std::cout << "Wrong file format: " << fileIn.toStdString() << " ..." << std::endl;
            sendFeedback("error::"+itemID+"::Wrong file format");
            return;
		}
	}

	//We launch magick with the required parameters
	QProcess magick;
    magick.start(settings.magickpath, listargs);

	if(magick.waitForStarted())
        std::cout << "Resizing: " << fileIn.toStdString() << " ..." << std::endl;
	else
	{// The process isn't starting
		std::cerr << "Error: cannot start " << settings.magickpath.toStdString() << std::endl;
		if(checkVersion)
        {
            sendFeedback("version::error::Can\'t launch Magick. Check the location again.");
            return;
        }
		else
        {
            sendFeedback("error::"+itemID+"::Can\'t start magick");
            return;
        }
	}

	if(magick.waitForFinished())
	{// Process ended
		std::cout << "Done!" << std::endl;
		if(checkVersion)
        {
            sendFeedback("version::" + QString(magick.readAllStandardOutput()));
            return;
        }
		else
        {
            sendFeedback("done::"+itemID);
            return;
        }
	}
	else
	{// The process isn't ending
		std::cerr << "Error: cannot end " << settings.magickpath.toStdString() << std::endl;
        if(checkVersion)
        {
            sendFeedback("version::error:::Magick did\'t finish correctly.");
            return;
        }
        else
        {
            sendFeedback("error::"+itemID+"::Process magick cannot end");
            return;
        }
	}
}

MainApplication::~MainApplication()
{
    /*if(client != nullptr)
        delete client;*/
}
