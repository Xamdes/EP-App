#include "epc.h"

epc::epc(QObject *parent) : QObject(parent)
{
    for(int i = 0; i < 25; i++)
    {
        this->data[i] = 0;
    }
    for(int i = 0; i < 25; i++)
    {
        this->dataReal[i] = 0;
    }
    for(int i = 0; i < 5; i++)
    {
        this->stats[i] = 0;
    }
    this->character = "Name";
    this->strV.resize(0);
    this->strRows.resize(0);
    this->strMorphs.resize(0);
    this->morphIndex = 0;
    this->morphSelectIndex=0;
}

QDataStream &operator<<(QDataStream &ds, const epc &obj)
{
    for(int i=0; i<obj.metaObject()->propertyCount(); ++i)
    {
        if(obj.metaObject()->property(i).isStored(&obj))
        {
            ds << obj.metaObject()->property(i).read(&obj);

        }
    }
    return ds;
}
QDataStream &operator>>(QDataStream &ds, epc &obj) {
    QVariant var;
    for(int i=0; i<obj.metaObject()->propertyCount(); ++i)
    {
        if(obj.metaObject()->property(i).isStored(&obj))
        {
            ds >> var;
            obj.metaObject()->property(i).write(&obj, var);
        }
    }
    return ds;
}

void epc::save(QDataStream& ds) const
{
    int i;
    ds << character << morphIndex << strMorphs << strV << strRows;
    for(i = 0; i < 25; i++)
    {
        ds << data[i];
    }
    for(i = 0; i < 5; i++)
    {
        ds << stats[i];
    }
    ds << morphSelectIndex;
}

void epc::load(QDataStream& ds)
{
    int i;
    ds >> character >> morphIndex >> strMorphs >> strV >> strRows;
    for(i = 0; i < 25; i++)
    {
        ds >> data[i];
    }
    for(i = 0; i < 5; i++)
    {
        ds >> stats[i];
    }
    ds >> morphSelectIndex;
}
