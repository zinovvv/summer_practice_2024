#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scene(new QGraphicsScene(this))
    , chartWindow(nullptr)
    , sheepMovementTimer(new QTimer(this))
    , plantGenerationTimer(new QTimer(this))
    , wolfMovementTimer(new QTimer(this))
    , endGameCheckTimer(new QTimer(this))
    , gameController(new GameController(scene, this))
    , currentPlantTimerInterval(1500)
    , currentGameTimerInterval(28)
    , currentWolfTimerInterval(15)
    , intervalMultiplier(1)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(scene);
    ui->showChartButton->hide();

    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::handleStartButtonClick);
    connect(ui->gameSpeedSlider, &QSlider::valueChanged, this, &MainWindow::handleGameSpeedSliderChange);
    connect(ui->plantSpeedSlider, &QSlider::valueChanged, this, &MainWindow::handlePlantSpeedSliderChange);
    connect(ui->sheepBirthRateSlider, &QSlider::valueChanged, this, &MainWindow::handleSheepBirthRateSliderChange);
    connect(ui->wolfBirthRateSlider, &QSlider::valueChanged, this, &MainWindow::handleWolfBirthRateSliderChange);
    connect(ui->pauseButton, &QPushButton::clicked, this, &MainWindow::handlePauseButtonClick);
    connect(ui->showChartButton, &QPushButton::clicked, this, &MainWindow::handleShowChartButtonClick);
    connect(ui->restartButton, &QPushButton::clicked, this, &MainWindow::handleRestartButtonClick);

    connect(sheepMovementTimer, &QTimer::timeout, gameController, &GameController::moveSheepToPlants);
    connect(plantGenerationTimer, &QTimer::timeout, this, [this]() {
        QRectF viewRect = ui->graphicsView->sceneRect();
        gameController->spawnPlants(1, viewRect.width(), viewRect.height());
    });
    connect(wolfMovementTimer, &QTimer::timeout, gameController, &GameController::moveWolves);
    connect(wolfMovementTimer, &QTimer::timeout, this, &MainWindow::updateSpinBoxes);
    connect(endGameCheckTimer, &QTimer::timeout, this, &MainWindow::checkForEndGameCondition);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
    delete gameController;
    delete sheepMovementTimer;
    delete plantGenerationTimer;
    delete wolfMovementTimer;
    delete endGameCheckTimer;
}

void MainWindow::initializeGameScene()
{
    if (!chartWindow) {
        chartWindow = new chartwindow(scene, gameController, this);
    }

    QRectF viewRect(0, 0, ui->graphicsView->width() - 10, ui->graphicsView->height() - 10);
    scene->setSceneRect(viewRect);
    scene->clear();

    int viewWidth = ui->graphicsView->width();
    int viewHeight = ui->graphicsView->height();

    int numWolves = ui->wolfsspinBox->value();
    gameController->spawnWolves(numWolves, viewWidth, viewHeight);

    int numPlants = ui->plantsspinBox->value();
    gameController->spawnPlants(numPlants, viewWidth, viewHeight);

    int numSheep = ui->sheepsspinBox->value();
    gameController->spawnSheep(numSheep, viewWidth, viewHeight);

    sheepMovementTimer->start(currentGameTimerInterval);
    plantGenerationTimer->start(currentPlantTimerInterval / intervalMultiplier);
    wolfMovementTimer->start(currentWolfTimerInterval);
    endGameCheckTimer->start(1000);
}

void MainWindow::handleStartButtonClick()
{
    ui->wolfsspinBox->setMinimum(0);
    ui->sheepsspinBox->setMinimum(0);
    ui->plantsspinBox->setMaximum(1000);
    ui->wolfsspinBox->setMaximum(100);
    ui->sheepsspinBox->setMaximum(100);
    ui->restartButton->setEnabled(true);
    ui->showChartButton->show();
    ui->plantsspinBox->setDisabled(true);
    ui->sheepsspinBox->setDisabled(true);
    ui->wolfsspinBox->setDisabled(true);
    ui->pauseButton->setEnabled(true);
    initializeGameScene();
}

void MainWindow::updateSpinBoxes()
{

    ui->wolfsspinBox->setValue(gameController->getWolfList().size());
    ui->sheepsspinBox->setValue(gameController->getSheepList().size());
    ui->plantsspinBox->setValue(gameController->getPlantList().size());
}

void MainWindow::updateAllTimerIntervals()
{
    switch (ui->gameSpeedSlider->value()) {
    case 1:
        sheepMovementTimer->setInterval(currentGameTimerInterval);
        wolfMovementTimer->setInterval(currentWolfTimerInterval);
        plantGenerationTimer->setInterval(currentPlantTimerInterval);
        break;
    case 2:
        sheepMovementTimer->setInterval(currentGameTimerInterval);
        wolfMovementTimer->setInterval(currentWolfTimerInterval);
        plantGenerationTimer->setInterval(currentPlantTimerInterval / intervalMultiplier);
        break;
    case 3:
        sheepMovementTimer->setInterval(currentGameTimerInterval);
        wolfMovementTimer->setInterval(currentWolfTimerInterval);
        plantGenerationTimer->setInterval(currentPlantTimerInterval / intervalMultiplier);
        break;
    default:
        break;
    }
}

void MainWindow::handlePauseButtonClick()
{
    if (sheepMovementTimer->isActive()) {
        sheepMovementTimer->stop();
        plantGenerationTimer->stop();
        wolfMovementTimer->stop();
        chartWindow->pauseChartUpdateTimer();
        ui->pauseButton->setText("Возобновить");
    } else {
        ui->pauseButton->setText("Пауза");
        sheepMovementTimer->start();
        plantGenerationTimer->start();
        wolfMovementTimer->start();
        chartWindow->resumeChartUpdateTimer();
        updateAllTimerIntervals();
    }
}

void MainWindow::handleGameSpeedSliderChange(int value)
{
    int baseGameInterval = 28;
    int baseWolfInterval = 20;
    switch (value) {
    case 1:
        intervalMultiplier = 1;
        break;
    case 2:
        intervalMultiplier = 2;
        break;
    case 3:
        intervalMultiplier = 4;
        break;
    default:
        intervalMultiplier = 1;
        break;
    }
    currentGameTimerInterval = baseGameInterval / intervalMultiplier;
    currentWolfTimerInterval = baseWolfInterval / intervalMultiplier;
    updateAllTimerIntervals();
}

void MainWindow::handlePlantSpeedSliderChange(int value)
{
    int basePlantInterval = 1500;
    switch (value) {
    case 1:
        currentPlantTimerInterval = basePlantInterval;
        break;
    case 2:
        currentPlantTimerInterval = basePlantInterval / 2;
        break;
    case 3:
        currentPlantTimerInterval = basePlantInterval / 4;
        break;
    default:
        currentPlantTimerInterval = basePlantInterval;
        break;
    }
    updateAllTimerIntervals();
}

void MainWindow::handleShowChartButtonClick()
{
    if (chartWindow) {
        chartWindow->show();
    }
}

void MainWindow::handleSheepBirthRateSliderChange(int value)
{
    int sheepBirthRate = value;
    gameController->resetTotalEatenPlantCount();
    gameController->setSheepBirthRate(sheepBirthRate);
}

void MainWindow::handleWolfBirthRateSliderChange(int value)
{
    int wolfBirthRate = value;
    gameController->resetTotalEatenSheepCount();
    gameController->setWolfBirthRate(wolfBirthRate);
}

void MainWindow::handleRestartButtonClick()
{
    ui->restartButton->setEnabled(false);
    endGameCheckTimer->stop();
    sheepMovementTimer->stop();
    plantGenerationTimer->stop();
    wolfMovementTimer->stop();

    if (chartWindow) {
        chartWindow->pauseChartUpdateTimer();
        chartWindow->close();
        delete chartWindow;
        chartWindow = nullptr;
    }

    scene->clear();
    gameController->resetGame();
    ui->plantsspinBox->setDisabled(false);
    ui->sheepsspinBox->setDisabled(false);
    ui->wolfsspinBox->setDisabled(false);
    ui->showChartButton->hide();
    ui->pauseButton->setText("Пауза");
    ui->pauseButton->setEnabled(false);
    ui->startButton->show();
    ui->wolfsspinBox->setValue(0);
    ui->plantsspinBox->setValue(0);
    ui->sheepsspinBox->setValue(0);
    ui->wolfsspinBox->setMinimum(1);
    ui->sheepsspinBox->setMinimum(1);
    ui->plantsspinBox->setMaximum(30);
    ui->wolfsspinBox->setMaximum(30);
    ui->sheepsspinBox->setMaximum(30);
}

void MainWindow::checkForEndGameCondition()
{
    if (gameController->getSheepList().isEmpty() && gameController->getWolfList().isEmpty()) {
        ui->restartButton->setEnabled(false);
        sheepMovementTimer->stop();
        plantGenerationTimer->stop();
        wolfMovementTimer->stop();
        if (chartWindow) {
            chartWindow->pauseChartUpdateTimer();
            chartWindow->close();
            delete chartWindow;
            chartWindow = nullptr;
        }
        scene->clear();
        gameController->resetGame();
        ui->plantsspinBox->setDisabled(false);
        ui->sheepsspinBox->setDisabled(false);
        ui->wolfsspinBox->setDisabled(false);
        ui->showChartButton->hide();
        ui->pauseButton->setText("Пауза");
        ui->pauseButton->setEnabled(false);
        ui->startButton->show();
        ui->wolfsspinBox->setValue(0);
        ui->plantsspinBox->setValue(0);
        ui->sheepsspinBox->setValue(0);
        endGameCheckTimer->stop();
        ui->wolfsspinBox->setMinimum(1);
        ui->sheepsspinBox->setMinimum(1);
        ui->plantsspinBox->setMaximum(30);
        ui->wolfsspinBox->setMaximum(30);
        ui->sheepsspinBox->setMaximum(30);
        QMessageBox::information(this, "Игра окончена", "Игра окончена.");
    }
}
