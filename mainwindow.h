#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QList>
#include <QTimer>
#include <QPointF>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Plant;
class Sheep;
class Wolf;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void moveSheepTowardsPlants();
    void moveWolfs();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QTimer *gameTimer;
    QTimer *plantTimer;
    QTimer *wolfTimer;

    QList<Plant*> plantList;
    QList<Sheep*> sheepList;
    QList<Wolf*> wolfList;

    void initializeScene();
    void generatePlants(int numPlants, int viewWidth, int viewHeight);
    void generateSheeps(int numSheeps, int viewWidth, int viewHeight);
    void generateWolfs(int numWolfs, int viewWidth, int viewHeight);
    QPointF generateRandomPosition(int viewWidth, int viewHeight);
    void updateGame();
};

#endif // MAINWINDOW_H
