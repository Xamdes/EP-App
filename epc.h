#ifndef EPC_H
#define EPC_H

#include "info.h"

class epc : public QObject
{
    Q_OBJECT
public:
    explicit epc(QObject *parent = 0);

    qint32 data[25];

    qreal dataReal[25];

    QVector<QString> strV;

    QVector<QString> strRows;

    QString character;

    qint32 stats[5];

    qint32 morphIndex;

    qint32 morphSelectIndex;

    QVector<QString> strMorphs;

    void save(QDataStream&) const;

    // Important: this will throw an Exception on error
    void load(QDataStream&);

signals:

public slots:
};
QDataStream &operator<<(QDataStream &ds, const epc &obj);
QDataStream &operator>>(QDataStream &ds, epc &obj);

#endif // EPC_H
