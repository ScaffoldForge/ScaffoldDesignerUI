// TreeViewWidget.h

#ifndef TREEVIEWWIDGET_H
#define TREEVIEWWIDGET_H

#include <QTreeView>
#include <QStandardItemModel>

/** Custom data roles used for tagging items in the tree. */
constexpr int NewItemRole        = Qt::UserRole + 1; ///< Marks newly added items pending validation.
constexpr int PreviousNameRole   = Qt::UserRole + 2; ///< Stores the last known valid name of an item.
constexpr int IsFolderRole       = Qt::UserRole + 3; ///< Distinguishes folders/libraries from files.
constexpr int DependenciesRole   = Qt::UserRole + 4; ///< Stores a QStringList of dependencies (for libraries).

/**
 * @brief TreeViewWidget is a QTreeView subclass for interactive tree structures.
 *
 * This widget allows users to add folders, files, and libraries via context menus or external controls.
 * Each item is validated to prevent duplicates and empty names. Library items additionally store
 * dependencies selected via a configuration dialog.
 */
class TreeViewWidget : public QTreeView {
    Q_OBJECT

public:
    /**
     * @brief Constructs a TreeViewWidget.
     * @param parent The parent widget.
     */
    explicit TreeViewWidget(QWidget *parent = nullptr);

    /**
     * @brief Sets or updates the name of the root folder.
     *        Creates the root item if it doesn't exist.
     * @param name The desired root name.
     */
    void setRootFolderName(const QString &name);

    /**
     * @brief Adds a new folder or file under the specified parent item.
     * @param parentItem The parent item.
     * @param isFolder True to add a folder, false to add a file.
     */
    void addNewItem(QStandardItem *parentItem, bool isFolder);

    /**
     * @brief Launches a configuration dialog and adds a new library item under the given parent.
     * @param parentItem The parent item (must be the root).
     */
    void addLibrary(QStandardItem *parentItem);

    /**
     * @brief Returns a pointer to the root item in the tree.
     * @return The root QStandardItem.
     */
    QStandardItem* getRootItem() const;

    /**
     * @brief Returns the internal item model used by the tree.
     * @return A pointer to the QStandardItemModel.
     */
    QStandardItemModel* getModel() const;

protected:
    /**
     * @brief Reimplements the context menu event handler to provide options like Add/Remove.
     * @param event The context menu event.
     */
    void contextMenuEvent(QContextMenuEvent *event) override;

    /**
     * @brief Reimplements inline editing with prior-value storage for validation fallback.
     * @param index The index being edited.
     * @param trigger The edit trigger type.
     * @param event The edit event (optional).
     * @return True if editing was accepted.
     */
    bool edit(const QModelIndex &index,
              QAbstractItemView::EditTrigger trigger,
              QEvent *event = nullptr) override;

private slots:
    /**
     * @brief Slot triggered when an item’s text changes. Performs validation.
     * @param item The modified item.
     */
    void onItemChanged(QStandardItem *item);

private:
    QStandardItem *rootItem;    ///< The top-level root node in the tree.
    QStandardItemModel *model;  ///< The underlying model.
};

#endif // TREEVIEWWIDGET_H
