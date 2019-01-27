#ifndef MODEL_H
#define MODEL_H

#include <QStandardItemModel>

#define ICON_WAIT_PATH "/wait.png"
#define ICON_RUN_PATH "/run.png"
#define ICON_SUCC_PATH "/success.png"
#define ICON_ERR_PATH "/error.png"

#define FORCE_BG_COLOR QColor(255,0,0, 30)

#define DATA_ID Qt::UserRole + 1
#define DATA_STATUS Qt::UserRole + 2
#define DATA_FORCE Qt::UserRole + 3
#define DATA_OUTPATH Qt::UserRole + 4

enum status_t {status_wait = 0, status_run, status_done, status_error};

class CustomItemModel : public QStandardItemModel
{
    Q_OBJECT
    struct Archive{
        //Archive(i, t, f){itemID = i; text = t; force = f}
        int itemID;
        QString text;
        QVariant force;
        QVariant outputfolder;
    };

public:
    explicit CustomItemModel(QObject *parent = nullptr);
    void initIconsAndTips(const QString& iconspath);
	void reloadIcons(const QString& iconspath);
	
	int itemRowFromID(const int itemID);
    int nextWaitingItemRow();
    bool hasItemsRunning();
	
    void updateItem(const int itemID, const status_t status, const QString& optional = "");
    void addItem(const int itemID, const status_t status, const QString& text, const QString& force = "", const QString& outputfolder = "", const QString& errtype = "");
	void removeItem(const int itemID);
	void clearAll();
	
    int getTotalDone() const { return totalDone; }
    int getLastestItemID() const { return maxItemsID; }

private:
    void addToArchive(const int itemID, const QString& text, const QVariant& force, const QVariant& outputfolder);
    void removeFromArchive(const int index);
    void addItemFromArchive(const int itemID, const status_t status, const QString& optional);

    int maxItemsID;
    int totalDone;
    QList<Archive> archive;

    QList<QIcon> icons;
    QStringList toolTips;
};

#endif // MODEL_H
