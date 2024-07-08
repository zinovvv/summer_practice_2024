#ifndef CHARTWINDOW_H
#define CHARTWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "gamecontroller.h"
#include <QList>
#include <QTimer>
#include <QPointF>

namespace Ui {
class chartwindow;
}

class GameController;

class chartwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit chartwindow(QGraphicsScene *mainScene, GameController *controller, QWidget *parent = nullptr);
    void pauseChartUpdateTimer();
    void resumeChartUpdateTimer();

    ~chartwindow();

private slots:
    void updateChart();

private:
    GameController *gameController;
    Ui::chartwindow *ui;
    QGraphicsScene *chartScene;
    QTimer *chartUpdateTimer;
    std::vector<int> plantData;
    std::vector<int> sheepData;
    std::vector<int> wolfData;

};

#endif // CHARTWINDOW_H
