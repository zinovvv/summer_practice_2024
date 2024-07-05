#include "animal.h"

Animal::Animal(QGraphicsItem *parent) : QObject(), QGraphicsPixmapItem(parent) {}

void Animal::moveTowards(QGraphicsItem* target)
{
    qreal animalX = x();
    qreal animalY = y();

    qreal targetX = target->x();
    qreal targetY = target->y();

    qreal deltaX = targetX - animalX;
    qreal deltaY = targetY - animalY;

    qreal length = std::sqrt(deltaX * deltaX + deltaY * deltaY);
    qreal normX = deltaX / length;
    qreal normY = deltaY / length;

    setPos(animalX + normX, animalY + normY);
}

bool Animal::hasReached(QGraphicsItem* target, qreal threshold)
{
    qreal distance = std::sqrt(std::pow(x() - target->x(), 2) + std::pow(y() - target->y(), 2));
    return distance < threshold;
}
