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




#ifndef UBGRAPHICSRULER_H_
#define UBGRAPHICSRULER_H_

#include <QtGui>
#include <QtSvg>

#include "core/UB.h"
#include "domain/UBItem.h"
#include "tools/UBAbstractDrawRuler.h"

class UBGraphicsScene;

class UBGraphicsRuler : public UBAbstractDrawRuler, public QGraphicsRectItem, public UBItem
{
    Q_OBJECT;

    public:
        UBGraphicsRuler();
        ~UBGraphicsRuler() override;

        enum { Type = UBGraphicsItemType::RulerItemType };

        int type() const override
        {
            return Type;
        }

        UBItem* deepCopy() const override;
        void copyItemParameters(UBItem *copy) const override;

        void StartLine(const QPointF& position, qreal width) override;
        void DrawLine(const QPointF& position, qreal width) override;
        void EndLine() override;

    protected:

        void paint(QPainter *painter, const QStyleOptionGraphicsItem *styleOption, QWidget *widget) override;
        QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

        // Events
        void    mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        void    mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
        void    mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
        void    hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
        void    hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
        void    hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
        void paintGraduations(QPainter *painter) override;

    private:

        bool mResizing;
        bool mRotating;


        // Helpers
        void    fillBackground(QPainter *painter);
        void    paintRotationCenter(QPainter *painter);
        void    rotateAroundCenter(qreal angle) override;

        QGraphicsSvgItem* mRotateSvgItem;
        QGraphicsSvgItem* mResizeSvgItem;

        void updateResizeCursor();
        QCursor resizeCursor() const{return mResizeCursor;}

        QPointF             rotationCenter() const override;
        virtual QRectF           resizeButtonRect() const;
        QRectF            closeButtonRect() const override;
        virtual QRectF           rotateButtonRect() const;
        UBGraphicsScene*            scene() const override;

        QCursor mResizeCursor;

        int drawLineDirection;

        // Constants
        static const QRect               sDefaultRect;

        static const int    sMinLength = 150;   // 3sm
        static const int    sMaxLength = 35000; // 700sm

        qreal mStrokeWidth;
};

#endif /* UBGRAPHICSRULER_H_ */
