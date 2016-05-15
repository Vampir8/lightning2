/************************************************************************
QtPatcher - Cross-Platform Circuit Drawing Library
Copyright (c) 2014-2015 Marcus Tomlinson

This file is part of QtPatcher.

GNU Lesser General Public License Usage
This file may be used under the terms of the GNU Lesser General Public
License version 3.0 as published by the Free Software Foundation and
appearing in the file LGPLv3.txt included in the packaging of this
file. Please review the following information to ensure the GNU Lesser
General Public License version 3.0 requirements will be met:
http://www.gnu.org/copyleft/lgpl.html.

Other Usage
Alternatively, this file may be used in accordance with the terms and
conditions contained in a signed written agreement between you and
Marcus Tomlinson.

DSPatch is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
************************************************************************/

#include <QtpComp.h>
#include <QtpPin.h>

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include <QtWidgets>

QtpComp::QtpComp(CompInfo const& compInfo, int id, QPointF const& position, QGraphicsItem* parent)
    : QGraphicsPolygonItem(parent)
{
    setPos(position);
    width = 10;
    height = 10;
    _id = id;
    _color = Qt::white;
    _compInfo = compInfo;
    _contextMenu = new QMenu();
     setPen(QPen(Qt::white));

    _polygon << QPointF(-width, -height) << QPointF(width, -height) << QPointF(width, height) << QPointF(-width, height) << QPointF(-width, -height);
    //_polygon.operator = (QRectF(-10,-10,10,10));

    _nameText = new QGraphicsTextItem(this);
    _nameText->setDefaultTextColor(Qt::white);

    setName(compInfo.typeName);
    foreach (QString const& pin, compInfo.inPins)
    {
        addInPin(pin);
    }
    foreach (QString const& pin, compInfo.outPins)
    {
        addOutPin(pin);
    }

    setPolygon(_polygon);

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

QtpComp::~QtpComp()
{
    delete _contextMenu;
    removeInPins();
    removeOutPins();
}

void QtpComp::save(QDataStream &ds)
{
    /*ds << pos();

    int count(0);

    foreach(QGraphicsItem *port_, childItems())
    {
        if (port_->type() != QtpPin::Type)
            continue;

        count++;
    }

    ds << count;

    foreach(QGraphicsItem *port_, childItems())
    {
        if (port_->type() != QtpPin::Type)
            continue;

        QtpPin *port = (QtpPin*) port_;
        ds << (quint64) port;
        ds << port->i;
        ds << port->isOutput();
        ds << port->portFlags();
    }*/
}

int QtpComp::id()
{
    return _id;
}

int QtpComp::type() const
{
    return Type;
}

void QtpComp::setColor(const QColor& color)
{
    _color = color;
    setBrush(_color);

    foreach (QtpPin* pin, _inPins)
    {
        pin->setBrush(_color);
    }

    foreach (QtpPin* pin, _outPins)
    {
        pin->setBrush(_color);
    }
}

void QtpComp::addInPin(QString pinName)
{
    QtpPin* pin = new QtpPin(QtpPin::InPin, pinName, _inPins.size(), this);
    pin->setPos(-width-3,15 * _inPins.size());
    pin->setBrush(Qt::darkGreen);
    _inPins.push_back(pin);

    updatePolygon();
}

void QtpComp::addOutPin(QString pinName)
{
    QtpPin* pin = new QtpPin(QtpPin::OutPin, pinName, _outPins.size(), this);
    pin->setPos(width+3,15 * _outPins.size());
    pin->setBrush(Qt::darkYellow);
    _outPins.push_back(pin);

    updatePolygon();
}

void QtpComp::removeInPin()
{
    delete _inPins.back();
    _inPins.pop_back();

    updatePolygon();
}

void QtpComp::removeOutPin()
{
    delete _outPins.back();
    _outPins.pop_back();

    updatePolygon();
}

void QtpComp::removeInPins()
{
    foreach (QtpPin* pin, _inPins)
    {
        delete pin;
    }

    _inPins.clear();

    updatePolygon();
}

void QtpComp::removeOutPins()
{
    foreach (QtpPin* pin, _outPins)
    {
        delete pin;
    }

    _outPins.clear();

    updatePolygon();
}

QMenu* QtpComp::contextMenu()
{
    return _contextMenu;
}

QPixmap QtpComp::image() const
{
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 1));

    int pinsAfterFirst = std::max(_inPins.size(), _outPins.size());
    pinsAfterFirst = --pinsAfterFirst < 0 ? 0 : pinsAfterFirst;

    painter.translate(125, 125 - (10 * pinsAfterFirst));
    painter.drawPolyline(_polygon);

    painter.translate(-20, 0);
    foreach (QtpPin* pin, _inPins)
    {
        painter.drawPolyline(pin->polygon());
        painter.translate(0, 20);
    }

    painter.resetTransform();
    painter.translate(145, 125 - (10 * pinsAfterFirst));
    foreach (QtpPin* pin, _outPins)
    {
        painter.drawPolyline(pin->polygon());
        painter.translate(0, 20);
    }

    return pixmap;
}


QtpComp::CompInfo QtpComp::compInfo() const
{
    return _compInfo;
}

void QtpComp::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    if (_contextMenu->actions().size() != 0)
    {
        scene()->clearSelection();
        setSelected(true);
        _contextMenu->exec(event->screenPos());
    }
}

void QtpComp::setName(QString name)
{
    _nameText->setPlainText(name);
    _nameText->setRotation(90);
    _nameText->setPos(11, -5);

}

void QtpComp::updatePolygon()
{
    int pinsAfterFirst = std::max(_inPins.size(), _outPins.size());
    pinsAfterFirst = --pinsAfterFirst < 0 ? 0 : pinsAfterFirst;
    int nameWidth=_nameText->boundingRect().width();
    if (nameWidth > (height + ((height+5) * pinsAfterFirst)))
    {
    _polygon.replace(2, QPointF(width, nameWidth));
    _polygon.replace(3, QPointF(-width, nameWidth));
    }else
    {
        _polygon.replace(2, QPointF(width, height + ((height+5) * pinsAfterFirst)));
        _polygon.replace(3, QPointF(-width, height + ((height+5) * pinsAfterFirst)));

    }

    setPolygon(_polygon);
}
