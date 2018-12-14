/*
 * Copyright (C) 2015-2018 Département de l'Instruction Publique (DIP-SEM)
 *
 * Copyright (C) 2013 Open Education Foundation
 *
 * Copyright (C) 2010-2013 Groupement d'Intérêt Public pour
 * l'Education Numérique en Afrique (GIP ENA)
 *
 * This file is part of OpenBoard.
 *
 * OpenBoard is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License,
 * with a specific linking exception for the OpenSSL project's
 * "OpenSSL" library (or with modified versions of it that use the
 * same license as the "OpenSSL" library).
 *
 * OpenBoard is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenBoard. If not, see <http://www.gnu.org/licenses/>.
 */




#ifndef UBGRAPHICSSTROKESGROUP_H
#define UBGRAPHICSSTROKESGROUP_H

#include <QGraphicsItemGroup>
#include <QGraphicsSceneMouseEvent>

#include "core/UB.h"
#include "UBItem.h"

class UBGraphicsStrokesGroup : public QObject, public QGraphicsItemGroup, public UBItem, public UBGraphicsItem
{
    Q_OBJECT
public:
    enum colorType {
        currentColor = 0
        , colorOnLightBackground
        , colorOnDarkBackground
    };

    UBGraphicsStrokesGroup(QGraphicsItem* parent = 0);
    ~UBGraphicsStrokesGroup() override;
    UBItem* deepCopy() const override;
    void copyItemParameters(UBItem *copy) const override;
    enum { Type = UBGraphicsItemType::StrokeItemType };
    int type() const override
    {
        return Type;
    }
    void setUuid(const QUuid &pUuid) override;
    void setColor(const QColor &color, colorType pColorType = currentColor);
    QColor color(colorType pColorType = currentColor) const;

protected:

    QPainterPath shape () const override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    // Graphical display of stroke Z-level
    bool debugTextEnabled;
    QGraphicsSimpleTextItem * mDebugText;
};

#endif // UBGRAPHICSSTROKESGROUP_H
