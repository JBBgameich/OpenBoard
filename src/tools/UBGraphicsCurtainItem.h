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




#ifndef UBGRAPHICSCURTAINITEM_H_
#define UBGRAPHICSCURTAINITEM_H_

#include <QtGui>

#include "core/UB.h"

#include "domain/UBItem.h"

class UBGraphicsItemDelegate;


class UBGraphicsCurtainItem : public QObject, public QGraphicsRectItem, public UBItem, public UBGraphicsItem
{

    Q_OBJECT

    public:
        UBGraphicsCurtainItem(QGraphicsItem* parent = nullptr);
        ~UBGraphicsCurtainItem() override;

        enum { Type = UBGraphicsItemType::CurtainItemType };

        int type() const override
        {
            return Type;
        }

        UBItem* deepCopy() const override;
        void copyItemParameters(UBItem *copy) const override;

        //TODO UB 4.x not nice ...
        void triggerRemovedSignal();
        void clearSource() override{;}

        void setUuid(const QUuid &pUuid) override;

     signals:

        void removed();

     protected:

        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

        QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

        QColor  drawColor() const;
        QColor  opaqueControlColor() const;

//        UBGraphicsItemDelegate* mDelegate;

        static const QColor sDrawColor;
        static const QColor sDarkBackgroundDrawColor;
        static const QColor sOpaqueControlColor;
        static const QColor sDarkBackgroundOpaqueControlColor;
};

#endif /* UBGRAPHICSCURTAINITEM_H_ */
