#ifndef CHARTWINDOW_H
#define CHARTWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "gamecontroller.h"
#include <QTimer>
#include <vector>

namespace Ui {
class chartwindow;
}

class chartwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit chartwindow(QGraphicsScene *mainScene, GameController *controller, QWidget *parent = nullptr);
    void pauseChartUpdateTimer();
    void resumeChartUpdateTimer();
    ~chartwindow();

private slots:
    void refreshChart();

private:
    Ui::chartwindow *ui;
    GameController *gameController;
    QGraphicsScene *chartGraphicsScene;
    QTimer *chartRefreshTimer;
    std::vector<int> plantData;
    std::vector<int> sheepData;
    std::vector<int> wolfData;
};

#endif // CHARTWINDOW_H
