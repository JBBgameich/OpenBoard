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




#ifndef UBACTIONPALETTE_H_
#define UBACTIONPALETTE_H_

#include <QtGui>
#include <QPoint>
#include <QButtonGroup>
#include <QToolButton>

#include "UBFloatingPalette.h"

class UBActionPaletteButton;

class UBActionPalette : public UBFloatingPalette
{
    Q_OBJECT;

    public:
        UBActionPalette(QList<QAction*> actions, Qt::Orientation orientation = Qt::Vertical, QWidget* parent = nullptr);
        UBActionPalette(Qt::Orientation orientation, QWidget* parent = nullptr);
        UBActionPalette(Qt::Corner corner, QWidget* parent = nullptr, Qt::Orientation orient = Qt::Vertical);
        UBActionPalette(QWidget* parent = nullptr);

        ~UBActionPalette() override;

        void setButtonIconSize(const QSize& size);
        void setToolButtonStyle(Qt::ToolButtonStyle);

        QList<QAction*> actions();
        virtual void setActions(QList<QAction*> actions);
        void groupActions();
        virtual void addAction(QAction* action);

        void setClosable(bool closable);
        void setAutoClose(bool autoClose)
        {
            mAutoClose = autoClose;
        }

        void setCustomCloseProcessing(bool customCloseProcessing)
        {
            m_customCloseProcessing = customCloseProcessing;
        }
        bool m_customCloseProcessing;

        int border() override;
        virtual void clearLayout();
        QSize buttonSize();

        virtual UBActionPaletteButton* getButtonFromAction(QAction* action);

    public slots:
        void close();


    signals:
        void closed();
        void buttonGroupClicked(int id);
        void customMouseReleased();

    protected:
        void paintEvent(QPaintEvent *event) override;
        void mouseReleaseEvent(QMouseEvent * event) override;
        virtual void init(Qt::Orientation orientation);

        virtual void updateLayout();

        QList<UBActionPaletteButton*> mButtons;
        QButtonGroup* mButtonGroup;
        QList<QAction*> mActions;
        QMap<QAction*, UBActionPaletteButton*> mMapActionToButton;

        bool mIsClosable;
        Qt::ToolButtonStyle mToolButtonStyle;
        bool mAutoClose;
        QSize mButtonSize;
        QPoint mMousePos;
        UBActionPaletteButton *createPaletteButton(QAction* action, QWidget *parent);

    protected slots:
        void buttonClicked();
        void actionChanged();
};


class UBActionPaletteButton : public QToolButton
{
    Q_OBJECT

    public:
        UBActionPaletteButton(QAction* action, QWidget * parent = nullptr);
        ~UBActionPaletteButton() override;

    signals:
        void doubleClicked();

    protected:
        void mouseDoubleClickEvent(QMouseEvent *event) override;
        bool hitButton(const QPoint &pos) const override;

};

#endif /* UBACTIONPALETTE_H_ */
