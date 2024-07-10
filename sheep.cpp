#include "sheep.h"

Sheep::Sheep(QGraphicsItem *parent)
    : Animal(parent)
    , plantsEatenCount(0)
{
    QPixmap pixmap(":/images/sheep.jpg");
    QPixmap scaledPixmap = pixmap.scaled(QSize(70, 50));
    setPixmap(scaledPixmap);
}

int Sheep::getEatenPlantCount()
{
    return plantsEatenCount;
}

void Sheep::incrementEatenPlantCount()
{
    plantsEatenCount++;
}

void Sheep::resetEatenPlantCount()
{
    plantsEatenCount = 0;
}
