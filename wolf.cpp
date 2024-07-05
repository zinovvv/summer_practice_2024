#include "wolf.h"

Wolf::Wolf(QGraphicsItem *parent) : QGraphicsPixmapItem(parent) {
    // Здесь можно установить изображение растения
    QPixmap pixmap(":/images/wolf.jpg"); // Пример пути к изображению
    QPixmap scaledPixmap = pixmap.scaled(QSize(70,50));
    setPixmap(scaledPixmap);
    eatenSheepCount = 0;
}

void Wolf::moveTowardsSheep(QGraphicsItem* sheep)
{
    qreal wolfX = x();
    qreal wolfY = y();

    qreal sheepX = sheep->x();
    qreal sheepY = sheep->y();

    qreal deltaX = sheepX - wolfX;
    qreal deltaY = sheepY - wolfY;

    qreal length = std::sqrt(deltaX * deltaX + deltaY * deltaY);
    qreal normX = deltaX / length;
    qreal normY = deltaY / length;

    setPos(wolfX + normX, wolfY + normY);
}

bool Wolf::hasReachedSheep(QGraphicsItem* sheep, qreal threshold)
{
    qreal distance = std::sqrt(std::pow(x() - sheep->x(), 2) + std::pow(y() - sheep->y(), 2));
    return distance < threshold;
}

void Wolf::incrementEatenSheepCount(){
    eatenSheepCount++;
}

int Wolf::getEatenSheepCount(){
    return eatenSheepCount;
}

void Wolf::resetEatenSheepCount(){
    eatenSheepCount = 0;
}
