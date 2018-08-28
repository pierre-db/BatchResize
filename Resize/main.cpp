#include <QFileInfo>
#include <QFile>
#include <QMimeDatabase>

#include <QApplication>
#include <QSettings>

#include <QProcess>

#include "socket.h"
#include "resizesettings.h"

#define DEFAULT_ARG "-strip -interlace JPEG -quality 90"

int sendfeedback(const ResizeSettings &settings, const QString &feedback)
{
    TcpClient client;

    //We signal to the main instance that file needs to be processed
    client.doSend(settings.ipaddress, settings.port, feedback);

    //client.doSend("127.0.0.1", 5002, feedback);

    return 0;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(argc > 1)
    {
        QString idNb;
        if(argc > 2)
            idNb = strcat(argv[2], "\0");
        else
            idNb = "-1";

        QString extension;
        QString feedback;

        QProcess cmd;
        QString pathexec = QCoreApplication::applicationDirPath();
        ResizeSettings settings;
        settings.magickSetup(DEFAULT_CMD, "");

        //We define the command's execution path
        QString pathcmd = settings.magickpath;
        if(!QFileInfo::exists(pathcmd))
            pathcmd = DEFAULT_CMD;

        //We retreive the argument list to be passed to the cmd
        QString args = settings.args;
        if(args == "")
        {
            std::cout << "Empty arguments!" << std::endl;
            return sendfeedback(settings, "error::"+idNb+"::1");
        }

        QStringList listargs = QString(args).split(" ");

        //The file to execute the cmd on is passed in argument
        QString file = strcat(argv[1], "\0");
        //file = file.remove(":");// QRegExp( "\\|\\*\\<\\>:\\?\"" )

        //Check if the file exists
        if(!QFileInfo::exists(file))
        {
            std::cout << "File does not exist: " << file.toStdString() << " ..." << std::endl;
            return sendfeedback(settings, "error::"+idNb+"::2");
        }

        //Check the file mime type
        QMimeDatabase db;
        QMimeType mime = db.mimeTypeForFile(file, QMimeDatabase::MatchDefault);
        if (!mime.inherits("image/jpeg"))
        {
            std::cout << "Wrong file format: " << file.toStdString() << " ..." << std::endl;
            return sendfeedback(settings, "error::"+idNb+"::3");
        }

        //We launch the resize commande
        cmd.start(pathcmd, QStringList() << file << listargs << file );
        if (cmd.waitForStarted())
        {
            std::cout << "Resizing " << file.toStdString() << " ..." << std::endl;
        }
        else
        {// The process isn't starting
            std::cerr << "Error: cannot start " << pathcmd.toStdString() << std::endl;
            return sendfeedback(settings, "error::"+idNb+"::4");
        }

        if(cmd.waitForFinished())
        {// Process ended
            //QByteArray result = cmd.readAll();
            std::cout << "Done!" << std::endl;
            return sendfeedback(settings, "done::"+idNb);
        }
        else
        {// The process isn't ending
            std::cerr << "Error: cannot end " << pathcmd.toStdString() << std::endl;
            return sendfeedback(settings, "error::"+idNb+"::5");
        }

    }
    else
        std::cerr << "Invalid arguments" << std::endl;

    return a.exec();
}
