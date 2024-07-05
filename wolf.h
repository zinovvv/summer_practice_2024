#ifndef WOLF_H
#define WOLF_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QPointF>

class Wolf : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    const qreal MAX_HEALTH = 50;
    explicit Wolf(QGraphicsItem *parent = nullptr);
    QGraphicsRectItem* getHungerBar() const { return hungerBar; }
    void setHungerBar(QGraphicsRectItem* bar) { hungerBar = bar; }
    void moveTowardsSheep(QGraphicsItem* sheep);
    bool hasReachedSheep(QGraphicsItem* sheep, qreal threshold = 30.0);
    void incrementEatenSheepCount();
    int getEatenSheepCount();
    void resetEatenSheepCount();
private:
    QGraphicsRectItem* hungerBar;
    int eatenSheepCount;

};

#endif // WOLF_H
