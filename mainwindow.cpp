// Ваш MainWindow.cpp

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "gamecontroller.h"
#include "chartwindow.h" // Подключаем новое окно
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
    ui->showChartButton->hide();
    currentPlantTimerInterval = 1500;

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
    plantTimer->start(currentPlantTimerInterval);
    wolfTimer->start(20);

}

void MainWindow::on_startButton_clicked()
{
    ui->showChartButton->show();
    ui->plantsspinBox->setDisabled(true);
    ui->sheepsspinBox->setDisabled(true);
    ui->wolfsspinBox->setDisabled(true);
    ui->pauseButton->setEnabled(true);
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
        wolfTimer->setInterval(20 * intervalMultiplier);
        plantTimer->setInterval(currentPlantTimerInterval);
        break;
    case 2:
        gameTimer->setInterval(14 * intervalMultiplier);
        wolfTimer->setInterval(10 * intervalMultiplier);
        plantTimer->setInterval(currentPlantTimerInterval/2);

        break;
    case 3:
        gameTimer->setInterval(7 * intervalMultiplier);
        wolfTimer->setInterval(5 * intervalMultiplier);
        plantTimer->setInterval(currentPlantTimerInterval/4);
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
        chartWindow->pauseChartUpdateTimer();

        ui->pauseButton->setText("Возобновить");
    } else {
        ui->pauseButton->setText("Пауза");
        gameTimer->start();
        plantTimer->start();
        wolfTimer->start();
        chartWindow->resumeChartUpdateTimer();
        updateTimerIntervals();
    }
}

void MainWindow::on_gameSpeedSlider_valueChanged(int value)
{
    updateTimerIntervals();
}

void MainWindow::on_plantSpeedSlider_valueChanged(int value)
{
    // Базовый интервал для генерации растений
    int basePlantInterval = 1500;

    switch (value) {
    case 1:
        currentPlantTimerInterval = basePlantInterval; // 1500
        break;
    case 2:
        currentPlantTimerInterval = basePlantInterval / 2; // 750
        break;
    case 3:
        currentPlantTimerInterval = basePlantInterval / 4; // 375
        break;
    default:
        currentPlantTimerInterval = basePlantInterval;
        break;
    }
    updateTimerIntervals();
}

void MainWindow::on_showChartButton_clicked()
{
    chartwindow *chartWindow = new chartwindow(scene, gameController, this);
    chartWindow->show();
}
