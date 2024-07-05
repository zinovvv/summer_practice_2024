#ifndef SHEEP_H
#define SHEEP_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QPointF>

class Sheep : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Sheep(QGraphicsItem *parent = nullptr);
    const qreal MAX_HEALTH = 50;
    void moveTowardsPlant(QGraphicsItem* plant);
    bool hasReachedPlant(QGraphicsItem* plant, qreal threshold = 30.0);
    QGraphicsRectItem* getHungerBar() const { return hungerBar; }
    void setHungerBar(QGraphicsRectItem* bar) { hungerBar = bar; }

private:
    QGraphicsRectItem* hungerBar;
};

#endif // SHEEP_H
