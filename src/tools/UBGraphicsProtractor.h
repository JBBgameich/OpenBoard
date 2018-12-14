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




#ifndef UBGRAPHICSPROTRACTOR_H_
#define UBGRAPHICSPROTRACTOR_H_

#include <QtWidgets>
#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QGraphicsView>
#include <QtSvg>

#include "core/UB.h"
#include "tools/UBAbstractDrawRuler.h"
#include "domain/UBItem.h"

#include "frameworks/UBGeometryUtils.h"


class UBGraphicsScene;

class UBGraphicsProtractor : public UBAbstractDrawRuler, public QGraphicsEllipseItem, public UBItem
{

    Q_OBJECT

    public:
        UBGraphicsProtractor ();
        enum Tool {None, Move, Resize, Rotate, Reset, Close, MoveMarker};

        qreal angle () { return mStartAngle; }
        qreal markerAngle () { return mCurrentAngle; }
        void  setAngle (qreal angle) { mStartAngle = angle; setStartAngle(mStartAngle * 16); }
        void  setMarkerAngle (qreal angle) { mCurrentAngle = angle; }

        UBItem* deepCopy() const override;
        void copyItemParameters(UBItem *copy) const override;

        enum { Type = UBGraphicsItemType::ProtractorItemType };

        int type() const override
        {
            return Type;
        }

    protected:
        void paint (QPainter *painter, const QStyleOptionGraphicsItem *styleOption, QWidget *widget) override;

        QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

        void   mousePressEvent (QGraphicsSceneMouseEvent *event) override;
        void    mouseMoveEvent (QGraphicsSceneMouseEvent *event) override;
        void mouseReleaseEvent (QGraphicsSceneMouseEvent *event) override;
        void   hoverEnterEvent (QGraphicsSceneHoverEvent *event) override;
        void   hoverLeaveEvent (QGraphicsSceneHoverEvent *event) override;
        void    hoverMoveEvent (QGraphicsSceneHoverEvent *event) override;
        QPainterPath shape() const override;
        QRectF boundingRect() const override;
        void paintGraduations(QPainter *painter) override;        


    private:
        // Helpers
        void paintButtons (QPainter *painter);
        void paintAngleMarker (QPainter *painter);
        Tool toolFromPos (QPointF pos);
        qreal antiScale () const;
        UBGraphicsScene*            scene() const override;
        QBrush                  fillBrush() const;

        QSizeF buttonSizeReference () const{return QSizeF(radius() / 10, mCloseSvgItem->boundingRect().height() * radius()/(10 * mCloseSvgItem->boundingRect().width()));}
        QSizeF markerSizeReference () const{return QSizeF(radius() / 10, mMarkerSvgItem->boundingRect().height() * radius()/(10 * mMarkerSvgItem->boundingRect().width()));}
        QRectF    resetButtonRect () const;

        QRectF    closeButtonRect () const override;
        QRectF    resizeButtonRect () const;
        QRectF    rotateButtonRect () const;
        QRectF    markerButtonRect () const;

        inline qreal radius () const{return rect().height() / 2 - 20;}

        // Members
        QPointF mPreviousMousePos;
        Tool    mCurrentTool;
        bool    mShowButtons;
        qreal   mCurrentAngle;
        qreal   mSpan;
        qreal   mStartAngle;
        qreal   mScaleFactor;

        QGraphicsSvgItem* mResetSvgItem;
        QGraphicsSvgItem* mResizeSvgItem;
        QGraphicsSvgItem* mMarkerSvgItem;
        QGraphicsSvgItem* mRotateSvgItem;

        static const QRectF sDefaultRect;
        static const qreal minRadius;

        void rotateAroundCenter(qreal angle) override;
        QPointF    rotationCenter() const override;

        int    sFillTransparency;
        int    sDrawTransparency;
};

#endif /* UBGRAPHICSPROTRACTOR_H_ */
