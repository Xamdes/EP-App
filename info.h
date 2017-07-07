#ifndef INFO_H
#define INFO_H

#include "incfile.h"

class info : public QObject
{
    Q_OBJECT
public:
    explicit info(QObject *parent = 0);

    QVector<QString> augments;

    QVector<QString> morphs;

signals:

public slots:
};

#endif // INFO_H
