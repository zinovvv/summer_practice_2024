#include "wolf.h"

Wolf::Wolf(QGraphicsItem *parent) : Animal(parent)
{
    QPixmap pixmap(":/images/wolf.jpg");
    QPixmap scaledPixmap = pixmap.scaled(QSize(70,50));
    setPixmap(scaledPixmap);
    totalEatenSheepCount = 0;
}

void Wolf::incrementEatenSheepCount()
{
    totalEatenSheepCount++;
}

int Wolf::getEatenSheepCount()
{
    return totalEatenSheepCount;
}

void Wolf::resetEatenSheepCount()
{
    totalEatenSheepCount = 0;
}
