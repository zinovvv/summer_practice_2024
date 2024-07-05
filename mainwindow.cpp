#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "plant.h"
#include "sheep.h"
#include "wolf.h"
#include <QRandomGenerator>
#include <QPointF>
#include <QTimer>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &MainWindow::moveSheepTowardsPlants);

    plantTimer = new QTimer(this);
    connect(plantTimer, &QTimer::timeout, this, [this]() {
        QRectF viewRect = ui->graphicsView->sceneRect();
        this->generatePlants(1, viewRect.width(), viewRect.height());
    });
    wolfTimer = new QTimer(this);
    connect(wolfTimer, &QTimer::timeout, this, &MainWindow::moveWolfs);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeScene()
{
    QRectF viewRect(0, 0, ui->graphicsView->width()-10, ui->graphicsView->height()-10);
    scene->setSceneRect(viewRect);
    scene->clear();

    int viewWidth = ui->graphicsView->width();
    int viewHeight = ui->graphicsView->height();

    int numWolfs = ui->wolfsspinBox->value();
    generateWolfs(numWolfs,viewWidth,viewHeight);

    int numPlants = ui->plantsspinBox->value();
    generatePlants(numPlants, viewWidth, viewHeight);

    int numSheeps = ui->sheepsspinBox->value();
    generateSheeps(numSheeps, viewWidth, viewHeight);

    gameTimer->start(30);
    plantTimer->start(1500);
    wolfTimer->start(20);
}

void MainWindow::generatePlants(int numPlants, int viewWidth, int viewHeight)
{
    for (int i = 0; i < numPlants; ++i) {
        Plant *plant = new Plant();
        QPointF plantPos = generateRandomPosition(viewWidth-20, viewHeight-20);
        plant->setPos(plantPos);
        scene->addItem(plant);
        plantList.append(plant);
    }
    ui->plantsspinBox->setValue(plantList.size());
}

void MainWindow::generateWolfs(int numWolfs, int viewWidth, int viewHeight)
{
    for (int i = 0; i < numWolfs; ++i) {
        Wolf *wolf = new Wolf();
        QPointF wolfSpawnPos = QPointF(QRandomGenerator::global()->bounded(viewWidth), 0);
        wolf->setPos(wolfSpawnPos);
        scene->addItem(wolf); // Добавляем волка на сцену
        wolf->hide();
        wolfList.append(wolf);

        QGraphicsRectItem* hungerBar = new QGraphicsRectItem(wolf);
        hungerBar->setRect(0, 0, wolf->MAX_HEALTH, 5);
        hungerBar->setBrush(Qt::green);
        wolf->setHungerBar(hungerBar);
    }
    ui->wolfsspinBox->setValue(wolfList.size());
}


void MainWindow::generateSheeps(int numSheeps, int viewWidth, int viewHeight)
{
    for (int i = 0; i < numSheeps; ++i) {
        Sheep *sheep = new Sheep();
        QPointF sheepSpawnPos = QPointF(QRandomGenerator::global()->bounded(viewWidth), QRandomGenerator::global()->bounded(viewHeight));
        sheep->setPos(sheepSpawnPos);
        scene->addItem(sheep);

        QGraphicsRectItem* hungerBar = new QGraphicsRectItem(sheep);
        hungerBar->setRect(10, -5, sheep->MAX_HEALTH, 5);
        hungerBar->setBrush(Qt::green);
        sheep->setHungerBar(hungerBar);

        sheepList.append(sheep);
    }
    ui->sheepsspinBox->setValue(sheepList.size());
}

QPointF MainWindow::generateRandomPosition(int viewWidth, int viewHeight)
{
    qreal x = QRandomGenerator::global()->bounded(viewWidth);
    qreal y = QRandomGenerator::global()->bounded(viewHeight);
    return QPointF(x, y);
}

void MainWindow::on_pushButton_clicked()
{
    ui->plantsspinBox->setDisabled(true);
    ui->sheepsspinBox->setDisabled(true);
    ui->wolfsspinBox->setDisabled(true);
    initializeScene();
}

void MainWindow::moveSheepTowardsPlants()
{
    updateGame();
    for (auto sheep : sheepList) {
        QGraphicsItem* nearestPlant = nullptr;
        qreal minDistance = std::numeric_limits<qreal>::max();

        for (auto plant : plantList) {
            qreal distance = std::sqrt(std::pow(sheep->x() - plant->x(), 2.0) + std::pow(sheep->y() - plant->y(), 2.0));
            if (distance < minDistance) {
                minDistance = distance;
                nearestPlant = plant;
            }
        }

        if (nearestPlant) {
            if (sheep->hasReachedPlant(nearestPlant)) {
                scene->removeItem(nearestPlant);
                plantList.removeOne(nearestPlant);
                delete nearestPlant;
                ui->plantsspinBox->setValue(plantList.size());

                qreal currentWidth = sheep->getHungerBar()->rect().width();
                qreal newWidth = currentWidth + 10;
                if (newWidth > sheep->MAX_HEALTH) {
                    newWidth = sheep->MAX_HEALTH;
                }
                sheep->getHungerBar()->setRect(10, -10, newWidth, 5);
                generateSheeps(1, ui->graphicsView->width(), ui->graphicsView->height()); // Генерация новой овцы
            } else {
                sheep->moveTowardsPlant(nearestPlant);
            }
        }
    }
}

void MainWindow::moveWolfs()
{
    for (auto wolf : wolfList) {
        if (wolf->getHungerBar()->rect().width() <= 25) {
            wolf->show();
            QGraphicsItem* nearestSheep = nullptr;
            qreal minDistance = std::numeric_limits<qreal>::max();

            for (auto sheep : sheepList) {
                qreal distance = std::sqrt(std::pow(wolf->x() - sheep->x(), 2.0) + std::pow(wolf->y() - sheep->y(), 2.0));
                if (distance < minDistance) {
                    minDistance = distance;
                    nearestSheep = sheep;
                }
            }

            if (nearestSheep) {
                if (wolf->hasReachedSheep(nearestSheep)) {
                    scene->removeItem(nearestSheep);
                    sheepList.removeOne(nearestSheep);
                    delete nearestSheep;

                    for (auto wolf : wolfList) {
                        wolf->getHungerBar()->setRect(10, -10, wolf->MAX_HEALTH, 5);
                    }
                    wolf->incrementEatenSheepCount();

                    if (wolf->getEatenSheepCount() >= 5) {
                        int viewWidth = ui->graphicsView->width();
                        generateWolfs(1, viewWidth, ui->graphicsView->height());
                        wolf->resetEatenSheepCount();
                    }


                } else {
                    wolf->moveTowardsSheep(nearestSheep);
                }
            }
        }
        else{
            if (wolf->y() + wolf->boundingRect().height() > 50)
            {
                qreal currentY = wolf->y();
                qreal newY = currentY - 1; // Vertical movement speed
                wolf->setY(newY);
                if (wolf->y() + wolf->boundingRect().height() <= 50)
                    wolf->hide();
            }
        }
    }
}

void MainWindow::updateGame()
{
    for (auto sheep : sheepList) {
        qreal currentWidth = sheep->getHungerBar()->rect().width();
        qreal newWidth = currentWidth - 0.05;

        sheep->getHungerBar()->setRect(10, -10, newWidth, 5);

        if (newWidth <= 0) {
            scene->removeItem(sheep);
            sheepList.removeOne(sheep);
            delete sheep;
            ui->sheepsspinBox->setValue(sheepList.size());
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
            ui->wolfsspinBox->setValue(wolfList.size());
        }
    }
}
