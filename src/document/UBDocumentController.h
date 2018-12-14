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



#ifndef UBDOCUMENTCONTROLLER_H_
#define UBDOCUMENTCONTROLLER_H_

#include <QtGui>
#include "document/UBDocumentContainer.h"
#include "core/UBApplicationController.h"
#include "core/UBApplication.h"


#include "document/UBSortFilterProxyModel.h"

namespace Ui
{
    class documents;
}

#include "gui/UBMessageWindow.h"

class UBGraphicsScene;
class QDialog;
class UBDocumentProxy;
class UBBoardController;
class UBThumbnailsScene;
class UBDocumentGroupTreeItem;
class UBDocumentProxyTreeItem;
class UBMainWindow;
class UBDocumentToolsPalette;


class UBDocumentReplaceDialog : public QDialog
{
    Q_OBJECT

public:
    UBDocumentReplaceDialog(const QString &pIncommingName, const QStringList &pFileList, QWidget *parent = nullptr, Qt::WindowFlags pFlags = nullptr);
    void setRegexp(const QRegExp pRegExp);
    bool validString(const QString &pStr);
    void setFileNameAndList(const QString &fileName, const QStringList &pLst);
    QString  labelTextWithName(const QString &documentName) const;
    QString lineEditText() const {return mLineEdit->text();}

signals:
    void createNewFolder(QString str);
    void closeDialog();

private slots:
    void accept() override;
    void reject() override;

    void reactOnTextChanged(const QString &pStr);

private:
    QLineEdit *mLineEdit;
    QRegExpValidator *mValidator;
    QStringList mFileNameList;
    QString mIncommingName;
    QPushButton *acceptButton;
    const QString acceptText;
    const QString replaceText;
    const QString cancelText;
    QLabel *mLabelText;
};


class UBDocumentTreeNode
{
public:
    friend class UBDocumentTreeModel;

    enum Type {
        Catalog = 0
        , Document
    };

    UBDocumentTreeNode(Type pType, const QString &pName, const QString &pDisplayName = QString(), UBDocumentProxy *pProxy = nullptr);
    UBDocumentTreeNode() : mType(Catalog), mParent(nullptr), mProxy(nullptr) {;}
    ~UBDocumentTreeNode();

    QList<UBDocumentTreeNode*> children() const {return mChildren;}
    UBDocumentTreeNode *parentNode() {return mParent;}
    Type nodeType() const {return mType;}
    QString nodeName() const {return mName;}
    QString displayName() const {return mDisplayName;}
    void setNodeName(const QString &str) {mName = str; mDisplayName = str;}
    void addChild(UBDocumentTreeNode *pChild);
    void insertChild(int pIndex, UBDocumentTreeNode *pChild);
    void moveChild(UBDocumentTreeNode *child, int index, UBDocumentTreeNode *newParent);
    void removeChild(int index);
    UBDocumentProxy *proxyData() const {return mProxy;}
    bool isRoot() {return !mParent;}
    bool isTopLevel()
    {
        if (mParent)
        {
            return !mParent->mParent;
        }
        else return false;
    }
    UBDocumentTreeNode *clone();
    QString dirPathInHierarchy();

    //issue 1629 - NNE - 20131105 : Add some utility methods
    bool findNode(UBDocumentTreeNode *node);
    UBDocumentTreeNode *nextSibling();
    UBDocumentTreeNode *previousSibling();
    //issue 1629 - NNE - 20131105 : END

private:
    Type mType;
    QString mName;
    QString mDisplayName;
    UBDocumentTreeNode *mParent;
    QList<UBDocumentTreeNode*> mChildren;
    QPointer<UBDocumentProxy> mProxy;
};
Q_DECLARE_METATYPE(UBDocumentTreeNode*)

class UBDocumentTreeModel : public QAbstractItemModel {
    Q_OBJECT

public:
    enum eAncestors {
        aMyDocuments
        , aUntitledDocuments
        , aModel
        , aTrash
    };

    enum eCopyMode {
        aReference
        , aContentCopy
    };

    enum eAddItemMode {
        aEnd = 0          //Add to the end of the children list
        , aDetectPosition //Detect the appropriate position (sorting)
    };

    enum eDocumentData{
        DataNode = Qt::UserRole +1,
        CreationDate,
        UpdateDate
    };

    UBDocumentTreeModel(QObject *parent = nullptr);
    ~UBDocumentTreeModel() override;

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags ( const QModelIndex & index ) const override;
    Qt::DropActions supportedDropActions() const override {return Qt::MoveAction | Qt::CopyAction;}
    QStringList mimeTypes() const override;
    QMimeData *mimeData (const QModelIndexList &indexes) const override;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;

    bool containsDocuments(const QModelIndex& index);

    QModelIndex indexForNode(UBDocumentTreeNode *pNode) const;
    QPersistentModelIndex persistentIndexForNode(UBDocumentTreeNode *pNode);
//    bool insertRow(int row, const QModelIndex &parent);

    QPersistentModelIndex copyIndexToNewParent(const QModelIndex &source, const QModelIndex &newParent, eCopyMode pMode = aReference);

    //N/C - NNE - 20140411
    void copyIndexToNewParent(const QModelIndexList &list, const QModelIndex &newParent, eCopyMode pMode);

    void moveIndex(const QModelIndex &what, const QModelIndex &destination);
    UBDocumentTreeNode *currentNode() const {return mCurrentNode;} //work around for sorting model.
    void setCurrentNode(UBDocumentTreeNode *pNode) {mCurrentNode = pNode;}
    QModelIndex currentIndex() {return indexForNode(mCurrentNode);} //index representing a current document
    QModelIndex indexForProxy(UBDocumentProxy *pSearch) const;
    void setCurrentDocument(UBDocumentProxy *pDocument);
    void setRootNode(UBDocumentTreeNode *pRoot);
    UBDocumentTreeNode *rootNode() const {return mRootNode;}
    UBDocumentProxy *proxyForIndex(const QModelIndex &pIndex) const;
    QString virtualDirForIndex(const QModelIndex &pIndex) const;
    QString virtualPathForIndex(const QModelIndex &pIndex) const;
    QStringList nodeNameList(const QModelIndex &pIndex) const;
    bool newNodeAllowed(const QModelIndex &pSelectedIndex)  const;
    QModelIndex goTo(const QString &dir);
    bool inTrash(const QModelIndex &index) const;
    bool inUntitledDocuments(const QModelIndex &index) const;
    bool isCatalog(const QModelIndex &index) const {return nodeFromIndex(index)->nodeType() == UBDocumentTreeNode::Catalog;}
    bool isDocument(const QModelIndex &index) const {return nodeFromIndex(index)->nodeType() == UBDocumentTreeNode::Document;}
    bool isToplevel(const QModelIndex &index) const {return nodeFromIndex(index) ? nodeFromIndex(index)->isTopLevel() : false;}
    bool isConstant(const QModelIndex &index) const {return isToplevel(index) || (index == mUntitledDocuments);}
    bool isOkToRename(const QModelIndex &index) const {return flags(index) & Qt::ItemIsEditable;}
    UBDocumentProxy *proxyData(const QModelIndex &index) const {return nodeFromIndex(index)->proxyData();}
    void addDocument(UBDocumentProxy *pProxyData, const QModelIndex &pParent = QModelIndex());
    void addNewDocument(UBDocumentProxy *pProxyData, const QModelIndex &pParent = QModelIndex());
    QModelIndex addCatalog(const QString &pName, const QModelIndex &pParent);
    QList<UBDocumentProxy*> newDocuments() {return mNewDocuments;}
    void markDocumentAsNew(UBDocumentProxy *pDoc) {if (indexForProxy(pDoc).isValid()) mNewDocuments << pDoc;}
    void setNewName(const QModelIndex &index, const QString &newName);
    QString adjustNameForParentIndex(const QString &pName, const QModelIndex &pIndex);

    QPersistentModelIndex myDocumentsIndex() const {return mMyDocuments;}
    QPersistentModelIndex trashIndex() const {return mTrash;}
    QPersistentModelIndex untitledDocumentsIndex() const {return mMyDocuments;}
    UBDocumentTreeNode *nodeFromIndex(const QModelIndex &pIndex) const;
    static bool nodeLessThan(const UBDocumentTreeNode *firstIndex, const UBDocumentTreeNode *secondIndex);
    void setHighLighted(const QModelIndex &newHighLighted) {mHighLighted = newHighLighted;}
    QModelIndex highLighted() {return mHighLighted;}

    //N/C - NNE - 20140407
    bool ascendingOrder() const{ return mAscendingOrder; }

    QDateTime findNodeDate(UBDocumentTreeNode *node, QString type) const;
    bool inMyDocuments(const QModelIndex &index) const;
    void moveIndexes(const QModelIndexList &source, const QModelIndex &destination);
    //N/C - NNE - 20140407 : END
    bool isDescendantOf(const QModelIndex &pPossibleDescendant, const QModelIndex &pPossibleAncestor) const;

signals:
    void indexChanged(const QModelIndex &newIndex, const QModelIndex &oldIndex);
    void currentIndexMoved(const QModelIndex &newIndex, const QModelIndex &previous); /* Be aware that when you got the signal
                                                                                       "previous" index would have allready been deleted.
                                                                                        check it for "valid" first */

private:
    UBDocumentTreeNode *mRootNode;
    UBDocumentTreeNode *mCurrentNode;

    UBDocumentTreeNode *findProxy(UBDocumentProxy *pSearch, UBDocumentTreeNode *pParent) const;
    QModelIndex pIndexForNode(const QModelIndex &parent, UBDocumentTreeNode *pNode) const;
    QModelIndex addNode(UBDocumentTreeNode *pFreeNode, const QModelIndex &pParent, eAddItemMode pMode = aDetectPosition);
    int positionForParent(UBDocumentTreeNode *pFreeNode, UBDocumentTreeNode *pParentNode);
    void fixNodeName(const QModelIndex &source, const QModelIndex &dest);
    void updateIndexNameBindings(UBDocumentTreeNode *nd);
    QPersistentModelIndex mRoot;
    QPersistentModelIndex mMyDocuments;
    QPersistentModelIndex mTrash;
    QPersistentModelIndex mUntitledDocuments;
    QList<UBDocumentProxy*> mNewDocuments;
    QModelIndex mHighLighted;

    //N/C - NNE - 20140407
    bool mAscendingOrder;

    QDateTime findCatalogUpdatedDate(UBDocumentTreeNode *node) const;
    QDateTime findCatalogCreationDate(UBDocumentTreeNode *node) const;
    //N/C - NNE - 20140407 : END
};

class UBDocumentTreeMimeData : public QMimeData
{
    Q_OBJECT

    public:
        QList<QModelIndex> indexes() const {return mIndexes;}
        void setIndexes (const QList<QModelIndex> &pIndexes) {mIndexes = pIndexes;}

    private:
        QList<QModelIndex> mIndexes;
};

class UBDocumentTreeView : public QTreeView
{
    Q_OBJECT

public:
    UBDocumentTreeView (QWidget *parent = nullptr);

    //N/C - NNE - 20140404
    QModelIndex mapIndexToSource(const QModelIndex &index);
    QModelIndexList mapIndexesToSource(const QModelIndexList &indexes);

public slots:
    void setSelectedAndExpanded(const QModelIndex &pIndex, bool pExpand = true, bool pEdit = false);
    void onModelIndexChanged(const QModelIndex &pNewIndex, const QModelIndex &pOldIndex);
    void hSliderRangeChanged(int min, int max);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void paintEvent(QPaintEvent *event) override;


    UBDocumentTreeModel *fullModel() {return qobject_cast<UBDocumentTreeModel*>(model());}
    void rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end) override;

private:
    bool isAcceptable(const QModelIndex &dragIndex, const QModelIndex &atIndex);
    Qt::DropAction acceptableAction(const QModelIndex &dragIndex, const QModelIndex &atIndex);
    void updateIndexEnvirons(const QModelIndex &index);
};

class UBDocumentTreeItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    UBDocumentTreeItemDelegate(QObject *parent = nullptr);

private slots:
    void commitAndCloseEditor();
    void processChangedText(const QString &str) const;
    bool validateString(const QString &str) const;

protected:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex &index) const override;

private:
    mutable QStringList mExistingFileNames;
};

class UBDocumentController : public UBDocumentContainer
{
    Q_OBJECT

    public:

    enum DeletionType {
        MoveToTrash = 0
        , CompleteDelete
        , EmptyFolder
        , EmptyTrash
        , DeletePage
        , NoDeletion
    };

    enum LastSelectedElementType
    {
        None = 0, Folder, Document, Page
    };

    enum SortOrder
    {
        ASC = 0,
        DESC
    };

    enum SortKind
    {
        CreationDate,
        UpdateDate,
        Alphabetical
    };

        UBDocumentController(UBMainWindow* mainWindow);
        ~UBDocumentController() override;

        void closing();
        QWidget* controlView();
        UBDocumentProxyTreeItem* findDocument(UBDocumentProxy* proxy);
        bool addFileToDocument(UBDocumentProxy* document);
        void deletePages(QList<QGraphicsItem*> itemsToDelete);
        int getSelectedItemIndex();


        bool pageCanBeMovedUp(int page);
        bool pageCanBeMovedDown(int page);
        bool pageCanBeDuplicated(int page);
        bool pageCanBeDeleted(int page);
        QString documentTrashGroupName(){ return mDocumentTrashGroupName;}
        QString defaultDocumentGroupName(){ return mDefaultDocumentGroupName;}

        void setDocument(UBDocumentProxy *document, bool forceReload = false);
        QModelIndex firstSelectedTreeIndex();
        UBDocumentProxy *firstSelectedTreeProxy();
        inline DeletionType deletionTypeForSelection(LastSelectedElementType pTypeSelection
                                                     , const QModelIndex &selectedIndex
                                                     , UBDocumentTreeModel *docModel) const;
        bool firstAndOnlySceneSelected() const;
        QWidget *mainWidget() const {return mDocumentWidget;}

        //issue 1629 - NNE - 20131212
        /**
          * Check if the current view will be deleted. If it' true,
          * this function assures a view will be always selected, even if
          * no view is availaible by creating a new one.
          *
          * \param index The index that will be deleted.
          * \param docModel The tree model which represents the organisation of the document hierarchy
          *
          * \return True if the index passed in argument was the current view, false otherwise.
          */
        void moveToTrash(QModelIndex &index, UBDocumentTreeModel* docModel);

        void deleteDocumentsInFolderOlderThan(const QModelIndex &index, const int days);
        void deleteEmptyFolders(const QModelIndex &index);

        QModelIndex mapIndexToSource(const QModelIndex &index);
        QModelIndexList mapIndexesToSource(const QModelIndexList &indexes);

        void sortDocuments(int kind, int order);

        void moveIndexesToTrash(const QModelIndexList &list, UBDocumentTreeModel *docModel);
        QModelIndex findPreviousSiblingNotSelected(const QModelIndex &index, QItemSelectionModel *selectionModel);
        QModelIndex findNextSiblingNotSelected(const QModelIndex &index, QItemSelectionModel *selectionModel);
        bool parentIsSelected(const QModelIndex& child, QItemSelectionModel *selectionModel);

    signals:
        void exportDone();
        void reorderDocumentsRequested();

    public slots:
        void createNewDocument();
        void refreshDateColumns();
        void reorderDocuments();
        //issue 1629 - NNE - 20131105
        void createNewDocumentInUntitledFolder();

        void createNewDocumentGroup();
        void deleteSelectedItem();
        void deleteSingleItem(QModelIndex index, UBDocumentTreeModel *docModel);
        void deleteMultipleItems(QModelIndexList indexes, UBDocumentTreeModel *docModel);
        void emptyFolder(const QModelIndex &index, DeletionType pDeletionType = MoveToTrash);
        void deleteIndexAndAssociatedData(const QModelIndex &pIndex);
        void renameSelectedItem();
        void openSelectedItem();
        void duplicateSelectedItem();
        void importFile();
        void moveSceneToIndex(UBDocumentProxy* proxy, int source, int target);
        void selectDocument(UBDocumentProxy* proxy, bool setAsCurrentDocument = true, const bool onImport = false, const bool editMode = false);
        void show();
        void hide();
        void showMessage(const QString& message, bool showSpinningWheel = false);
        void hideMessage();
        void toggleDocumentToolsPalette();
        void cut();
        void copy();
        void paste();
        void focusChanged(QWidget *old, QWidget *current);
        void updateActions();
        void updateExportSubActions(const QModelIndex &selectedIndex);
        void currentIndexMoved(const QModelIndex &newIndex, const QModelIndex &PreviousIndex);

        //N/C - NNE - 20140403
        void onSortKindChanged(int index);
        void onSortOrderChanged(bool order);
        void onSplitterMoved(int size, int index);
        void collapseAll();
        void expandAll();

protected:
        virtual void setupViews();
        virtual void setupToolbar();
        void setupPalettes();
        bool isOKToOpenDocument(UBDocumentProxy* proxy);
        UBDocumentProxy* selectedDocumentProxy();
        QList<UBDocumentProxy*> selectedProxies();
        QModelIndexList selectedTreeIndexes();
        UBDocumentProxyTreeItem* selectedDocumentProxyTreeItem();
        UBDocumentGroupTreeItem* selectedDocumentGroupTreeItem();
        QStringList allGroupNames();

        LastSelectedElementType mSelectionType;

    private:
        QWidget *mParentWidget;
        UBBoardController *mBoardController;
        Ui::documents* mDocumentUI;
        UBMainWindow* mMainWindow;
        QWidget *mDocumentWidget;
        QPointer<UBMessageWindow> mMessageWindow;
        QAction* mAddFolderOfImagesAction;
        QAction* mAddFileToDocumentAction;
        QAction* mAddImagesAction;
        bool mIsClosing;
        UBDocumentToolsPalette *mToolsPalette;
        bool mToolsPalettePositionned;
        UBDocumentGroupTreeItem* mTrashTi;

        void moveDocumentToTrash(UBDocumentGroupTreeItem* groupTi, UBDocumentProxyTreeItem *proxyTi);
        void moveFolderToTrash(UBDocumentGroupTreeItem* groupTi);
        QString mDocumentTrashGroupName;
        QString mDefaultDocumentGroupName;

        UBDocumentProxy *mCurrentTreeDocument;
        bool mCurrentIndexMoved;

        UBSortFilterProxyModel *mSortFilterProxyModel;

    public slots:
        void TreeViewSelectionChanged(const QModelIndex &current, const QModelIndex &previous);
        void TreeViewSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

   private slots:
        void documentZoomSliderValueChanged (int value);
        void itemSelectionChanged(LastSelectedElementType newSelection);
        void exportDocument();
        void exportDocumentSet();

        void thumbnailViewResized();
        void pageSelectionChanged();

        void documentSceneChanged(UBDocumentProxy* proxy, int pSceneIndex);

        void thumbnailPageDoubleClicked(QGraphicsItem* item, int index);
        void pageClicked(QGraphicsItem* item, int index);
        void addToDocument();

        void addFolderOfImages();
        void addFileToDocument();
        void addImages();
        void refreshDocumentThumbnailsView(UBDocumentContainer* source);
};



#endif /* UBDOCUMENTCONTROLLER_H_ */
