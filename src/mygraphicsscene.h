#ifndef MYGRAPHICSSCENE_H
#define MYGRAPHICSSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class MyGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit MyGraphicsScene(QObject *parent = 0);
    ~MyGraphicsScene();

signals:
    void signalTargetCoordinate(QPointF point);

public slots:

private:
    void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
};

#endif // MYGRAPHICSSCENE_H
