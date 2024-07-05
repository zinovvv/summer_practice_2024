#ifndef SHEEP_H
#define SHEEP_H

#include "animal.h"

class Sheep : public Animal
{
    Q_OBJECT
public:
    explicit Sheep(QGraphicsItem *parent = nullptr);
};

#endif // SHEEP_H
