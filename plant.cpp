#include "plant.h"

Plant::Plant(QGraphicsItem *parent) : QGraphicsPixmapItem(parent) {
    QPixmap pixmap(":/images/plant.png");
    QPixmap scaledPixmap = pixmap.scaled(QSize(30, 30));
    setPixmap(scaledPixmap);
}
