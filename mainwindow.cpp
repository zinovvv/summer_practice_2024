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
    currentGameTimerInterval = 28;
    currentWolfTimerInterval = 20;
    intervalMultiplier=1;
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
    chartWindow = new chartwindow(scene, gameController, this);
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

    gameTimer->start(currentGameTimerInterval);
    plantTimer->start(currentPlantTimerInterval/intervalMultiplier);
    wolfTimer->start(currentWolfTimerInterval);

}

void MainWindow::on_startButton_clicked()
{
    ui->restartButton->setEnabled(true);
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
    switch (ui->gameSpeedSlider->value()) {
    case 1:
        gameTimer->setInterval(currentGameTimerInterval);
        wolfTimer->setInterval(currentWolfTimerInterval);
        plantTimer->setInterval(currentPlantTimerInterval);
        break;
    case 2:
        gameTimer->setInterval(currentGameTimerInterval);
        wolfTimer->setInterval(currentWolfTimerInterval);
        plantTimer->setInterval(currentPlantTimerInterval/intervalMultiplier);

        break;
    case 3:
        gameTimer->setInterval(currentGameTimerInterval);
        wolfTimer->setInterval(currentWolfTimerInterval);
        plantTimer->setInterval(currentPlantTimerInterval/intervalMultiplier);
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
    int baseGameInterval=28;
    int baseWolfInterval=20;
    switch(value){
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
        intervalMultiplier=1;
        break;
    }
    currentGameTimerInterval=baseGameInterval/intervalMultiplier;
    currentWolfTimerInterval=baseWolfInterval/intervalMultiplier;
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
    chartWindow->show();
}

// В файле mainwindow.cpp
void MainWindow::on_sheepBirthRateSlider_valueChanged(int value)
{
    int sheepBirthRate = 1; // Значение по умолчанию

    switch (value) {
    case 1:
        sheepBirthRate = 1;
        break;
    case 2:
        sheepBirthRate = 2;
        break;
    case 3:
        sheepBirthRate = 3;
        break;
    default:
        break;
    }
    gameController->resetTotalEatenPlantCount();
    gameController->setSheepBirthRate(sheepBirthRate);
}

void MainWindow::on_wolfBirthRateSlider_valueChanged(int value)
{
    int wolfBirthRate = 1; // Значение по умолчанию

    switch (value) {
    case 1:
        wolfBirthRate = 1;
        break;
    case 2:
        wolfBirthRate = 2;
        break;
    case 3:
        wolfBirthRate = 3;
        break;
    default:
        break;
    }
    gameController->resetTotalEatenSheepCount();
    gameController->setWolfBirthRate(wolfBirthRate);
}

void MainWindow::on_restartButton_clicked()
{
    ui->restartButton->setEnabled(false);
    // Останавливаем все таймеры
    gameTimer->stop();
    plantTimer->stop();
    wolfTimer->stop();
    chartWindow->pauseChartUpdateTimer();

    // Закрываем и удаляем окно chartWindow
    if (chartWindow) {
        chartWindow->close(); // Закрываем окно
        delete chartWindow;   // Удаляем объект
        chartWindow = nullptr;
    }

    // Очищаем сцену
    scene->clear();

    // Очищаем сцену
    scene->clear();

    // Сбрасываем контроллер
    gameController->reset();

    // Разблокируем спин-боксы
    ui->plantsspinBox->setDisabled(false);
    ui->sheepsspinBox->setDisabled(false);
    ui->wolfsspinBox->setDisabled(false);

    // Скрываем кнопку графика
    ui->showChartButton->hide();

    // Сбрасываем текст на кнопке паузы
    ui->pauseButton->setText("Пауза");
    ui->pauseButton->setEnabled(false);
    ui->startButton->show();

    ui->wolfsspinBox->setValue(0);
    ui->plantsspinBox->setValue(0);
    ui->sheepsspinBox->setValue(0);
}

