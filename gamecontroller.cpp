#include "gamecontroller.h"
#include <QRandomGenerator>
#include <QLineF>

GameController::GameController(QGraphicsScene *scene, QObject *parent)
    : QObject(parent), scene(scene) {}

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
    for (auto sheep : sheepList) {
        QGraphicsItem* nearestPlant = sheep->findNearestItem(plantList);

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
                generateSheeps(1, scene->width(), scene->height());
            } else {
                sheep->moveTowards(nearestPlant);
            }
        }
    }
}

void GameController::moveWolfs()
{
    // Проверка наличия элементов в списках
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


            // Проверка наличия ближайшей овцы
            if (nearestSheep) {
                if (wolf->hasReached(nearestSheep)) {
                    // Удаляем овцу из сцены
                    scene->removeItem(nearestSheep);

                    // Преобразуем nearestSheep к типу Sheep*
                    Sheep* sheepToRemove = static_cast<Sheep*>(nearestSheep);

                    // Удаляем овцу из списка и освобождаем память
                    if (sheepList.removeOne(sheepToRemove)) {
                        delete sheepToRemove;
                    }

                    // Сбрасываем показатель голода волка
                    wolf->getHungerBar()->setRect(10, -10, wolf->MAX_HEALTH, 5);
                    totalEatenSheepCount++;

                    // Проверка условия для генерации нового волка
                    if (totalEatenSheepCount >= wolfList.size() * 2) {
                        generateWolfs(1, scene->width(), scene->height());
                        totalEatenSheepCount = 0;
                    }
                } else {
                    wolf->moveTowards(nearestSheep);
                }
            }
        } else {
            // Перемещение волка вверх по вертикали, если он не голоден и находится ниже определенной точки
            if (wolf->y() + wolf->boundingRect().height() > 50) {
                qreal currentY = wolf->y();
                qreal newY = currentY - 1; // Скорость вертикального движения
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
