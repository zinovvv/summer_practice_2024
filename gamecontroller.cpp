#include "gamecontroller.h"
#include <QRandomGenerator>
#include <QLineF>

GameController::GameController(QGraphicsScene *scene, QObject *parent)
    : QObject(parent), scene(scene), sheepBirthRate(1), wolfBirthRate(1), totalEatenSheepCount(0) {}

void GameController::setSheepBirthRate(int rate)
{
    sheepBirthRate = rate;
}

void GameController::setWolfBirthRate(int rate)
{
    wolfBirthRate = rate;
}

QPointF GameController::generateRandomPosition(int minX, int maxX, int minY, int maxY)
{
    qreal x = QRandomGenerator::global()->bounded(minX, maxX);
    qreal y = QRandomGenerator::global()->bounded(minY, maxY);
    return QPointF(x, y);
}


void GameController::generatePlants(int numPlants, int viewWidth, int viewHeight)
{
    int minY = viewHeight / 4; // Начало нижней половины
    for (int i = 0; i < numPlants; ++i) {
        Plant *plant = new Plant();
        QPointF plantPos = generateRandomPosition(0, viewWidth - 20, minY, viewHeight - 20);
        plant->setPos(plantPos);
        scene->addItem(plant);
        plantList.append(plant);
    }
}

void GameController::generateSheeps(int numSheeps, int viewWidth, int viewHeight)
{
    int minY = viewHeight / 4; // Начало нижней половины
    for (int i = 0; i < numSheeps; ++i) {
        Sheep *sheep = new Sheep();
        QPointF sheepSpawnPos = generateRandomPosition(0, viewWidth-70, minY, viewHeight-50);
        sheep->setPos(sheepSpawnPos);
        scene->addItem(sheep);

        QGraphicsRectItem* hungerBar = new QGraphicsRectItem(sheep);
        hungerBar->setRect(10, -5, sheep->MAX_HEALTH, 5);
        hungerBar->setBrush(Qt::green);
        sheep->setHungerBar(hungerBar);

        sheepList.append(sheep);
    }
}

void GameController::generateWolfs(int numWolfs, int viewWidth, int viewHeight)
{
    for (int i = 0; i < numWolfs; ++i) {
        Wolf *wolf = new Wolf();
        QPointF wolfSpawnPos = QPointF(QRandomGenerator::global()->bounded(viewWidth), 0);
        wolf->setPos(wolfSpawnPos);
        scene->addItem(wolf);
        wolf->hide();
        wolfList.append(wolf);

        QGraphicsRectItem* hungerBar = new QGraphicsRectItem(wolf);
        hungerBar->setRect(0, 0, wolf->MAX_HEALTH, 5);
        hungerBar->setBrush(Qt::green);
        wolf->setHungerBar(hungerBar);
    }
}

void GameController::moveSheepTowardsPlants()
{
    updateGame();
    if (plantList.isEmpty() || sheepList.isEmpty()) {
        return;
    }

    for (auto sheep : sheepList) {
        if (!sheep) {
            continue;
        }

        QGraphicsItem* nearestPlant = nullptr;

        // Проверка, что plantList не пустой и содержит корректные элементы
        if (sheep&&!plantList.isEmpty() && std::all_of(plantList.begin(), plantList.end(), [](QGraphicsItem* item) { return item != nullptr; })) {
            nearestPlant = sheep->findNearestItem(plantList);
        } else {
            continue;
        }

        if (nearestPlant) {
            if (sheep->hasReached(nearestPlant)) {
                scene->removeItem(nearestPlant);
                plantList.removeOne(static_cast<Plant*>(nearestPlant));
                delete nearestPlant;

                qreal currentWidth = sheep->getHungerBar()->rect().width();
                qreal newWidth = currentWidth + 10;
                if (newWidth > sheep->MAX_HEALTH) {
                    newWidth = sheep->MAX_HEALTH;
                }
                sheep->getHungerBar()->setRect(10, -10, newWidth, 5);
                sheep->incrementEatenPlantCount();
                totalEatenPlantCount++;

                // Проверка на валидность sheepBirthRate и totalEatenPlantCount
                if (sheepBirthRate >= 1 && sheepBirthRate <= 3) {
                    if (sheepBirthRate == 1 && totalEatenPlantCount == 2) {
                        generateSheeps(1, scene->width(), scene->height());
                        sheep->resetEatenPlantCount();
                        totalEatenPlantCount = 0;
                    } else if (sheepBirthRate == 2 && totalEatenPlantCount == 1) {
                        generateSheeps(1, scene->width(), scene->height());
                        sheep->resetEatenPlantCount();
                        totalEatenPlantCount = 0;
                    } else if (sheepBirthRate == 3 && totalEatenPlantCount == 1) {
                        generateSheeps(2, scene->width(), scene->height());
                        sheep->resetEatenPlantCount();
                        totalEatenPlantCount = 0;
                    }
                }
            } else {
                sheep->moveTowards(nearestPlant);
            }
        }
    }
}


void GameController::moveWolfs()
{
    if (wolfList.isEmpty()) {
        return;
    }

    for (auto wolf : wolfList) {
        if (!wolf) {
            continue;
        }

        if (wolf->getHungerBar()->rect().width() <= 25) {
            wolf->show();
            QGraphicsItem* nearestSheep = wolf->findNearestItem(sheepList);

            if (nearestSheep) {
                if (wolf->hasReached(nearestSheep)) {
                    scene->removeItem(nearestSheep);
                    Sheep* sheepToRemove = static_cast<Sheep*>(nearestSheep);

                    if (sheepList.removeOne(sheepToRemove)) {
                        delete sheepToRemove;
                    }

                    wolf->getHungerBar()->setRect(10, -10, wolf->MAX_HEALTH, 5);
                    totalEatenSheepCount++;

                    // Проверка на валидность wolfBirthRate и totalEatenSheepCount
                    if (wolfBirthRate >= 1 && wolfBirthRate <= 3) {
                        if (wolfBirthRate == 1 && totalEatenSheepCount == wolfList.size() * 2) {
                            generateWolfs(1, scene->width(), scene->height());
                            totalEatenSheepCount = 0;
                        } else if (wolfBirthRate == 2 && totalEatenSheepCount == wolfList.size()) {
                            generateWolfs(1, scene->width(), scene->height());
                            totalEatenSheepCount = 0;
                        } else if (wolfBirthRate == 3 && totalEatenSheepCount == wolfList.size()) {
                            generateWolfs(2, scene->width(), scene->height());
                            totalEatenSheepCount = 0;
                        }
                    }
                } else {
                    wolf->moveTowards(nearestSheep);
                }
            }
        } else {
            if (wolf->y() + wolf->boundingRect().height() > 50) {
                qreal currentY = wolf->y();
                qreal newY = currentY - 1;
                wolf->setY(newY);
                if (wolf->y() + wolf->boundingRect().height() <= 50) {
                    wolf->hide();
                }
            }
        }
    }
}





void GameController::updateGame()
{
    for (auto sheep : sheepList) {
        qreal currentWidth = sheep->getHungerBar()->rect().width();
        qreal newWidth = currentWidth - 0.05;

        sheep->getHungerBar()->setRect(10, -10, newWidth, 5);

        if (newWidth <= 0) {
            scene->removeItem(sheep);
            sheepList.removeOne(sheep);
            delete sheep;
        }
    }

    for (auto wolf : wolfList) {
        qreal currentWidth = wolf->getHungerBar()->rect().width();
        qreal newWidth = currentWidth - 0.05;

        wolf->getHungerBar()->setRect(10, 0, newWidth, 5);

        if (newWidth <= 0) {
            scene->removeItem(wolf);
            wolfList.removeOne(wolf);
            delete wolf;
        }
    }
}

QList<Plant*> GameController::getPlantList() const
{
    return plantList;
}

QList<Sheep*> GameController::getSheepList() const
{
    return sheepList;
}

QList<Wolf*> GameController::getWolfList() const
{
    return wolfList;
}

void GameController::resetTotalEatenPlantCount(){
    totalEatenPlantCount=0;
}

void GameController::resetTotalEatenSheepCount(){
    totalEatenSheepCount=0;
}

void GameController::reset(){
    sheepList.clear();
    wolfList.clear();
    plantList.clear();
}


