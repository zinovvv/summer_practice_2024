#include "sheep.h"

Sheep::Sheep(QGraphicsItem *parent)
    : Animal(parent)
    , eatenPlantCount(0)
{
    QPixmap pixmap(":/images/sheep.jpg");
    QPixmap scaledPixmap = pixmap.scaled(QSize(70,50));
    setPixmap(scaledPixmap);
}

int Sheep::getEatenPlantCount(){
    return eatenPlantCount;
}

void Sheep::incrementEatenPlantCount(){
    eatenPlantCount++;
}

void Sheep::resetEatenPlantCount(){
    eatenPlantCount=0;
}
