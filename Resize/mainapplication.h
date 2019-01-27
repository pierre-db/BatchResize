#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QMimeDatabase>

#include <QTimer>

#include <QCoreApplication>
#include <QSettings>

#include <QProcess>

#include "socket.h"
#include "resizesettings.h"

#define LIST_ACCEPTED_FORMATS {"jpg", "jpeg", "png", "gif", "bmp", "tiff"}

class MainApplication : public QObject
{
	 Q_OBJECT
public:
    MainApplication(const QStringList& arguments, QObject *parent = nullptr);
    ~MainApplication();
	
	int argc;
	QString itemID;
	
	QString pathexec;
	QStringList listargs;
    QString fileIn;
    QString fileOut;
    QString force;

	ResizeSettings settings;	
	bool checkVersion;
	
public slots:
	void run();
    //void kill();

signals:
    void finished();
	
private:
    void sendFeedback(const QString &feedback);
};


#endif // MAINAPPLICATION_H
