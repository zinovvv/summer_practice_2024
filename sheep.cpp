#include "sheep.h"

Sheep::Sheep(QGraphicsItem *parent) : Animal(parent)
{
    QPixmap pixmap(":/images/sheep.jpg");
    QPixmap scaledPixmap = pixmap.scaled(QSize(70,50));
    setPixmap(scaledPixmap);
}
