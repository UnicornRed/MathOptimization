#include "mygraphicsscene.h"
#include <iostream>

MyGraphicsScene::MyGraphicsScene(QObject *parent) :
    QGraphicsScene()
{
    Q_UNUSED(parent);
}

MyGraphicsScene::~MyGraphicsScene()
{

}

void MyGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit signalTargetCoordinate(event->scenePos());
}
