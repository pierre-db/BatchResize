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

void ResizeSettings::magickSetup(const QString& mpath, const QString& margs)
{
    if(!s->contains("magickpath"))
        s->setValue("magickpath", mpath);
    if(!s->contains("args"))
        s->setValue("args", margs);

    magickpath = s->value("magickpath").toString();
    args = s->value("args").toString();

}

ResizeSettings::~ResizeSettings()
{
    //s->sync();
    if(s != nullptr)
        delete s;
}
