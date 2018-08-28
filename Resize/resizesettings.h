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
    void magickSetup(const QString& mpath, const QString& margs);

    QString magickpath;
    QString args;
    QString ipaddress;
    quint16 port = DEFAULT_PORT;
private:
    QSettings* s;
};

#endif // RESIZESETTINGS_H
