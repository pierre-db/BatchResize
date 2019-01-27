#include "model.h"

//A derivate from QStandardItemModel with a few extra custom methods
CustomItemModel::CustomItemModel(QObject *parent) :
    QStandardItemModel(parent)
{
	maxItemsID = 0;
	totalDone = 0;
}

void CustomItemModel::initIconsAndTips(const QString& iconspath)
{
    icons.insert(status_wait, QIcon(iconspath + ICON_WAIT_PATH));
    icons.insert(status_run, QIcon(iconspath + ICON_RUN_PATH));
    icons.insert(status_done, QIcon(iconspath + ICON_SUCC_PATH));
    icons.insert(status_error, QIcon(iconspath + ICON_ERR_PATH));

    toolTips.insert(status_wait, "Waiting");
    toolTips.insert(status_run, "Running...");
    toolTips.insert(status_done, "Done");
    toolTips.insert(status_error, "Error: ");
}

void CustomItemModel::reloadIcons(const QString& iconspath)
{
    //We clear the icons list
    icons.clear();

    //ANd then we reload all of the actions icons
    icons.insert(status_wait, QIcon(iconspath + ICON_WAIT_PATH));
    icons.insert(status_run, QIcon(iconspath + ICON_RUN_PATH));
    icons.insert(status_done, QIcon(iconspath + ICON_SUCC_PATH));
    icons.insert(status_error, QIcon(iconspath + ICON_ERR_PATH));
	
	//And we re-load the icons for each element of the list
    const int nbItems = this->rowCount()
            ;
	for(int i = 0; i < nbItems; i++)
    {
        int itemStatus = this->item(i, 0)->data(DATA_STATUS).toInt();
        this->item(i, 0)->setIcon(icons.at(itemStatus));
	}

}

int CustomItemModel::nextWaitingItemRow()
{
	const int nbItems = this->rowCount();
	int row = nbItems;
	
    for(int i = 0; i < nbItems; i++)
    {
		int itemStatus = this->item(i, 0)->data(DATA_STATUS).toInt();
        if(itemStatus == status_run) //There is still an item running: no next undone item
			return -1;
		if(itemStatus == status_wait)
			row = qMin(row, i);
    }
	
	//If row still has its initial value it means that no items have a status = status_wait
	if(row == nbItems)
		row = -1;
	
    return row;
}

bool CustomItemModel::hasItemsRunning()
{
    const int nbItems = this->rowCount();

    for(int i = 0; i < nbItems; i++)
    {
        int itemStatus = this->item(i, 0)->data(DATA_STATUS).toInt();
        if(itemStatus == status_run) //There is still an item running: no next undone item
            return true;
    }


    return false;
}

int CustomItemModel::itemRowFromID(const int itemID)
{
    for(int i=0; i < this->rowCount(); i++)
    {
        if(this->item(i,0)->data(DATA_ID).toInt() == itemID)
            return i;
    }

    return -1;
}

void CustomItemModel::updateItem(const int itemID, const status_t status, const QString& optional)
{
	if(itemID > -1 && itemID <= maxItemsID)
	{
		int rowNb = itemRowFromID(itemID);
		if(rowNb > -1)
		{
            //If there is a force parameter
            QString force = this->item(rowNb, 0)->data(DATA_FORCE).toString();
            if(force != "")
                force = " (" + force + ")";

            this->item(rowNb, 0)->setIcon(icons.at(status));
            this->item(rowNb, 0)->setToolTip(toolTips.at(status) + optional + force);
			this->item(rowNb, 0)->setData(QVariant(status), DATA_STATUS);

			
			if(status == status_done || status == status_error)
				totalDone++;
		}
		else //The item is no longer in the list we add at at the end
            addItemFromArchive(itemID, status, optional);
	}
}

void CustomItemModel::addItem(const int itemID, const status_t status, const QString& text, const QString& force, const QString& outputfolder, const QString& errtype)
{	
	//itemID = -1: We increment the items ID to get a new ID for the next item
    int itemIDUsed = itemID;
	if(itemID == -1)
		itemIDUsed = ++maxItemsID;
	
	this->appendRow(new QStandardItem(icons.at(status), text));
    const int rowNb = this->rowCount()-1;

    QString forceStr = "";
    //We store the forced arguments in the Data
    if(force != "")
    {
        this->item(rowNb, 0)->setData(QVariant(force), DATA_FORCE);
        //We set a different background color for forced items
        this->item(rowNb, 0)->setData(QVariant(FORCE_BG_COLOR), Qt::BackgroundColorRole);
        forceStr = " (" + force + ")";
    }

    if(outputfolder != "")
        this->item(rowNb, 0)->setData(QVariant(outputfolder), DATA_OUTPATH);

    this->item(rowNb, 0)->setToolTip(toolTips.at(status) + errtype + forceStr);
	this->item(rowNb, 0)->setData(QVariant(itemIDUsed), DATA_ID);
	this->item(rowNb, 0)->setData(QVariant(status), DATA_STATUS);

    if(status == status_done || status == status_error)
		totalDone++;        
}

void CustomItemModel::addItemFromArchive(const int itemID, const status_t status, const QString& optional)
{
    QString text;
    QString force;
    QString outputfolder;

    //We determine the index from the itemID
    int index = -1;
    for(int i = 0; i < archive.length(); i++)
    {
        if(archive.at(i).itemID == itemID)
        {
            index = i;
            break;
        }
    }

    if(index > -1)
    {// The item is in the archive
        //We copy the  information
        text =  archive.at(index).text;
        force = archive.at(index).force.toString();
        outputfolder = archive.at(index).outputfolder.toString();

        //We delete the item frm the archive
        removeFromArchive(index);
    }
    else
    {//Item not in archive, we create custom values for it
        text = "Item removed number " + QString::number(itemID);
        force = "";
        outputfolder = "";
    }

    addItem(itemID, status, text, force, outputfolder, optional);
}

void CustomItemModel::addToArchive(const int itemID, const QString& text, const QVariant& force, const QVariant& outputfolder)
{
    archive.append(Archive());
    archive.last().itemID = itemID;
    archive.last().text = text;
    archive.last().force = force;
    archive.last().outputfolder = outputfolder;
}

void CustomItemModel::removeFromArchive(const int index)
{
    if(index >= 0 && index < archive.length())
        archive.removeAt(index);
}

void CustomItemModel::removeItem(const int itemID)
{
    if(itemID > -1 && itemID <= maxItemsID)
    {
        const int rowNb = itemRowFromID(itemID);
        if(rowNb > -1)
		{
            const int itemStatus = this->item(rowNb, 0)->data(DATA_STATUS).toInt();

            if(itemStatus == status_run) //We save the running items into the archive
                addToArchive(this->item(rowNb, 0)->data(DATA_ID).toInt(), this->item(rowNb, 0)->text(), \
                             this->item(rowNb, 0)->data(DATA_FORCE), this->item(rowNb, 0)->data(DATA_OUTPATH));

            this->removeRow(rowNb);
			
			if(itemStatus == status_done || itemStatus == status_error)
				totalDone--;
		}

    }
}

void CustomItemModel::clearAll()
{
    const int nbItems = this->rowCount();
    for(int i = 0; i < nbItems; i++)
    {
        int itemStatus = this->item(i, 0)->data(DATA_STATUS).toInt();
        if(itemStatus == status_run) //We save the running items into the archive
            addToArchive(this->item(i, 0)->data(DATA_ID).toInt(), this->item(i, 0)->text(), \
                         this->item(i, 0)->data(DATA_FORCE), this->item(i, 0)->data(DATA_OUTPATH));
    }

    this->clear();
    totalDone = 0;
}
