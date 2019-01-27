#include "mainapplication.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    MainApplication mainapp(a.arguments(), &a);
	
    QObject::connect(&mainapp, &MainApplication::finished, &a, &QCoreApplication::quit);

    // This will launch run from the application event loop.
    QTimer::singleShot(0, &mainapp, &MainApplication::run);
	
    //mainapp.run();

    return a.exec();
}
