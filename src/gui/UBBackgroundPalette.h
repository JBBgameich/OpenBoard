#ifndef UBBACKGROUNDPALETTE_H
#define UBBACKGROUNDPALETTE_H

#include "gui/UBActionPalette.h"
#include "core/UBApplication.h"
#include "board/UBBoardController.h"
#include "domain/UBGraphicsScene.h"

class UBBackgroundPalette : public UBActionPalette
{
    Q_OBJECT

    public:

        UBBackgroundPalette(QList<QAction*> actions, QWidget* parent = 0);
        UBBackgroundPalette(QWidget* parent = 0);

        void addAction(QAction *action) override;
        void setActions(QList<QAction *> actions) override;
        void clearLayout() override;


    public slots:
        void showEvent(QShowEvent* event) override;
        void backgroundChanged();
        void refresh();

    protected slots:
        void sliderValueChanged(int value);
        void defaultBackgroundGridSize();

    protected:
        void updateLayout() override;
        void init();


        QVBoxLayout* mVLayout;
        QHBoxLayout* mTopLayout;
        QHBoxLayout* mBottomLayout;

        QSlider* mSlider;
        QLabel* mSliderLabel;
        UBActionPaletteButton* mResetDefaultGridSizeButton;


};

#endif // UBBACKGROUNDPALETTE_H
