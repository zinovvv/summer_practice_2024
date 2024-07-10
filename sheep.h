#ifndef SHEEP_H
#define SHEEP_H

#include "animal.h"

class Sheep : public Animal
{
    Q_OBJECT
public:
    explicit Sheep(QGraphicsItem *parent = nullptr);
    int getEatenPlantCount();
    void incrementEatenPlantCount();
    void resetEatenPlantCount();
private:
    int plantsEatenCount;
};

#endif // SHEEP_H
