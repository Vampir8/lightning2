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

#include <QtpPin.h>
#include <QtpWire.h>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QPainter>

QtpPin::QtpPin(PinType pinType, QString pinName, int pinIndex, QGraphicsItem* parent)
    : QGraphicsPolygonItem(parent)
    //: QGraphicsPolygonItem(parent)
    , _pinIndex(pinIndex)
{
    _pinType = pinType;

    width = 5;
    height = 5;

    _polygon << QPointF(-width, height) << QPointF(width, height) << QPointF(width, -height) << QPointF(-width, -height) << QPointF(-width, height);

    _nameText = new QGraphicsTextItem(this);
    _nameText->setOpacity(0.4);
    _nameText->setDefaultTextColor(Qt::red);
     setPen(QPen(Qt::white));

    setName(pinName);

    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
    setPolygon(_polygon);
}

QtpPin::~QtpPin()
{
    // delete wires attached to pin when pin is deleted
    foreach (QtpWire* wire, _wires)
    {
        delete wire;
    }

    removeWires();
}

std::string QtpPin::name()
{
    return _nameText->toPlainText().toUtf8().constData();
}

int QtpPin::index()
{
    return _pinIndex;
}

int QtpPin::type() const
{
    return Type;
}

bool QtpPin::addWire(QtpWire* newWire)
{
    foreach (QtpWire* wire, _wires)
    {
        if ((_pinType == InPin && wire->startPin() == newWire->startPin()) ||
            (_pinType == OutPin && wire->endPin() == newWire->endPin()))
        {
            // wire already exists
            return false;
        }
    }

    _wires.append(newWire);
    return true;
}

void QtpPin::removeWire(QtpWire* wire)
{
    int index = _wires.indexOf(wire);

    if (index != -1)
    {
        _wires.removeAt(index);
    }
}

void QtpPin::removeWires()
{
    _wires.clear();
}

QList<QtpWire*> QtpPin::wires()
{
    return _wires;
}

QtpPin::PinType QtpPin::pinType() const
{
    return _pinType;
}

QPolygonF QtpPin::polygon() const
{
    return _polygon;
}

void QtpPin::hover(bool hover)
{
    if (hover)
    {
        _nameText->setDefaultTextColor(Qt::white);
        _nameText->setOpacity(1.0);
        if (_pinType == InPin)
            setBrush(Qt::green);
        if (_pinType == OutPin)
            setBrush(Qt::yellow);

        foreach (QtpWire* wire, _wires)
        {
            wire->setColor(Qt::red);
            wire->setPen(QPen(Qt::red));
        }

    }
    else
    {
        if (_pinType == InPin)
            setBrush(Qt::darkGreen);
        if (_pinType == OutPin)
            setBrush(Qt::darkYellow);
        _nameText->setDefaultTextColor(Qt::red);
        _nameText->setOpacity(0.4);
        foreach (QtpWire* wire, _wires)
        {
            wire->setColor(Qt::white);
            wire->setPen(QPen(Qt::white));
        }
    }
}

QVariant QtpPin::itemChange(GraphicsItemChange change, const QVariant& value)
{
    if (change == QGraphicsItem::ItemPositionChange)
    {
        foreach (QtpWire* wire, _wires)
        {
            wire->updatePosition();
        }
    }

    return value;
}

void QtpPin::setName(QString name)
{
    _nameText->setPlainText(name);

    if (_pinType == InPin)
    {
        _nameText->setPos(-_nameText->boundingRect().width() - 7, -11);
    }
    else
    {
        _nameText->setPos(7, -11);
    }
}
