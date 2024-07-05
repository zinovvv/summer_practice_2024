// Ваш MainWindow.cpp

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "gamecontroller.h"
#include <QRandomGenerator>
#include <QPointF>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    gameController = new GameController(scene, this);

    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, gameController, &GameController::moveSheepTowardsPlants);

    plantTimer = new QTimer(this);
    connect(plantTimer, &QTimer::timeout, this, [this]() {
        QRectF viewRect = ui->graphicsView->sceneRect();
        gameController->generatePlants(1, viewRect.width(), viewRect.height());
    });

    wolfTimer = new QTimer(this);
    connect(wolfTimer, &QTimer::timeout, gameController, &GameController::moveWolfs);

    // Подключаем слот updateSpinBoxes к сигналу timeout таймера wolfTimer
    connect(wolfTimer, &QTimer::timeout, this, &MainWindow::updateSpinBoxes);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
    delete gameController;
    delete gameTimer;
    delete plantTimer;
    delete wolfTimer;
}

void MainWindow::initializeScene()
{
    QRectF viewRect(0, 0, ui->graphicsView->width()-10, ui->graphicsView->height()-10);
    scene->setSceneRect(viewRect);
    scene->clear();

    int viewWidth = ui->graphicsView->width();
    int viewHeight = ui->graphicsView->height();

    int numWolfs = ui->wolfsspinBox->value();
    gameController->generateWolfs(numWolfs, viewWidth, viewHeight);

    int numPlants = ui->plantsspinBox->value();
    gameController->generatePlants(numPlants, viewWidth, viewHeight);

    int numSheeps = ui->sheepsspinBox->value();
    gameController->generateSheeps(numSheeps, viewWidth, viewHeight);

    gameTimer->start(30);
    plantTimer->start(1500);
    wolfTimer->start(20);
}

void MainWindow::on_startButton_clicked()
{
    ui->plantsspinBox->setDisabled(true);
    ui->sheepsspinBox->setDisabled(true);
    ui->wolfsspinBox->setDisabled(true);
    initializeScene();
}

void MainWindow::updateSpinBoxes()
{
    ui->wolfsspinBox->setValue(gameController->getWolfList().size());
    ui->sheepsspinBox->setValue(gameController->getSheepList().size());
    ui->plantsspinBox->setValue(gameController->getPlantList().size());
}

void MainWindow::updateTimerIntervals()
{
    int intervalMultiplier = 1;
    switch (ui->gameSpeedSlider->value()) {
    case 1:
        gameTimer->setInterval(28 * intervalMultiplier);
        plantTimer->setInterval(2000 * intervalMultiplier);
        wolfTimer->setInterval(20 * intervalMultiplier);
        break;
    case 2:
        gameTimer->setInterval(14 * intervalMultiplier);
        plantTimer->setInterval(1000 * intervalMultiplier);
        wolfTimer->setInterval(10 * intervalMultiplier);
        break;
    case 3:
        gameTimer->setInterval(7 * intervalMultiplier);
        plantTimer->setInterval(500 * intervalMultiplier);
        wolfTimer->setInterval(5 * intervalMultiplier);
        break;
    default:
        break;
    }
}

void MainWindow::on_pauseButton_clicked()
{
    if (gameTimer->isActive()) {
        gameTimer->stop();
        plantTimer->stop();
        wolfTimer->stop();
    } else {
        gameTimer->start();
        plantTimer->start();
        wolfTimer->start();
        updateTimerIntervals();
    }
}

void MainWindow::on_gameSpeedSlider_valueChanged(int value)
{
    updateTimerIntervals();
}


