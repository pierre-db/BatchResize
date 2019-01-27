#ifndef RESIZESETTINGS_H
#define RESIZESETTINGS_H

#include <QSettings>
#include <QHostAddress>

#define DEFAULT_PORT 5002
#define DEFAULT_CMD "magick"

class ResizeSettings
{
public:
    ResizeSettings();
    ~ResizeSettings();
    void magickSetup(const QString& defaultMagickpath, const QString& defaultArgs, const QString& defaultOutputfolder);

    QString ipaddress;
    quint16 port = DEFAULT_PORT;

    QString magickpath;
    QString args;

    bool outputfolderon;
    QString outputfolder;
private:
    QSettings* s;
};

#endif // RESIZESETTINGS_H
