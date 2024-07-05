#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QGraphicsScene>
#include <QList>
#include "plant.h"
#include "sheep.h"
#include "wolf.h"

class GameController : public QObject
{
    Q_OBJECT
public:
    explicit GameController(QGraphicsScene *scene, QObject *parent = nullptr);
    void generatePlants(int numPlants, int viewWidth, int viewHeight);
    void generateSheeps(int numSheeps, int viewWidth, int viewHeight);
    void generateWolfs(int numWolfs, int viewWidth, int viewHeight);
    void moveSheepTowardsPlants();
    void moveWolfs();
    void updateGame();
    QList<Plant*> getPlantList() const; // Метод доступа к списку растений
    QList<Sheep*> getSheepList() const; // Метод доступа к списку овец
    QList<Wolf*> getWolfList() const;   // Метод доступа к списку волков

private:
    int totalEatenSheepCount = 0;
    QGraphicsScene *scene;
    QList<Plant*> plantList;
    QList<Sheep*> sheepList;
    QList<Wolf*> wolfList;

    QPointF generateRandomPosition(int viewWidth, int viewHeight);
};

#endif // GAMECONTROLLER_H