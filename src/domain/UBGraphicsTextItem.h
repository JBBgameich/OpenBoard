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




#ifndef UBGRAPHICSTEXTITEM_H_
#define UBGRAPHICSTEXTITEM_H_

#include <QtGui>
#include "UBItem.h"
#include "core/UB.h"
#include "UBResizableGraphicsItem.h"

class UBGraphicsItemDelegate;
class UBGraphicsScene;

class UBGraphicsTextItem : public QGraphicsTextItem, public UBItem, public UBResizableGraphicsItem, public UBGraphicsItem
{
    Q_OBJECT

    public:
        UBGraphicsTextItem(QGraphicsItem * parent = 0);
        ~UBGraphicsTextItem() override;

        enum { Type = UBGraphicsItemType::TextItemType };

        int type() const override
        {
            return Type;
        }

        UBItem* deepCopy() const override;

        void copyItemParameters(UBItem *copy) const override;

        UBGraphicsScene* scene() override;

        QRectF boundingRect() const override;
        QPainterPath shape() const override;

        void setTextWidth(qreal width);
        void setTextHeight(qreal height);
        qreal textHeight() const;
        qreal pixelsPerPoint() const;

        void contentsChanged();

        void resize(qreal w, qreal h) override;

        QSizeF size() const override;

        static QColor lastUsedTextColor;

        QColor colorOnDarkBackground() const
        {
            return mColorOnDarkBackground;
        }

        void setColorOnDarkBackground(QColor pColorOnDarkBackground)
        {
            mColorOnDarkBackground = pColorOnDarkBackground;
        }

        QColor colorOnLightBackground() const
        {
            return mColorOnLightBackground;
        }

        void setColorOnLightBackground(QColor pColorOnLightBackground)
        {
            mColorOnLightBackground = pColorOnLightBackground;
        }

        void clearSource() override{;}
        void setUuid(const QUuid &pUuid) override;
        void activateTextEditor(bool activate);
        void setSelected(bool selected);
        void recolor();

        QString mTypeTextHereLabel;


    signals:
        void textUndoCommandAdded(UBGraphicsTextItem *textItem);

    private slots:
        void undoCommandAdded();
        void documentSizeChanged(const QSizeF & newSize);

    private:
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

        void keyPressEvent(QKeyEvent *event) override;
        void keyReleaseEvent(QKeyEvent *event) override;

        void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;

        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

        QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

        qreal mTextHeight;

        int mMultiClickState;
        QTime mLastMousePressTime;

        QColor mColorOnDarkBackground;
        QColor mColorOnLightBackground;
        bool isActivatedTextEditor;
};

#endif /* UBGRAPHICSTEXTITEM_H_ */
