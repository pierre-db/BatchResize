#include "settingsdialog.h"

SettingsDialog::SettingsDialog(const ResizeSettings& settings, QWidget *parent) : QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
{
    //mainSettings = settings;

    lblMagickArgs = new QLabel("Magick Arguments:");
    leMagickArgs = new QLineEdit();
    lblMagickArgs->setBuddy(leMagickArgs);
    leMagickArgs->setText(settings.args);

    btnOutputOn = new QCheckBox("Output in a different folder");
    btnOutputOn->setChecked(settings.outputfolderon);

    lblOutputPath = new QLabel("Output Folder:");
    leOutputPath = new QLineEdit();
    lblOutputPath->setBuddy(leOutputPath);
    leOutputPath->setText(settings.outputfolder);
    btnOutputPath = new QPushButton("...");
    btnOutputPath->setFixedWidth(32);
    this->toggleOutputOn(settings.outputfolderon);
    connect(btnOutputPath, &QAbstractButton::clicked, this, &SettingsDialog::selectOutputPath);
    connect(btnOutputOn, &QAbstractButton::toggled, this, &SettingsDialog::toggleOutputOn);

    lblMagickPath = new QLabel("Magick Path:");
    leMagickPath = new QLineEdit();
    lblMagickPath->setBuddy(leMagickPath);
    leMagickPath->setText(settings.magickpath);
    btnMagickPath = new QPushButton("...");
    btnMagickPath->setFixedWidth(32);
    connect(btnMagickPath, &QAbstractButton::clicked, this, &SettingsDialog::selectMagickPath);

    lblIconsPath = new QLabel("Icons Path:");
    leIconsPath = new QLineEdit();
    lblIconsPath->setBuddy(leIconsPath);
    leIconsPath->setText(settings.iconspath);
    btnIconsPath = new QPushButton("...");
    btnIconsPath->setFixedWidth(32);
    connect(btnIconsPath, &QAbstractButton::clicked, this, &SettingsDialog::selectIconsPath);
	
    lblIconsSize = new QLabel("Icons Size:");
    const int sizesOptionsLen = sizesOptions.length();
    for(int i = 0;i < sizesOptionsLen; i++)
    {
        radIconsSize.append(new QRadioButton(QString::number(sizesOptions.at(i))+"px"));
        if(sizesOptions.at(i) == settings.iconssize)
            radIconsSize.at(i)->setChecked(true);
    }

    btnAdvanced = new QCheckBox("Show advanced settings");
   // btnAdvanced->setCheckable(true);
   // btnAdvanced->setChecked(false);
   // btnAdvanced->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    //btnAdvanced->setFixedWidth(btnAdvanced->width());

    extension = new QWidget(this);
    extension->setVisible(false);
    connect(btnAdvanced, &QAbstractButton::toggled, extension, &QWidget::setVisible);
    //connect(btnAdvanced, &QAbstractButton::toggled, this, &SettingsDialog::btnAdvancedPressed);

    lblIpAddress = new QLabel("IP Address");
    leIpAddress = new QLineEdit();
    lblIpAddress->setBuddy(leIpAddress);
    leIpAddress->setInputMask("000.000.000.000; ");
    leIpAddress->setText(settings.ipaddress);
    leIpAddress->setToolTip(settings.ipaddress);

    lblPortNb = new QLabel("Port number");
    sbPortNb = new QSpinBox();
    lblPortNb->setBuddy(sbPortNb);
    sbPortNb->setMinimum(0);
    sbPortNb->setMaximum(65535);
    sbPortNb->setValue(settings.port);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Apply);
    //buttonBox->button(QDialogButtonBox::Ok)->setDefault(true);
    connect(buttonBox->button(QDialogButtonBox::Ok), &QAbstractButton::clicked, this, &SettingsDialog::btnOkPressed);
    connect(buttonBox->button(QDialogButtonBox::Cancel), &QAbstractButton::clicked, this, &SettingsDialog::btnCancelPressed);
    connect(buttonBox->button(QDialogButtonBox::Apply), &QAbstractButton::clicked, this, &SettingsDialog::btnApplyPressed);

    layoutLines.append(new QHBoxLayout());
    layoutLines.last()->addWidget(lblMagickArgs);
    layoutLines.last()->addWidget(leMagickArgs);

    layoutLines.append(new QHBoxLayout());
    layoutLines.last()->addWidget(btnOutputOn);

    layoutLines.append(new QHBoxLayout());
    layoutLines.last()->addWidget(lblOutputPath);
    layoutLines.last()->addWidget(leOutputPath);
    layoutLines.last()->addWidget(btnOutputPath);

    layoutLines.append(new QHBoxLayout());
    layoutLines.last()->addWidget(lblMagickPath);
    layoutLines.last()->addWidget(leMagickPath);
    layoutLines.last()->addWidget(btnMagickPath);

    layoutLines.append(new QHBoxLayout());
    layoutLines.last()->addWidget(lblIconsPath);
    layoutLines.last()->addWidget(leIconsPath);
    layoutLines.last()->addWidget(btnIconsPath);
	
    layoutLines.append(new QHBoxLayout());
    layoutLines.last()->addWidget(lblIconsSize);
    for(int i = 0;i < sizesOptionsLen; i++)
        layoutLines.last()->addWidget(radIconsSize.at(i));

    layoutLines.append(new QHBoxLayout());
    layoutLines.last()->addWidget(btnAdvanced);
    //const int btnAdvancedPos = layoutLines.length() - 1;

    layoutExtension = new QGridLayout();
    layoutExtension->addWidget(lblIpAddress, 0, 0);
    layoutExtension->addWidget(leIpAddress, 0, 1);
    layoutExtension->addWidget(lblPortNb, 1, 0);
    layoutExtension->addWidget(sbPortNb, 1, 1);

   //extension->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
   //layoutExtension->setSizeConstraint(QLayout::SetMinimumSize);

    extension->setLayout(layoutExtension);

    layoutLines.append(new QHBoxLayout());
    layoutLines.last()->addWidget(extension);
    //layoutLines.last()->setSizeConstraint(QLayout::SetFixedSize);


    mainLayout = new QGridLayout();
    mainLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
	const int layoutLinesLength = layoutLines.length();
	for(int i = 0; i < layoutLinesLength; i++)
		mainLayout->addLayout(layoutLines.at(i), i, 0);

	//We add it at the end
    mainLayout->addWidget(buttonBox, layoutLinesLength, 0, Qt::AlignBottom);
   // mainLayout->setRowStretch(layoutLinesLength-2, 1);

   setLayout(mainLayout);
   resize(500, this->height());
   setWindowTitle("Settings");
}

/*QString SettingsDialog::formatIpAddress(const QString& ip)
{
    QStringList number = ip.split(".");
    const int numberLen = number.length();
    for(int i = 0; i < numberLen;i++)
        number[i] = QString::number(number.at(i).toInt()).rightJustified(3, '0');

    return number.join(".");
}*/
QString SettingsDialog::simplifyIpAddress(const QString& ip)
{
    QStringList number = ip.split(".");
    const int numberLen = number.length();
    for(int i = 0; i < numberLen;i++)
        number[i] = QString::number(number.at(i).toInt());

    return number.join(".");
}

/*void SettingsDialog::btnAdvancedPressed()
{

    extension->setVisible(!extension->isVisible());
    this->resize(500, this->height() - extension->height());
}*/

QString SettingsDialog::openDir(QLineEdit* lineEdit, QFileDialog::FileMode type)
{
    QString path = lineEdit->text().trimmed();
    QStringList selectedFiles;

    QFileDialog dialog(this);
    dialog.setFileMode(type);

    QString navpath = QFileInfo(path).path();
    if(!QFileInfo(navpath).exists())
    {
        QMessageBox msgBox;
        msgBox.setText(navpath + ":\nThis path doesn\'t exist. It may have been moved or renamed.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();

        navpath = QDir::homePath();
    }
    dialog.setDirectory(navpath);

    if(type == QFileDialog::ExistingFile)
		dialog.setNameFilter("Programs (*.exe)");
    else
		dialog.setOption(QFileDialog::ShowDirsOnly);

    if (dialog.exec())
    {
        selectedFiles = dialog.selectedFiles();

        if(selectedFiles.length() > 0)
        {
            QString newpath = selectedFiles.at(0);
            if(QFileInfo(newpath).exists())
                path = newpath;
            else
            {
                    QMessageBox msgBox;
                    msgBox.setText("This path doesn\'t exist. It may have been moved or renamed.");
                    msgBox.setStandardButtons(QMessageBox::Ok);
                    msgBox.setIcon(QMessageBox::Warning);
                    msgBox.exec();
            }
        }
    }

    return path;
}

void SettingsDialog::selectIconsPath()
{
    leIconsPath->setText(openDir(leIconsPath, QFileDialog::Directory));
}

void SettingsDialog::selectMagickPath()
{
    leMagickPath->setText(openDir(leMagickPath, QFileDialog::ExistingFile));
}

void SettingsDialog::selectOutputPath()
{
    leOutputPath->setText(openDir(leOutputPath, QFileDialog::Directory));
}

void SettingsDialog::toggleOutputOn(bool enabled)
{
    leOutputPath->setDisabled(!enabled);
    btnOutputPath->setDisabled(!enabled);
}

void SettingsDialog::btnApplyPressed()
{
    QMap <QString, QString> newsettings;
    newsettings.insert("args", leMagickArgs->text().simplified());
    newsettings.insert("magickpath", leMagickPath->text().trimmed());
    newsettings.insert("outputfolder", leOutputPath->text().trimmed());
    newsettings.insert("outputfolderon",  QString::number(btnOutputOn->isChecked()));
    newsettings.insert("iconspath", leIconsPath->text().trimmed());
    int newiconssize = 0;
    for(int i = 0; i < sizesOptions.length(); i++)
    {
        if(radIconsSize.at(i)->isChecked())
            newiconssize = sizesOptions.at(i);
    }
    newsettings.insert("iconssize", QString::number(newiconssize));

    newsettings.insert("ipaddress", simplifyIpAddress(leIpAddress->text().trimmed()));
    QHostAddress address;//Check if the new IP is valid
    if(address.setAddress(newsettings.value("ipaddress"))
      && address.protocol() == QAbstractSocket::IPv4Protocol)
    {
            //newipaddress = address.toString();
            //address.setAddress(newipaddress);
            leIpAddress->setText(newsettings.value("ipaddress"));
            leIpAddress->setToolTip(newsettings.value("ipaddress"));
    }
    else
    {//Not a vlid IP, we get back the previous input from the tool tip
        newsettings.insert("ipaddress", leIpAddress->toolTip());
        leIpAddress->setText(newsettings.value("ipaddress"));
    }

    quint16 newport = (quint16)sbPortNb->text().toInt();
    //sbPortNb->setText(QString::number(newport));
    sbPortNb->setValue(newport);
    newsettings.insert("port", QString::number(newport));

    /*if(!QFileInfo(newmagickpath).exists())
    {
        QMessageBox msgBox;
        msgBox.setText("The new Magick path doesn\'t exist. It may have been moved or renamed.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();

        newmagickpath = mainSettings->magickpath;
    }

    if(!QFileInfo(newiconspath).exists())
    {
        QMessageBox msgBox;
        msgBox.setText("The new icons path doesn\'t exist. It may have been moved or renamed.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();

        newiconspath = mainSettings->iconspath;
    }*/
	
    //emit applyChanges(newargs, newmagickpath, newoutputfolder, newoutputfolderon, newiconspath, newiconssize, newipaddress, newport);
    emit applyChanges(newsettings);
}
void SettingsDialog::btnCancelPressed()
{
    this->close();
}
void SettingsDialog::btnOkPressed()
{
    btnApplyPressed();
    btnCancelPressed();
}

SettingsDialog::~SettingsDialog()
{
    /*
    if(lblMagickPath != nullptr)
        delete lblMagickPath;
    if(lblMagickArgs != nullptr)
        delete lblMagickArgs;
    if(lblIconsPath != nullptr)
        delete lblIconsPath;

    if(leMagickPath != nullptr)
        delete leMagickPath;
    if(leMagickArgs != nullptr)
        delete leMagickArgs;
    if(leIconsPath != nullptr)
        delete leIconsPath;

    if(btnMagickPath != nullptr)
        delete btnMagickPath;
    if(btnIconsPath != nullptr)
        delete btnIconsPath;
    if(buttonBox != nullptr)
        delete buttonBox;
	
	qDeleteAll(layoutLines);
	layoutLines.clear();
    if(line1Layout != nullptr)
        delete line1Layout;
    if(line2Layout != nullptr)
        delete line2Layout;
    if(line3Layout != nullptr)
        delete line3Layout;
    if(mainLayout != nullptr)
        delete mainLayout;
    */
}
