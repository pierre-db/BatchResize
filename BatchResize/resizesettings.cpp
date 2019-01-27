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

void ResizeSettings::iconsSetup(const QString& ipath)
{
    if(!s->contains("iconspath"))
        s->setValue("iconspath", ipath);

    iconspath = s->value("iconspath").toString().trimmed();
	
	if(!s->contains("iconssize"))
        s->setValue("iconssize", "16");
	
	iconssize = s->value("iconssize").toInt();

    if(iconssize != 16 && iconssize != 24 && iconssize != 32)
	{
        iconssize = 24;
        s->setValue("iconssize", "24");
	}

}

void ResizeSettings::simultaneousSetup(const bool state)
{
    if(!s->contains("simultaneous"))
        s->setValue("simultaneous", state);

    simultaneous = s->value("simultaneous").toBool();
}

void ResizeSettings::toggleSimultaneous()
{
    simultaneous = !simultaneous;
    s->setValue("simultaneous", simultaneous);
}

void ResizeSettings::saveSettings(const QMap<QString, QString>& newsettings)
{
    args = newsettings.value("args");
    s->setValue("args", args);

    magickpath = newsettings.value("magickpath");
    s->setValue("magickpath", magickpath);

    outputfolder = newsettings.value("outputfolder");
    s->setValue("outputfolder", outputfolder);

    outputfolderon = newsettings.value("outputfolderon").toInt();
    s->setValue("outputfolderon", outputfolderon);

    iconspath = newsettings.value("iconspath");
    s->setValue("iconspath", iconspath);

    iconssize = newsettings.value("iconssize").toInt();
    s->setValue("iconssize", iconssize);

    ipaddress = newsettings.value("ipaddress");
    s->setValue("ipaddress", ipaddress);

    port = newsettings.value("port").toInt();
    s->setValue("port", port);
}

ResizeSettings::~ResizeSettings()
{
    //s->sync();
    if(s != nullptr)
        delete s;
}
