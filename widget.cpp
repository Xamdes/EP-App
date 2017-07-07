#include "widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    QTimer *timer = new QTimer(this);
    /*Connect to update()*/
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(500);
    egoTable = ui->Ego_Apt_Table;
    selTable = ui->tableCombat;
    indexStatBox = 0;
    tempItem = 0;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::update()
{
    maxApt();
    ui->morphTypeSelect->setCurrentIndex(storage.morphIndex);
    skillsUpdate();
    stats();
    calcPoints();
    morphApt();
}

void Widget::on_EA_Add_clicked()
{
    qint32 input = ui->valueSelect->text().toLong();
    switch(indexStatBox)
    {
    case 0 :
        storage.stats[0] = storage.stats[0] + input;
    case 1 :
        storage.stats[1] = storage.stats[1] + input;
        break;
    case 2 :
        storage.stats[2] = storage.stats[2] + input;
        break;
    case 3 :
        storage.stats[3] = storage.stats[3] + input;
        break;
    }

}

void Widget::on_EA_Sub_clicked()
{
    qint32 input = ui->valueSelect->text().toLong();
    switch(indexStatBox)
    {
    case 0 :
        storage.stats[0] = storage.stats[0] - input;
    case 1 :
        storage.stats[1] = storage.stats[1] - input;
        break;
    case 2 :
        storage.stats[2] = storage.stats[2] - input;
        break;
    case 3 :
        storage.stats[3] = storage.stats[3] - input;
        break;
    }
}

QString Widget::toString(qint32 temp)
{
    return QString::number(temp);
}

void Widget::on_save_clicked()
{
    push_epc();
    out();
}

void Widget::on_load_clicked()
{
    in();
    pull_epc();
}

void Widget::on_addRow_clicked()
{
    rowName = ui->addText->text();
    addRow();
}

void Widget::on_delRow_clicked()
{
    rowName = ui->addText->text();
    delRow();
}

void Widget::push_epc()
{
    storage.strRows.resize(0);
    storage.strV.resize(0);

    /*Pushes Ego Table and all Skill Table Vertical Names into storage*/
    for(counter[0] = 1; counter[0] < 11; counter[0]++)
    {
        setTable(counter[0]);
        rowCount[0] = tempTable->rowCount();
        for(counter[1] = 0; counter[1] < rowCount[0]; counter[1]++)
        {
            storage.strRows.push_back(tempTable->verticalHeaderItem(counter[1])->text());
        }
        /*For storing the row count of each table*/
        storage.data[counter[0]] = rowCount[0];
    }

    /*Saves data of each table*/
    for(counter[0] = 0; counter[0] < 11; counter[0]++)
    {
        setTable(counter[0]);
        counter[4] = tempTable->rowCount();
        counter[5] = tempTable->columnCount();
        for(counter[1] = 0; counter[1] < counter[4]; counter[1]++)
        {
            for(counter[2] = 0; counter[2] < counter[5]; counter[2]++)
            {
                storage.strV.push_back(tempTable->item(counter[1],counter[2])->text());
            }
        }
    }
    storage.character = ui->charName->text();
}

void Widget::pull_epc()
{    
    for(counter[0] = 1; counter[0] < 11; counter[0]++)
    {
        QStringList list;
        setTable(counter[0]);
        removeAllRows();
        rowCount[0] = storage.data[counter[0]];
        for(counter[1] = 0; counter[1] <  rowCount[0]; counter[1]++)
        {
            list.append(storage.strRows.first());
            storage.strRows.pop_front();
        }
        for(counter[1] = 0; counter[1] <  rowCount[0]; counter[1]++)
        {
            QTableWidgetItem *item;
            tempTable->insertRow(counter[1]);
            item = new QTableWidgetItem(QString("0"));
            tempTable->setItem(counter[1], 0, item);
            item = new QTableWidgetItem(QString("0"));
            tempTable->setItem(counter[1], 1, item);
            item = new QTableWidgetItem(QString("0"));
            tempTable->setItem(counter[1], 2, item);
            item = new QTableWidgetItem(QString("0"));
            tempTable->setItem(counter[1], 3, item);
            item = new QTableWidgetItem(QString("None"));
            tempTable->setItem(counter[1], 4, item);
            item = new QTableWidgetItem(QString(""));
            tempTable->setItem(counter[1], 5, item);
        }
        tempTable->setVerticalHeaderLabels(list);
    }

    for(counter[0] = 0; counter[0] < 11; counter[0]++)
    {
        setTable(counter[0]);
        counter[4] = tempTable->rowCount();
        counter[5] = tempTable->columnCount();
        for(counter[1] = 0; counter[1] < counter[4]; counter[1]++)
        {
            for(counter[2] = 0; counter[2] < counter[5]; counter[2]++)
            {
                tempTable->item(counter[1],counter[2])->setText(storage.strV.first());
                storage.strV.pop_front();
            }
        }
    }
    ui->charName->setText(storage.character);

    /*Populate MorphList on Load*/
    morphList.clear();
    for(counter[0] = 0; counter[0] < storage.strMorphs.size(); counter[0]++)
    {
        if(storage.strMorphs.at(counter[0]) == "NAME")
        {
            morphList.append(storage.strMorphs.at(counter[0]+1));
        }
    }
    qSort(morphList.begin(),morphList.end());
    ui->morphSelect->clear();
    ui->morphSelect->addItems(morphList);
    ui->morphSelect->setCurrentIndex(0);
    //qDebug() << morphList;
    //qDebug() << storage.strMorphs;
}


void Widget::out()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Character"), "",
                                                    tr("Eclipse Phace Character (*.epc);;All Files (*)"));
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                                     file.errorString());
            return;
        }
        QDataStream out(&file);
        storage.save(out);
    }
}

void Widget::in()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Load Character"), "",
                                                    tr("Eclipse Phace Character (*.epc);;All Files (*)"));
    if (!fileName.isEmpty())
    {
        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                                     file.errorString());
            return;
        }

        QDataStream in(&file);
        storage.load(in);
    }
}

void Widget::addRow()
{
    if(selTable != 0)
    {
        QTableWidgetItem *item;
        QStringList list;
        rowCount[0] = (selTable->rowCount());
        for(counter[0] = 0; counter[0] < rowCount[0]; counter[0]++)
        {
            list.append(selTable->verticalHeaderItem(counter[0])->text());
        }
        list.append(rowName);
        selTable->insertRow(rowCount[0]);
        item = new QTableWidgetItem(QString("0"));
        selTable->setItem(rowCount[0], 0, item);
        item = new QTableWidgetItem(QString("0"));
        selTable->setItem(rowCount[0], 1, item);
        item = new QTableWidgetItem(QString("0"));
        selTable->setItem(rowCount[0], 2, item);
        item = new QTableWidgetItem(QString("0"));
        selTable->setItem(rowCount[0], 3, item);
        item = new QTableWidgetItem(QString("None"));
        selTable->setItem(rowCount[0], 4, item);
        item = new QTableWidgetItem(QString(""));
        selTable->setItem(rowCount[0], 5, item);

        selTable->setVerticalHeaderLabels(list);
    }
}

void Widget::delRow()
{
    if(selTable != 0)
    {
        for(counter[0] = 0; counter[0] < selTable->rowCount(); counter[0]++)
        {
            if(selTable->verticalHeaderItem(counter[0])->text() == rowName)
            {
                selTable->removeRow(counter[0]);
            }
        }
    }
}

void Widget::skillsUpdate()
{
    for(counter[0] = 1; counter[0] < 11; counter[0]++)
    {
        setTable(counter[0]);
        counter[2] = tempTable->rowCount();

        for(counter[1] = 0; counter[1] < counter[2]; counter[1]++)
        {
            QString tempApt = tempTable->item(counter[1],5)->text();
            values[3] = getLinkedApt(tempApt);
            if(values[3] == 7)
            {
                tempTable->item(counter[1],1)->setText("0");
            }
            else
            {
                tempTable->item(counter[1],1)->setText(egoTable->item(values[3],5)->text());
            }

            values[0] = tempTable->item(counter[1],0)->text().toLong();
            if(values[0] > 99)
            {
                values[0] = 99;
                tempTable->item(counter[1],0)->setText("99");
            }
            if(values[0] < 0 || tempTable->item(counter[1],0)->text().isEmpty())
            {
                values[0] = 0;
                tempTable->item(counter[1],0)->setText("0");
            }
            if(values[3] != 7)
            {
                values[5] = egoTable->item(values[3],0)->text().toInt();
            }
            else
            {
                values[5] = 0;
            }
            values[6] = values[0] + values[5];
            if(values[6] > 60)
            {
                //qDebug() << "6: " << values[6];
                values[7] = (values[6] - 60);
                //qDebug() << "7: " << values[7];
                values[0] = values[0] - values[7];

                values[7] = qFloor(values[7]/2);
                values[0] = values[0] + values[7];
            }
            values[1] = tempTable->item(counter[1],1)->text().toDouble();
            values[2] = tempTable->item(counter[1],2)->text().toDouble();
            values[0] = values[0] + values[1] + values[2];
            str[0] = toString(values[0]);
            tempTable->item(counter[1],3)->setText(str[0]);
        }
    }
}

void Widget::on_tabWidget_tabBarClicked(qint32 index)
{
    switch(index)
    {
    case 0 :
        selTable = ui->tableCombat;
        break;
    case 1 :
        selTable = ui->tableExWp;
        break;
    case 2 :
        selTable = ui->tableHardware;
        break;
    case 3 :
        selTable = ui->tableKnow;
        break;
    case 4 :
        selTable = ui->tableMental;
        break;
    case 5 :
        selTable = ui->tablePhysical;
        break;
    case 6 :
        selTable = ui->tablePsi;
        break;
    case 7 :
        selTable = ui->tableSocial;
        break;
    case 8 :
        selTable = ui->tableTechnical;
        break;
    case 9 :
        selTable = ui->tableVehicle;
        break;
    }
}

void Widget::setTable(qint32 index)
{
    switch(index)
    {
    case 0 :
        tempTable = egoTable;
        break;
    case 1 :
        tempTable = ui->tableCombat;
        break;
    case 2 :
        tempTable = ui->tableExWp;
        break;
    case 3 :
        tempTable = ui->tableHardware;
        break;
    case 4 :
        tempTable = ui->tableKnow;
        break;
    case 5 :
        tempTable = ui->tableMental;
        break;
    case 6 :
        tempTable = ui->tablePhysical;
        break;
    case 7 :
        tempTable = ui->tablePsi;
        break;
    case 8 :
        tempTable = ui->tableSocial;
        break;
    case 9 :
        tempTable = ui->tableTechnical;
        break;
    case 10 :
        tempTable = ui->tableVehicle;
        break;
    }
}

qint32 Widget::getLinkedApt(QString apt)
{
    if(apt == "COG")
    {
        return 0;
    }
    else if(apt == "COO")
    {
        return 1;
    }
    else if(apt == "INT")
    {
        return 2;
    }
    else if(apt == "REF")
    {
        return 3;
    }
    else if(apt == "SAV")
    {
        return 4;
    }
    else if(apt == "SOM")
    {
        return 5;
    }
    else if(apt == "WIL")
    {
        return 6;
    }
    else
    {
        return 7;
    }
}

void Widget::removeAllRows()
{
    int rowCo = tempTable->rowCount();
    for(int i = rowCo; i >= 0; i--)
    {
        tempTable->removeRow(i);
    }
}

void Widget::on_moveRight_clicked()
{
    if(tempItem != 0)
    {
        //qDebug() << tempItem->text() << ": Hi";
        ui->listEqImps->insertItem(0,new QListWidgetItem());
        ui->listEqImps->item(0)->setText(tempItem->text());
        ui->listImplant->sortItems(Qt::AscendingOrder);
        ui->listEqImps->sortItems(Qt::AscendingOrder);
    }
}

void Widget::on_moveLeft_clicked()
{
    if(tempItem != 0)
    {
        delete ui->listEqImps->item(implantIndex);
        ui->listImplant->sortItems(Qt::AscendingOrder);
        ui->listEqImps->sortItems(Qt::AscendingOrder);
    }
}

void Widget::on_listImplant_itemClicked(QListWidgetItem *item)
{
    ui->textInfo->clear();

    //qDebug() << item->text();
    qint32 augIndex = desc.augments.indexOf(item->text());
    qint32 offset = augIndex + 1;
    //qDebug() << augIndex;

    if(augIndex != -1)
    {
        tempItem = item;
        ui->textInfo->insertPlainText(desc.augments.at(offset));
    }
    else
    {
        tempItem = 0;
    }
}

void Widget::on_listEqImps_itemClicked(QListWidgetItem *item)
{
    ui->textInfo->clear();

    //qDebug() << item->text();
    qint32 augIndex = desc.augments.indexOf(item->text());
    qint32 offset = augIndex + 1;
    //qDebug() << augIndex;

    if(augIndex != -1)
    {
        tempItem = item;
        ui->textInfo->insertPlainText(desc.augments.at(offset));
    }
    else
    {
        tempItem = 0;
    }
}

void Widget::on_listImplant_pressed(const QModelIndex &index)
{
    implantIndex = index.row();
}

void Widget::on_listEqImps_pressed(const QModelIndex &index)
{
    implantIndex = index.row();
}

qint32 Widget::speed()
{
    return storage.stats[3];
}

qreal Widget::durability()
{
    return storage.stats[2];
}

qreal Widget::woundThresh()
{
    return qCeil(durability() / 5);
}

qreal Widget::deathRating()
{
    if(storage.morphIndex == 0)
    {
        return (durability() * 1.5);
    }
    else
    {
        return (durability() * 2);
    }
}

qreal Widget::lucidity()
{
    qreal temp = egoTable->item(6,5)->text().toDouble();
    return (temp * 2);
}

qreal Widget::traumaThresh()
{
    return qCeil(lucidity() / 5);
}

qreal Widget::insanityRating()
{
    return (lucidity() * 2);
}

qreal Widget::damageBonus()
{
    qreal temp = egoTable->item(5,5)->text().toDouble();
    return qFloor(temp/10);
}

void Widget::stats()
{
    QString statsVar;
    statsVar.append("Damage Bonus: \t\t" + toString(damageBonus()) +  "\t Death Rating: \t\t" + toString(deathRating()) +    "\t Durability: \t" + toString(durability()) + "\n" +
                    "Initiative: \t\t" + initiative() +             "\t Insanity Rating: \t" + toString(insanityRating()) + "\t Lucidity: \t" + toString(lucidity()) + "\n" +
                    "Moxie: \t\t" + toString(moxie()) +        "\t Current Moxie: \t\t" + toString(currentMoxie()) +   "\t Speed: \t" + toString(speed()) + "\n" +
                    "Trauma Threshold: \t" + toString(traumaThresh()) + "\t Wound Threshold: \t" + toString(woundThresh()));
    ui->textStats->setText(statsVar);
}

void Widget::on_morphTypeSelect_activated(int index)
{
    storage.morphIndex = index;
}

QString Widget::initiative()
{
    qreal tp1 = egoTable->item(2,5)->text().toDouble();
    qreal tp2 = egoTable->item(3,5)->text().toDouble();
    qreal tp3 = qFloor((tp1 + tp2)/5);
    return toString(tp3);

}
qint32 Widget::moxie()
{
    return storage.stats[0];
}
qint32 Widget::currentMoxie()
{
    return storage.stats[1];
}

void Widget::on_comboBox_currentIndexChanged(int index)
{
    indexStatBox = index;
}

void Widget::maxApt()
{
    for(counter[0] = 0; counter[0] < 7; counter[0]++)
    {
        values[0] = egoTable->item(counter[0],0)->text().toLong();
        values[1] = egoTable->item(counter[0],1)->text().toLong();
        values[2] = egoTable->item(counter[0],2)->text().toLong();
        values[3] = egoTable->item(counter[0],3)->text().toLong();
        values[4] = egoTable->item(counter[0],4)->text().toLong();
        values[0] = values[0] + values[1] + values[2] + values[3];
        if((values[4] <= values[0]))
        {
            values[0] = values[4];
        }
        str[0] = toString(values[0]);
        egoTable->item(counter[0],5)->setText(str[0]);
    }
}

void Widget::calcPoints()
{
    qint32 points = 0;
    values[2] = 0;

    /*Skills*/
    for(counter[0] = 1; counter[0] < 11; counter[0]++)
    {
        setTable(counter[0]);
        counter[4] = tempTable->rowCount();
        for(counter[1] = 0; counter[1] < counter[4]; counter[1]++)
        {
            values[0] = tempTable->item(counter[1],0)->text().toLong();
            if(counter[0] == 4)
            {
                /*Number of Knowledge Skill Points*/
                values[2] = values[2] + values[0];
            }
            points = points + values[0];
        }

    }
    /*Number of Active Skill Points*/
    values[1] = points - values[2];

    values[0] = 0;
    setTable(0);
    counter[4] = tempTable->rowCount();
    for(counter[1] = 0; counter[1] < counter[4]; counter[1]++)
    {
        values[0] = values[0] + tempTable->item(counter[1],0)->text().toLong();
    }

    points = (points + ((values[0] - 105)*10));

    points = points + (moxie()*15);

    QString str1; //=  "Points Spent:\t |" + toString(points) + "|\n";
    //str1.append("Active Skills:\t |" + toString(values[1]) + "|\n");
    //str1.append("Knowledge Skills: |" + toString(values[2]) + "|\n");
    str1 = QString("%1\t%2\n%3\t%4\n%5\t%6").arg("Points Spent:", toString(points), "Active Skills:", toString(values[1]), "Know Skills:", toString(values[2]));
    ui->pointsText->setText(str1);
}



void Widget::morphApt()
{

    QString str1;
    for(counter[0] = 0; counter[0] < 7; counter[0]++)
    {
        values[0] = ui->morphTable->item(counter[0],0)->text().toLong();
        values[1] = ui->morphTable->item(counter[0],1)->text().toLong();
        values[2] = ui->morphTable->item(counter[0],2)->text().toLong();
        values[0] = values[0] + values[1] + values[2];
        str1 = toString(values[0]);
        ui->morphTable->item(counter[0],3)->setText(str1);
        egoTable->item(counter[0],1)->setText(str1);
    }
    QString name = ui->morphName->text();

    if(morphList.contains(name))
    {
        for(counter[0] = 0; counter[0] < storage.strMorphs.size(); counter[0]++)
        {
            if(storage.strMorphs.at(counter[0]) == name)
            {
                counter[2] = counter[0] + 1;
                for(counter[1] = 0; counter[1] < 7; counter[1]++)
                {
                    storage.strMorphs.replace(counter[2],ui->morphTable->item(counter[1],0)->text());
                    counter[2]++;
                    storage.strMorphs.replace(counter[2],ui->morphTable->item(counter[1],1)->text());
                    counter[2]++;
                    storage.strMorphs.replace(counter[2],ui->morphTable->item(counter[1],2)->text());
                    counter[2]++;
                }
                break;
            }
        }
    }
}


void Widget::on_addMorph_clicked()
{
    QString name = ui->morphName->text();
    QTableWidget *apt = ui->morphTable;

    //qDebug() << morphList;
    if(!morphList.contains(name))
    {
        storage.strMorphs.append("NAME");
        storage.strMorphs.append(name);
        for(counter[0] = 0; counter[0] < 7; counter[0]++)
        {
            storage.strMorphs.append(apt->item(counter[0],0)->text());
            storage.strMorphs.append(apt->item(counter[0],1)->text());
            storage.strMorphs.append(apt->item(counter[0],2)->text());
        }
        storage.strMorphs.append("END");

        morphList.push_back(name);
        values[0] = updateMorphList(name);

        //qDebug() << morphList;
        //qDebug() << storage.strMorphs;

        ui->morphSelect->clear();
        ui->morphSelect->addItems(morphList);
        ui->morphSelect->setCurrentIndex(values[0]);
    }
}

void Widget::on_delMorph_clicked()
{
    //    QString name = ui->morphName->text();
    //    QTableWidget *apt = ui->morphTable;

    //    if(morphList.contains(name))
    //    {

    //        storage.strMorphs.append(name);
    //        for(counter[0] = 0; counter[0] < 7; counter[0]++)
    //        {
    //            storage.strMorphs.append(apt->item(counter[0],0)->text());
    //            storage.strMorphs.append(apt->item(counter[0],1)->text());
    //            storage.strMorphs.append(apt->item(counter[0],2)->text());
    //        }
    //        storage.strMorphs.append("END");

    //        morphList.push_back(name);
    //        values[0] = updateMorphList(name);


    //        qDebug() << morphList;
    //        qDebug() << storage.strMorphs;

    //        ui->morphSelect->clear();
    //        ui->morphSelect->addItems(morphList);
    //        ui->morphSelect->setCurrentIndex(values[0]);
    //    }
}

qint32 Widget::updateMorphList(QString name)
{
    qSort(morphList.begin(),morphList.end());
    for(counter[0] = 0; counter[0] < morphList.size(); counter[0]++)
    {
        if(morphList.at(counter[0]) == name)
        {
            return counter[0];
        }
    }
    return 0;
}

void Widget::selectedMorph(QString name)
{
    for(counter[0] = 0; counter[0] < storage.strMorphs.size(); counter[0]++)
    {
        if(storage.strMorphs.at(counter[0]) == name)
        {
            counter[2] = counter[0] + 1;
            ui->morphName->setText(name);
            QTableWidget *apt = ui->morphTable;
            for(counter[1] = 0; counter[1] < 7; counter[1]++)
            {
                apt->item(counter[1],0)->setText(storage.strMorphs.at(counter[2]));
                counter[2]++;
                apt->item(counter[1],1)->setText(storage.strMorphs.at(counter[2]));
                counter[2]++;
                apt->item(counter[1],2)->setText(storage.strMorphs.at(counter[2]));
                counter[2]++;
            }
        }
    }
}

void Widget::on_morphSelect_activated(int index)
{
    //qDebug() << index;
    selectedMorph(ui->morphSelect->itemText(index));
}
