#ifndef ANIMAL_H
#define ANIMAL_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsRectItem>
#include <QList>
#include <limits>
#include <QLineF>

class Animal : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Animal(QGraphicsItem *parent = nullptr);
    static constexpr qreal MAX_HEALTH = 50;
    QGraphicsRectItem* getHungerBar() const { return hungerBar; }
    void setHungerBar(QGraphicsRectItem* bar) { hungerBar = bar; }
    void moveTowards(QGraphicsItem* target);
    bool hasReached(QGraphicsItem* target, qreal threshold = 30.0);

    template<typename T>
    QGraphicsItem* findNearestItem(const QList<T*>& itemList);

protected:
    QGraphicsRectItem* hungerBar = nullptr;
};

template<typename T>
QGraphicsItem* Animal::findNearestItem(const QList<T*>& itemList)
{
    QGraphicsItem* nearestItem = nullptr;
    qreal minDistance = std::numeric_limits<qreal>::max();
    for (auto item : itemList) {
        qreal distance = QLineF(pos(), item->pos()).length();
        if (distance < minDistance) {
            minDistance = distance;
            nearestItem = item;
        }
    }
    return nearestItem;
}

#endif // ANIMAL_H
