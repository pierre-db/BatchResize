#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QtWidgets>
#include "resizesettings.h"

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    SettingsDialog(const ResizeSettings& settings, QWidget *parent = nullptr);
    ~SettingsDialog();
    QString openDir(QLineEdit* lineEdit, QFileDialog::FileMode type);
    //QString formatIpAddress(const QString& ip);
    QString simplifyIpAddress(const QString& ip);

public slots:
    void selectIconsPath();
    void selectMagickPath();
    void selectOutputPath();
    void toggleOutputOn(bool enabled);
    void btnOkPressed();
    void btnCancelPressed();
    void btnApplyPressed();
    //void btnAdvancedPressed();

signals:
    void applyChanges(const QMap<QString, QString>& newsettings);

private:
    const QList<int> sizesOptions = {16, 22, 24, 32, 48};
    QLabel *lblMagickPath;
    QLabel *lblMagickArgs;
    QLabel *lblIconsPath;
    QLabel *lblIconsSize;
    QLabel *lblSizeDisp;
    QLabel *lblOutputPath;
    QLabel *lblIpAddress;
    QLabel *lblPortNb;

    QLineEdit *leMagickPath;
    QLineEdit *leMagickArgs;
    QLineEdit *leIconsPath;
    QLineEdit *leOutputPath;
    QLineEdit *leIpAddress;
    QSpinBox *sbPortNb;
	
    QList<QRadioButton *> radIconsSize;

    QDialogButtonBox *buttonBox;
    QPushButton *btnMagickPath;
    QPushButton *btnIconsPath;
    QPushButton *btnOutputPath;
    QCheckBox *btnOutputOn;
    QCheckBox *btnAdvanced;

    QList<QHBoxLayout *> layoutLines;
    QGridLayout *layoutExtension;
    QGridLayout *mainLayout;

    QWidget *extension;

};

#endif // SETTINGSDIALOG_H
