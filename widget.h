#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTableWidget>
#include "ui_widget.h"

#include "epc.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:

    void on_EA_Add_clicked();

    void on_EA_Sub_clicked();

    void on_save_clicked();

    void on_load_clicked();

    void on_addRow_clicked();

    void update();

    void on_tabWidget_tabBarClicked(int index);

    void on_delRow_clicked();

    void on_moveRight_clicked();

    void on_listImplant_itemClicked(QListWidgetItem *item);

    void on_listImplant_pressed(const QModelIndex &index);

    void on_moveLeft_clicked();

    void on_listEqImps_pressed(const QModelIndex &index);

    void on_listEqImps_itemClicked(QListWidgetItem *item);

    void on_morphTypeSelect_activated(int index);

    void on_comboBox_currentIndexChanged(int index);

    void on_addMorph_clicked();

    void on_delMorph_clicked();

    void on_morphSelect_activated(int index);

private:
    Ui::Widget *ui;

    QString str[10];

    qint32 values[10];

    epc storage;

    void push_epc();

    void pull_epc();

    QString toString(qint32);

    qint32 counter[6];

    void out();

    void in();

    qint32 rowCount[10];

    void addRow();

    /*Selected Table to modify*/
    QTableWidget *selTable;

    QString rowName;

    QTableWidget *tempTable;

    void skillsUpdate();

    void setTable(qint32);

    QTableWidget *egoTable;

    qint32 getLinkedApt(QString);

    void delRow();

    void removeAllRows();

    QListWidgetItem *tempItem;

    info desc;

    qint32 implantIndex;

    qint32 speed();

    qreal durability();

    qreal woundThresh();

    qreal deathRating();

    qreal lucidity();

    qreal traumaThresh();

    qreal insanityRating();

    qreal damageBonus();

    void stats();

    qint32 currentMoxie();

    qint32 moxie();

    QString initiative();

    qint32 indexStatBox;

    void selectSkillTab(qint32);

    void maxApt();

    void calcPoints();

    void morphApt();

    QStringList morphList;

    qint32 updateMorphList(QString);

    void selectedMorph(QString);
};

#endif // WIDGET_H
