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
    void setSheepBirthRate(int rate);
    void setWolfBirthRate(int rate);
    void resetTotalEatenPlantCount();
    void resetTotalEatenSheepCount();
    void reset();
private:
    int totalEatenSheepCount = 0;
    int totalEatenPlantCount = 0;
    QGraphicsScene *scene;
    QList<Plant*> plantList;
    QList<Sheep*> sheepList;
    QList<Wolf*> wolfList;
    int sheepBirthRate;
    int wolfBirthRate;
    QPointF generateRandomPosition(int minX, int maxX, int minY, int maxY);
};

#endif // GAMECONTROLLER_H
