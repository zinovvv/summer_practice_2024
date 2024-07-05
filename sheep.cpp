#include "sheep.h"
#include <QVector2D>

Sheep::Sheep(QGraphicsItem *parent) : QObject(), QGraphicsPixmapItem(parent)
{
    QPixmap pixmap(":/images/sheep.jpg");
    QPixmap scaledPixmap = pixmap.scaled(QSize(70,50));
    setPixmap(scaledPixmap);
}

void Sheep::moveTowardsPlant(QGraphicsItem* plant)
{
    // Координаты овцы
    qreal sheepX = x();
    qreal sheepY = y();

    // Координаты растения
    qreal plantX = plant->x();
    qreal plantY = plant->y();

    // Вектор направления к растению
    qreal deltaX = plantX - sheepX;
    qreal deltaY = plantY - sheepY;

    // Нормализация вектора
    qreal length = std::sqrt(deltaX * deltaX + deltaY * deltaY);
    qreal normX = deltaX / length;
    qreal normY = deltaY / length;

    // Двигаем овцу в направлении растения
    setPos(sheepX + normX, sheepY + normY);
}

bool Sheep::hasReachedPlant(QGraphicsItem* plant, qreal threshold)
{
    qreal distance = std::sqrt(std::pow(x() - plant->x(), 2) + std::pow(y() - plant->y(), 2));
    return distance < threshold;
}
