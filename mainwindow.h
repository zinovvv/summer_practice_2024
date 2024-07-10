#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include "gamecontroller.h"
#include "chartwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleStartButtonClick();
    void updateSpinBoxes();
    void handlePauseButtonClick();
    void handleGameSpeedSliderChange(int value);
    void handlePlantSpeedSliderChange(int value);
    void handleShowChartButtonClick();
    void handleSheepBirthRateSliderChange(int value);
    void handleWolfBirthRateSliderChange(int value);
    void handleRestartButtonClick();
    void checkForEndGameCondition();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    chartwindow *chartWindow;
    QTimer *sheepMovementTimer;
    QTimer *plantGenerationTimer;
    QTimer *wolfMovementTimer;
    QTimer *endGameCheckTimer;
    GameController *gameController;
    int currentPlantTimerInterval;
    int currentGameTimerInterval;
    int currentWolfTimerInterval;
    int intervalMultiplier;
    void initializeGameScene();
    void updateAllTimerIntervals();
};

#endif // MAINWINDOW_H
