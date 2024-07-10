#include "wolf.h"

Wolf::Wolf(QGraphicsItem *parent) : Animal(parent), sheepEatenCount(0)
{
    QPixmap pixmap(":/images/wolf.jpg");
    QPixmap scaledPixmap = pixmap.scaled(QSize(70, 50));
    setPixmap(scaledPixmap);
}

void Wolf::incrementEatenSheepCount()
{
    sheepEatenCount++;
}

int Wolf::getEatenSheepCount()
{
    return sheepEatenCount;
}

void Wolf::resetEatenSheepCount()
{
    sheepEatenCount = 0;
}
