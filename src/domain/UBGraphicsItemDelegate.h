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




#ifndef UBGRAPHICSITEMDELEGATE_H_
#define UBGRAPHICSITEMDELEGATE_H_

#define UB_FREE_CONTROL(Object, Scene) \
    if (Object) {                      \
        if (Scene) {                   \
            Scene->removeItem(Object); \
        }                              \
        Object->deleteLater();         \
        Object = 0;                    \
    }                                  \

#include <QtGui>
#include <QtSvg>
#include <QMimeData>
#include <QGraphicsVideoItem>

#include "core/UB.h"
#include "core/UBSettings.h"

class QGraphicsSceneMouseEvent;
class QGraphicsItem;
class UBGraphicsScene;
class UBGraphicsProxyWidget;
class UBGraphicsDelegateFrame;
class UBGraphicsWidgetItem;
class UBGraphicsMediaItem;

class DelegateButton: public QGraphicsSvgItem
{
    Q_OBJECT

    public:
        DelegateButton(const QString & fileName, QGraphicsItem* pDelegated, QGraphicsItem * parent = nullptr, Qt::WindowFrameSection section = Qt::TopLeftSection);

        ~DelegateButton() override;

        enum { Type = UBGraphicsItemType::DelegateButtonType };
        int type() const override { return Type; }

        void setTransparentToMouseEvent(bool tr)
        {
            mIsTransparentToMouseEvent = tr;
        }

        void setFileName(const QString & fileName);

        void setShowProgressIndicator(bool pShow) {mShowProgressIndicator = pShow;}
        bool testShowProgresIndicator() const {return mShowProgressIndicator;}

        void setSection(Qt::WindowFrameSection section) {mButtonAlignmentSection =  section;}
        Qt::WindowFrameSection getSection() const {return mButtonAlignmentSection;}

    protected:

        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
        void timerEvent(QTimerEvent *event) override;

        void modified();

private slots:
        void startShowProgress();

    private:

        QGraphicsItem* mDelegated;

        QTime mPressedTime;
        bool mIsTransparentToMouseEvent;
        bool mIsPressed;
        int mProgressTimerId;
        int mPressProgres;
        bool mShowProgressIndicator;
        Qt::WindowFrameSection mButtonAlignmentSection;

    signals:
        void clicked (bool checked = false);
        void longClicked();

};

/*
    Code of this class is copied from QT QLCDNumber class sources
    See src\gui\widgets\qlcdnumber.cpp for original code
*/
class MediaTimer: public QGraphicsRectItem
{
public:
    MediaTimer(QGraphicsItem * parent = nullptr);
    ~MediaTimer() override;

    void positionHandles();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                QWidget *widget) override;

    void display(const QString &str);
    void setNumDigits(int nDigits);

private:

    static const char* getSegments(char);
    void drawString(const QString& s, QPainter &, QBitArray * = nullptr, bool = true);
    void drawDigit(const QPoint &, QPainter &, int, char, char = ' ');
    void drawSegment(const QPoint &, char, QPainter &, int, bool = false);
    void addPoint(QPolygon&, const QPoint&);
    void internalSetString(const QString& s);

    static char segments [][8];

    int ndigits;
    QString digitStr;
    QBitArray points;
    double val;

    uint shadow : 1;
    uint smallPoint : 1;

    int digitSpace;
    int xSegLen;
    int ySegLen;
    int segLen;
    int xAdvance;
    int xOffset;
    int yOffset;
};

class DelegateMediaControl: public QObject, public QGraphicsRectItem
{
    Q_OBJECT

    public:

        DelegateMediaControl(UBGraphicsMediaItem* pDelegated, QGraphicsItem * parent = nullptr);

        ~DelegateMediaControl() override
        {
            // NOOP
        }

        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                QWidget *widget) override;

        QPainterPath shape() const override;

        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
        virtual void update();

        void positionHandles();
        void updateTicker(qint64 time);
        void totalTimeChanged(qint64 newTotalTime);

    signals:
        void used();

    protected:
        void seekToMousePos(QPointF mousePos);

        UBGraphicsMediaItem* mDelegate;
        bool mDisplayCurrentTime;

        qint64 mCurrentTimeInMs;
        qint64 mTotalTimeInMs;

    private:
        int mStartWidth;
        int mSeecAreaBorderHeight;

        QRectF mSeecArea;
        QRectF mLCDTimerArea;

        MediaTimer *lcdTimer;

        QString mDisplayFormat;
};

class UBGraphicsToolBarItem : public QGraphicsRectItem, public QObject
{
    public:
        UBGraphicsToolBarItem(QGraphicsItem * parent = nullptr);
        ~UBGraphicsToolBarItem() override {;}

        bool isVisibleOnBoard() const { return mVisible; }
        void setVisibleOnBoard(bool visible) { mVisible = visible; }
        bool isShifting() const { return mShifting; }
        void setShifting(bool shifting) { mShifting = shifting; }
        QList<QGraphicsItem*> itemsOnToolBar() const { return mItemsOnToolBar; }
        void setItemsOnToolBar(QList<QGraphicsItem*> itemsOnToolBar) { mItemsOnToolBar = itemsOnToolBar;}
        int minWidth() { return mMinWidth; }
        void positionHandles();
        void update();
        int getElementsPadding(){return mElementsPadding;}

    private:
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                QWidget *widget) override;

    private:
        bool mShifting;
        bool mVisible;
        int mMinWidth;
        int mInitialHeight;
        int mElementsPadding;
        QList<QGraphicsItem*> mItemsOnToolBar;
};

class UBGraphicsItemDelegate : public QObject
{
    Q_OBJECT

    public:
    UBGraphicsItemDelegate(QGraphicsItem* pDelegated, QObject * parent = nullptr, UBGraphicsFlags fls = nullptr);

        ~UBGraphicsItemDelegate() override;

        virtual void createControls();
        virtual void freeControls();
        virtual void showControls();
        virtual bool controlsExist() const;

        virtual bool mousePressEvent(QGraphicsSceneMouseEvent *event);
        virtual bool mouseMoveEvent(QGraphicsSceneMouseEvent *event);
        virtual bool mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
        virtual bool wheelEvent(QGraphicsSceneWheelEvent *event);

        virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
        virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

        virtual bool keyPressEvent(QKeyEvent *event);
        virtual bool keyReleaseEvent(QKeyEvent *event);

        virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change,
                const QVariant &value);
        virtual UBGraphicsScene *castUBGraphicsScene();
        virtual void postpaint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);


        void printMessage(const QString &mess) {qDebug() << mess;}

        QGraphicsItem* delegated();

        virtual void positionHandles();
        void setZOrderButtonsVisible(bool visible);

        void startUndoStep();
        void commitUndoStep();

        UBGraphicsDelegateFrame* frame() {return mFrame;}

        bool isLocked() const;

        QMimeData* mimeData(){ return mMimeData; }
        void setMimeData(QMimeData* mimeData);
        void setDragPixmap(const QPixmap &pix) {mDragPixmap = pix;}

        void setLocked(bool pLocked);
        void setButtonsVisible(bool visible);

        UBGraphicsToolBarItem* getToolBarItem() const { return mToolBarItem; }

        qreal antiScaleRatio() const { return mAntiScaleRatio; }
        virtual void update() {positionHandles();}

        UBGraphicsFlags ubflags() const {return mFlags;}
        bool testUBFlags(UBGraphicsFlags pf) const {return mFlags & pf;}
        void setUBFlags(UBGraphicsFlags pf);
        void setUBFlag(UBGraphicsFlags pf, bool set = true);

        virtual void showToolBar(bool autohide = true) {Q_UNUSED(autohide);}

    signals:
        void showOnDisplayChanged(bool shown);
        void lockChanged(bool locked);

    public slots:
        virtual void remove(bool canUndo = true);
        void showMenu();

        virtual void showHide(bool show);
        virtual void lock(bool lock);
        virtual void duplicate();

        void increaseZLevelUp();
        void increaseZLevelDown();
        void increaseZlevelTop();
        void increaseZlevelBottom();

        void onZoomChanged();

    protected:
        virtual void buildButtons();
        virtual void freeButtons();
        virtual void decorateMenu(QMenu *menu);
        virtual void updateMenuActionState();

        void showHideRecurs(const QVariant &pShow, QGraphicsItem *pItem);
        void setLockedRecurs(const QVariant &pLock, QGraphicsItem *pItem);

        QList<DelegateButton*> buttons() {return mButtons;}
        QGraphicsItem* mDelegated;

        //buttons from the top left section of delegate frame
        DelegateButton* mDeleteButton;
        DelegateButton* mDuplicateButton;
        DelegateButton* mMenuButton;

        //buttons from the bottom left section of delegate frame
        DelegateButton *mZOrderUpButton;
        DelegateButton *mZOrderDownButton;

        QMenu* mMenu;

        QAction* mLockAction;
        QAction* mShowOnDisplayAction;
        QAction* mSetAsBackgroundAction;
        QAction* mGotoContentSourceAction;

        UBGraphicsDelegateFrame* mFrame;
        qreal mFrameWidth;
        qreal mAntiScaleRatio;

        QList<DelegateButton*> mButtons;
        QList<DelegateButton*> mToolBarButtons;
        UBGraphicsToolBarItem* mToolBarItem;

protected slots:
        virtual void setAsBackground();
        virtual void gotoContentSource();

private:
        void updateFrame();
        void updateButtons(bool showUpdated = false);

        QPointF mOffset;
        QTransform mPreviousTransform;
        QPointF mPreviousPosition;
        QPointF mDragStartPosition;
        qreal mPreviousZValue;
        QSizeF mPreviousSize;
        QMimeData* mMimeData;
        QPixmap mDragPixmap;

        bool mMoved;
        UBGraphicsFlags mFlags;
};


#endif /* UBGRAPHICSITEMDELEGATE_H_ */
