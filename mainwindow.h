#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QList>
#include <QTimer>
#include <QPointF>
#include "gamecontroller.h"
#include "chartwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startButton_clicked();
    void updateSpinBoxes();
    void on_pauseButton_clicked();
    void on_gameSpeedSlider_valueChanged(int value);

    void on_plantSpeedSlider_valueChanged(int value);
    void on_showChartButton_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    chartwindow *chartWindow;
    QTimer *gameTimer;
    QTimer *plantTimer;
    QTimer *wolfTimer;
    GameController *gameController;
    int currentPlantTimerInterval;

    void initializeScene();
    void updateTimerIntervals();

};

#endif // MAINWINDOW_H
