#include "mainwindow.h"
#include "socket.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    bool serveron = false;
    QString pathlock = QDir::tempPath() + LOCKFILE;

    if(argc > 1)
    {
        //If the lock file already exists: the server is on
        if (QFileInfo::exists(pathlock))
             serveron = true;

        if(serveron)
        {//Server on: we just sent the argv to the server
         // std::cout << "Server on" << std::endl;
            ResizeSettings settings;
            TcpClient client(nullptr);
            QString file = QDir::toNativeSeparators(strcat(argv[1], "\0"));

            //We signal to the main instance that file needs to be processed
            client.doSend(settings.ipaddress, settings.port, "run::" + file);

            return 0;
        }
    }

    if(!serveron)
    {//No server on yet, we launch it
        //We create a lock file to define wether an instance of the program is already running
       QFile lockfile(pathlock);
       if(lockfile.open(QIODevice::WriteOnly))
       {
            std::cout << "Created " << pathlock.toStdString() << std::endl;
            lockfile.close();
       }
       else
           std::cerr << "Failed to create " << pathlock.toStdString() << std::endl;

        //We launch the main application
        QApplication a(argc, argv);
        MainWindow w;
        w.show();

        //We launch the command on the file passed in argument if any
        if(argc > 1)
        {
            QString file = QDir::toNativeSeparators(strcat(argv[1], "\0"));
            w.launchCmd("run::" + file);
        }

        return a.exec();
    }
}
