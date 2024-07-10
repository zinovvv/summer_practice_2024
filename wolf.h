#ifndef WOLF_H
#define WOLF_H

#include "animal.h"

class Wolf : public Animal
{
    Q_OBJECT
public:
    explicit Wolf(QGraphicsItem *parent = nullptr);
    void incrementEatenSheepCount();
    int getEatenSheepCount();
    void resetEatenSheepCount();
private:
    int sheepEatenCount;
};

#endif // WOLF_H
