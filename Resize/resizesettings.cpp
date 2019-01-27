#include "resizesettings.h"

ResizeSettings::ResizeSettings()
{
    s = new QSettings(QSettings::IniFormat, QSettings::UserScope, "BatchResize", "resize");
    if(!s->contains("ipaddress"))
    {
        ipaddress = QHostAddress(QHostAddress::LocalHost).toString();
        s->setValue("ipaddress", ipaddress);
    }
    else
        ipaddress = s->value("ipaddress").toString().trimmed();
    if(!s->contains("port"))
    {
        port = DEFAULT_PORT;
        s->setValue("port", DEFAULT_PORT);
    }
    else
         port = (quint16)s->value("port").toInt();
}

void ResizeSettings::magickSetup(const QString& defaultMagickpath, const QString& defaultArgs, const QString& defaultOutputfolder)
{
    if(!s->contains("magickpath"))
        s->setValue("magickpath", defaultMagickpath);
    if(!s->contains("args"))
        s->setValue("args", defaultArgs);

    if(!s->contains("outputfolderon"))
        s->setValue("outputfolderon", false);
    if(!s->contains("outputfolder"))
        s->setValue("outputfolder", defaultOutputfolder);

    magickpath = s->value("magickpath").toString().trimmed();
    args = s->value("args").toString().simplified();

    outputfolder = s->value("outputfolder").toString().trimmed();
    outputfolderon = s->value("outputfolderon").toBool();
}

ResizeSettings::~ResizeSettings()
{
    //s->sync();
    if(s != nullptr)
        delete s;
}
