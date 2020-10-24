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




#ifndef UBFEATURESWIDGET_H
#define UBFEATURESWIDGET_H

#include <QWidget>
#include <QListView>
#include <QGraphicsView>
#include <QAbstractListModel>
#include <QPixmap>
#include <QVBoxLayout>
#include <QSlider>
#include <QUrl>
#include <QStyledItemDelegate>
#include <QLocale>
#include <QGraphicsLinearLayout>
#include <QStackedWidget>
#include <QDropEvent>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QTreeWidget>

#include "UBDockPaletteWidget.h"
#include "core/UBSettings.h"
#include "board/UBFeaturesController.h"
#include "api/UBWidgetUniboardAPI.h"
#include "UBFeaturesActionBar.h"
#include "UBRubberBand.h"
#include <QWebEngineView>
#include <QWebEngineSettings>

#define THUMBNAIL_WIDTH 400
#define ID_LISTVIEW 0
#define ID_PROPERTIES 1
#define ID_WEBVIEW 2

class UBListModel;

class UBFeaturesModel;
class UBFeaturesItemDelegate;
class UBFeaturesPathItemDelegate;
class UBFeaturesProxyModel;
class UBFeaturesSearchProxyModel;
class UBFeaturesPathProxyModel;
class UBFeaturesPathViewer;
class UBFeatureProperties;
class UBFeatureItemButton;
class UBFeaturesListView;
class UBFeaturesWebView;
class UBFeaturesNavigatorWidget;
class UBFeaturesMimeData;
class UBFeaturesCentralWidget;
class UBFeaturesNewFolderDialog;
class UBFeaturesProgressBar;

class UBFeaturesWidget : public UBDockPaletteWidget
{
    Q_OBJECT
public:
    UBFeaturesWidget(QWidget* parent=nullptr, const char* name="UBFeaturesWidget");
    ~UBFeaturesWidget() override;

    bool visibleInMode(eUBDockPaletteWidgetMode mode) override
    {
        return mode == eUBDockPaletteWidget_BOARD
            || mode == eUBDockPaletteWidget_DESKTOP;
    }
    UBFeaturesController * getFeaturesController() const { return controller; }
    void importImage(const QImage &image, const QString &fileName = QString());

    static const int minThumbnailSize = 20;
    static const int maxThumbnailSize = 100;
    static const int defaultThumbnailSize = 40;

    static const char *objNamePathList;
    static const char *objNameFeatureList;

public:
    int scrollbarHorisontalPadding() const { return 10;}
    int scrollbarVerticalIndent() const { return 0;}

signals:
    void sendFileNameList(const QStringList lst);

private slots:
    void onPreviewLoaded(int id, bool pSuccess, QUrl sourceUrl, QUrl originalUrl, QString pContentTypeHeader, QByteArray pData, QPointF pPos, QSize pSize, bool isBackground);
    void currentSelected( const QModelIndex & );
    void searchStarted( const QString & );
    void createNewFolder();
    void deleteElements( const UBFeaturesMimeData * );
    void addToFavorite( const UBFeaturesMimeData  *);
    void removeFromFavorite( const UBFeaturesMimeData * );
    void onDisplayMetadata( QMap<QString,QString> );
    void onAddDownloadedFileToLibrary(bool, QUrl, QString, QByteArray, QString pTitle);
    void addElementsToFavorite();
    void removeElementsFromFavorite();
    void deleteSelectedElements();
    void rescanModel();
    void lockIt(bool pLock);

private:
    void switchToListView();
    void switchToProperties();
    void switchToWebView();

private:
    UBFeaturesController *controller;
    UBFeaturesListView *pathListView;
    QVBoxLayout *layout;
    UBFeaturesActionBar *mActionBar;
    UBDownloadHttpFile* imageGatherer;
    UBFeaturesCentralWidget *centralWidget;
};


class UBFeaturesMimeData : public QMimeData
{
    Q_OBJECT

public:
    QStringList formats() const override;
    QList<UBFeature> features() const {return mFeatures;}
    void setFeatures(const QList<UBFeature> &fList) {mFeatures = fList;}

private:
    QList<UBFeature> mFeatures;
};


class UBFeaturesListView : public QListView
{
    Q_OBJECT

public:
    UBFeaturesListView( QWidget* parent=nullptr, const char* name="UBFeaturesListView" );
    ~UBFeaturesListView() override {;}

protected:
    void dragEnterEvent( QDragEnterEvent *event ) override;
    void dropEvent( QDropEvent *event ) override;
    void dragMoveEvent( QDragMoveEvent *event ) override;

private slots:
    void thumbnailSizeChanged(int);

};


// class created to have the same style for slider and QListView itself
class UBFeaturesNavigatorWidget: public QWidget
{
    Q_OBJECT

public:
    UBFeaturesNavigatorWidget(QWidget *parent, const char* name = "");
    UBFeaturesListView *listView() {return mListView;}
    void setSliderPosition(int pValue);

private:
    UBFeaturesListView *mListView;
    QSlider *mListSlider;

};

class UBFeaturesCentralWidget : public QWidget
{
  Q_OBJECT

public:
    enum StackElement{
        MainList = 0,
        FeaturePropertiesList,
        FeaturesWebView
    };

    enum AddWidget {
        NewFolderDialog = 0,
        ProgressBarWidget
    };

    enum AddWidgetState {
        NonModal = 0,
        Modal
    };

    UBFeaturesCentralWidget(QWidget *parent = nullptr);
    void setSliderPosition(int pValue) {mNavigator->setSliderPosition(pValue);}

    UBFeaturesListView *listView() {return mNavigator->listView();}
    void showElement(const UBFeature &feature, StackElement pView);
    void switchTo(StackElement pView);
    void setPropertiesPixmap(const QPixmap &pix);
    void setPropertiesThumbnail(const QPixmap &pix);
    StackElement currentView() const {return static_cast<StackElement>(mStackedWidget->currentIndex());}
    UBFeature getCurElementFromProperties();

    void setLockedExcludingAdditional(bool pLock);

    QStackedWidget *mStackedWidget;
    UBFeaturesNavigatorWidget *mNavigator;
    UBFeatureProperties *mFeatureProperties;
    UBFeaturesWebView *webView;

    QStackedWidget *mAdditionalDataContainer;

signals:
    void lockMainWidget(bool pLock);
    void createNewFolderSignal(QString pStr);
    void sendFileNameList(const QStringList lst);

//    progressbar widget related signals
    void maxFilesCountEvaluated(int pValue);
    void increaseStatusBarValue();
    void scanCategory(const QString &);
    void scanPath(const QString &);

public slots:
    void showAdditionalData(AddWidget pWidgetType, AddWidgetState pState = NonModal);

private slots:
    void createNewFolderSlot(QString pStr);
    void hideAdditionalData();

    void scanStarted();
    void scanFinished();
};

class UBFeaturesNewFolderDialog : public QWidget
{
    Q_OBJECT

public:
    UBFeaturesNewFolderDialog(QWidget *parent = nullptr);
    void setRegexp(const QRegExp pRegExp);
    bool validString(const QString &pStr);

signals:
    void createNewFolder(QString str);
    void closeDialog();

private slots:
    void accept();
    void reject();
    void setFileNameList(const QStringList &pLst);
    void reactOnTextChanged(const QString &pStr);

private:
    QLineEdit *mLineEdit;
    QRegExpValidator *mValidator;
    QStringList mFileNameList;
    QPushButton *acceptButton;
    const QString acceptText;
    const QString cancelText;
    const QString labelText;
};

class UBFeaturesProgressInfo: public QWidget {
    Q_OBJECT

public:
    UBFeaturesProgressInfo(QWidget *parent = nullptr);

private slots:
    void setCommmonInfoText(const QString &str);
    void setDetailedInfoText(const QString &str);
    void setProgressMin(int pValue);
    void setProgressMax(int pValue);
    void increaseProgressValue();
    void sendFeature(UBFeature pFeature);


private:
    QProgressBar *mProgressBar;
    QLabel *mCommonInfoLabel;
    QLabel *mDetailedInfoLabel;
};

class UBFeaturesWebView : public QWidget
{
    Q_OBJECT
public:
    UBFeaturesWebView(QWidget* parent = nullptr, const char* name = "UBFeaturesWebView");
    ~UBFeaturesWebView() override;

    void showElement(const UBFeature &elem);

private slots:
    void onLoadFinished(bool ok);

private:
    QWebEngineView* mpView;
    QWebEngineSettings* mpWebSettings;
    QVBoxLayout* mpLayout;
    UBWidgetUniboardAPI* mpSankoreAPI;
};

class UBFeatureProperties : public QWidget
{
    Q_OBJECT
public:
    UBFeatureProperties(QWidget* parent=nullptr, const char* name="UBFeatureProperties");
    ~UBFeatureProperties() override;

    void showElement(const UBFeature &elem);
    UBFeature getCurrentElement() const;
    void setOrigPixmap(const QPixmap &pix);
    void setThumbnail(const QPixmap &pix);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;

private slots:
    void onAddToPage();
    void onAddToLib();
    void onSetAsBackground();
    //void onBack();

private:
    void populateMetadata();
    void adaptSize();

    QVBoxLayout* mpLayout;
    QHBoxLayout* mpButtonLayout;
    UBFeatureItemButton *mpAddPageButton;
    UBFeatureItemButton *mpAddToLibButton;
    UBFeatureItemButton *mpSetAsBackgroundButton;
    QLabel* mpObjInfoLabel;
    QTreeWidget* mpObjInfos;
    QLabel* mpThumbnail;
    QPixmap* mpOrigPixmap;
    int maxThumbHeight;
    UBFeature *mpElement;
    QTreeWidgetItem* mpItem;
};

class UBFeatureItemButton : public QPushButton
{
public:
    UBFeatureItemButton(QWidget* parent=nullptr, const char* name="UBFeatureItemButton");
    ~UBFeatureItemButton() override;
};

class UBFeaturesModel : public QAbstractListModel
{
    Q_OBJECT

signals:
    void dataRestructured();

public:
    UBFeaturesModel(QList<UBFeature> *pFeaturesList, QObject *parent = nullptr) : QAbstractListModel(parent), featuresList(pFeaturesList) {;}
    ~UBFeaturesModel() override{;}

    void deleteFavoriteItem( const QString &path );
    void deleteItem( const QString &path );
    void deleteItem(const UBFeature &feature);

    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const override;
    QMimeData *mimeData( const QModelIndexList &indexes ) const override;
    QStringList mimeTypes() const override;
    int rowCount( const QModelIndex &parent ) const override;
    Qt::ItemFlags flags( const QModelIndex &index ) const override;
    bool dropMimeData(const QMimeData *mimeData, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    //bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    //bool insertRow(int row, const QModelIndex &parent = QModelIndex());

    void moveData(const UBFeature &source, const UBFeature &destination, Qt::DropAction action, bool deleteManualy = false);
    Qt::DropActions supportedDropActions() const override { return Qt::MoveAction | Qt::CopyAction; }
    Qt::DropActions supportedDragActions() const override { return Qt::MoveAction | Qt::CopyAction; }

//    void setFeaturesList(QList <UBFeature> *flist ) { featuresList = flist; }

public slots:
    void addItem( const UBFeature &item );

private:
    QList <UBFeature> *featuresList;
};

class UBFeaturesProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    UBFeaturesProxyModel(QObject *parent = nullptr) : QSortFilterProxyModel(parent) {;}
    ~UBFeaturesProxyModel() override {}
protected:
    bool filterAcceptsRow ( int sourceRow, const QModelIndex & sourceParent ) const override;
};

class UBFeaturesSearchProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    UBFeaturesSearchProxyModel(QObject *parent = nullptr) : QSortFilterProxyModel(parent), mFilterPrefix() {;}
    ~UBFeaturesSearchProxyModel() override {}
    void setFilterPrefix(const QString &newPrefix) {mFilterPrefix = newPrefix;}
protected:
    bool filterAcceptsRow ( int sourceRow, const QModelIndex & sourceParent ) const override;
private:
    QString mFilterPrefix;
};

class UBFeaturesPathProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    UBFeaturesPathProxyModel(QObject *parent = nullptr) : QSortFilterProxyModel(parent) {;}
    ~UBFeaturesPathProxyModel() override {}
    void setPath( const QString &p ) { path = p; }
protected:
    bool filterAcceptsRow ( int sourceRow, const QModelIndex & sourceParent ) const override;
private:
    QString path;
};

class UBFeaturesItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    UBFeaturesItemDelegate(QObject *parent = nullptr, const QListView *lw = nullptr) : QStyledItemDelegate(parent) { listView = lw; }
    ~UBFeaturesItemDelegate() override {}
    //UBFeaturesItemDelegate(const QListView *lw = 0) { listView = lw; };
    //void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    //QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QString    displayText ( const QVariant & value, const QLocale & locale ) const override;
private:
    const QListView *listView;
};

class UBFeaturesPathItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    UBFeaturesPathItemDelegate(QObject *parent = nullptr);
    ~UBFeaturesPathItemDelegate() override;
    QString    displayText ( const QVariant & value, const QLocale & locale ) const override;
    void paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
private:
    QPixmap *arrowPixmap;
};


#endif // UBFEATURESWIDGET_H
