// plant.cpp
#include "plant.h"

Plant::Plant(QGraphicsItem *parent) : QGraphicsPixmapItem(parent) {
    // Здесь можно установить изображение растения
    QPixmap pixmap(":/images/plant.png"); // Пример пути к изображению
    QPixmap scaledPixmap = pixmap.scaled(QSize(30,30));
    setPixmap(scaledPixmap);
}
