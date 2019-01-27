#ifndef RESIZESETTINGS_H
#define RESIZESETTINGS_H

#include <QSettings>
#include <QHostAddress>

#define DEFAULT_PORT 5002
#define DEFAULT_CMD "magick"

class ResizeSettings
{
public:
    //Shared with resize
    ResizeSettings();
    ~ResizeSettings();
    void magickSetup(const QString& defaultMagickpath, const QString& defaultArgs, const QString& defaultOutputfolder);

    //Specific to the UI
    void iconsSetup(const QString& ipath);
    void simultaneousSetup(const bool state);
    void toggleSimultaneous();
    void saveSettings(const QMap<QString, QString>& newsettings);

    //Settings shared with resize
    QString ipaddress;
    quint16 port = DEFAULT_PORT;

    QString magickpath;
    QString args;
    bool outputfolderon;
    QString outputfolder;

    //Settings specific to the UI
    bool simultaneous;
    QString iconspath;
	int iconssize;

private:
    QSettings* s;
};

#endif // RESIZESETTINGS_H
