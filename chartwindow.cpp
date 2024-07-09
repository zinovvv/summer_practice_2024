#include "chartwindow.h"
#include "ui_chartwindow.h"
#include "gamecontroller.h"
#include <QRandomGenerator>
#include <QPointF>
#include <QTimer>

chartwindow::chartwindow(QGraphicsScene *mainScene, GameController *controller, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::chartwindow)
{
    ui->setupUi(this);
    setFixedSize(width(), height());
    setWindowTitle("Графики популяций");
    chartScene = new QGraphicsScene(this);
    ui->chartView->setScene(chartScene);

    // Используем переданный контроллер
    gameController = controller;

    chartUpdateTimer = new QTimer(this);
    connect(chartUpdateTimer, &QTimer::timeout, this, &chartwindow::updateChart);
    chartUpdateTimer->start(100);
}

chartwindow::~chartwindow()
{
    delete ui;
    delete chartScene;
}

void chartwindow::pauseChartUpdateTimer()
{
    if (chartUpdateTimer && chartUpdateTimer->isActive()) {
        chartUpdateTimer->stop();
    }
}

void chartwindow::resumeChartUpdateTimer()
{
    if (chartUpdateTimer && !chartUpdateTimer->isActive()) {
        chartUpdateTimer->start();
        updateChart(); // Если нужно обновить чарт при возобновлении
    }
}

void chartwindow::updateChart()
{

    plantData.push_back(gameController->getPlantList().size());
    sheepData.push_back(gameController->getSheepList().size());
    wolfData.push_back(gameController->getWolfList().size());

    if (plantData.size() > 50) plantData.erase(plantData.begin());
    if (sheepData.size() > 50) sheepData.erase(sheepData.begin());
    if (wolfData.size() > 50) wolfData.erase(wolfData.begin());

    chartScene->clear();

    int width = ui->chartView->width();
    int height = ui->chartView->height();
    int margin = 20;
    int graphWidth = width - 2 * margin;
    int graphHeight = height - 2 * margin;

    auto drawGraph = [&](const std::vector<int>& data, QColor color) {
        QPen pen(color, 2);
        for (size_t i = 1; i < data.size(); ++i) {
            int x1 = margin + (i - 1) * graphWidth / (data.size() - 1);
            int y1 = margin + graphHeight - data[i - 1] * graphHeight / 100;
            int x2 = margin + i * graphWidth / (data.size() - 1);
            int y2 = margin + graphHeight - data[i] * graphHeight / 100;
            chartScene->addLine(x1, y1, x2, y2, pen);
        }
    };

    drawGraph(plantData, Qt::green);
    drawGraph(sheepData, Qt::blue);
    drawGraph(wolfData, Qt::red);

    // Добавляем легенду
    int legendX = margin;
    int legendY = margin + graphHeight + 10;

    auto addLegendItem = [&](const QString& text, QColor color, int offsetX) {
        QGraphicsRectItem* rect = new QGraphicsRectItem(legendX + offsetX, legendY, 10, 10);
        rect->setBrush(QBrush(color));
        chartScene->addItem(rect);

        QGraphicsTextItem* label = new QGraphicsTextItem(text);
        label->setPos(legendX + offsetX + 15, legendY - 5);
        chartScene->addItem(label);
    };

    addLegendItem("Растения", Qt::green, 0);
    addLegendItem("Овцы", Qt::blue, 80);
    addLegendItem("Волки", Qt::red, 140);
}
