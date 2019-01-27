#include "mainwindow.h"


QString command(const int argc, char *argv[])
{
    QString file;
    QString force = "";
    int filePos = 1;

    if(strcmp(argv[1], "-force") == 0)
    {
        force = "::" + QString(strcat(argv[2], "\0"));
        filePos = 3;
    }
    else if(strcmp(argv[1], "-help") == 0)
    {
        std::cout << "Usage: BatchResize [-force \"arg1 arg2 ...\"] /path/to/file.jpg" << std::endl;
    }

    if(filePos < argc)
    {
        file = QDir::toNativeSeparators(strcat(argv[filePos], "\0"));

        //We signal to the main instance that file needs to be processed
        return "run::" + file + force;
    }
    else
        std::cout << "Error: invalid arguments" << std::endl;

    return "";
}

int main(int argc, char *argv[])
{
    //QUuid uuid = QUuid::createUuid();
    QString pathlock = QDir::temp().absoluteFilePath(LOCKFILE);
    QLockFile lockfile(pathlock);

    //If the lock file already exists: the server is on
    if(lockfile.tryLock(100))
    {//No server on yet, we launch it

        std::cout << "Created " << pathlock.toStdString() << std::endl;

        //We launch the main application
        QApplication a(argc, argv);
        MainWindow w;
        w.show();

        //We launch the command on the file passed in argument if any
        if(argc > 1)
        {
            QString cmd = command(argc, argv);
            if(cmd != "")
                w.launchCmd(cmd);
        }

        return a.exec();
    }
    else
    {//Server on: we just sent the argv to the server
     // std::cout << "Server on" << std::endl;
        if(argc > 1)
        {
            ResizeSettings settings;
            TcpClient client(nullptr);

            QString cmd = command(argc, argv);

            //We signal to the main instance that file needs to be processed
            if(cmd != "")
                client.doSend(settings.ipaddress, settings.port, cmd);

        }
        return 0;
    }
}
