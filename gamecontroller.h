#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QGraphicsScene>
#include "plant.h"
#include "sheep.h"
#include "wolf.h"

class GameController : public QObject
{
    Q_OBJECT
public:
    explicit GameController(QGraphicsScene *scene, QObject *parent = nullptr);
    void spawnPlants(int numPlants, int viewWidth, int viewHeight);
    void spawnSheep(int numSheep, int viewWidth, int viewHeight);
    void spawnWolves(int numWolves, int viewWidth, int viewHeight);
    void moveSheepToPlants();
    void moveWolves();
    void updateAnimalStates();
    QList<Plant*> getPlantList() const;
    QList<Sheep*> getSheepList() const;
    QList<Wolf*> getWolfList() const;
    void setSheepBirthRate(int rate);
    void setWolfBirthRate(int rate);
    void resetTotalEatenPlantCount();
    void resetTotalEatenSheepCount();
    void resetGame();
private:
    int totalSheepEaten;
    int totalPlantsEaten;
    QGraphicsScene *scene;
    QList<Plant*> plantList;
    QList<Sheep*> sheepList;
    QList<Wolf*> wolfList;
    int sheepBirthRate;
    int wolfBirthRate;
    QPointF generateRandomPosition(int minX, int maxX, int minY, int maxY);
};

#endif // GAMECONTROLLER_H
