#include "TreeViewWidget.h"
#include <QMenu>
#include <QContextMenuEvent>
#include <QFont>
#include <QDebug>
#include <QEvent>
#include <QMessageBox>

// Constructor
TreeViewWidget::TreeViewWidget(QWidget *parent)
    : QTreeView(parent),
    model(new QStandardItemModel(this)),
    rootItem(nullptr)
{
    // Set up the model with a header label.
    model->setHorizontalHeaderLabels(QStringList() << "Name");
    setModel(model);

    // Enable inline editing on double-click or via the edit key.
    setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);

    // Connect the model's itemChanged signal to our validation slot.
    connect(model, &QStandardItemModel::itemChanged,
            this, &TreeViewWidget::onItemChanged);
}

// Sets or updates the root folder name.
void TreeViewWidget::setRootFolderName(const QString &name)
{
    if (!rootItem) {
        rootItem = new QStandardItem(name);
        // Disable editing on the root item.
        rootItem->setFlags(rootItem->flags() & ~Qt::ItemIsEditable);
        model->appendRow(rootItem);
    } else {
        rootItem->setText(name);
    }
}

// Adds a new folder or file under the given parent.
void TreeViewWidget::addNewItem(QStandardItem *parentItem, bool isFolder)
{
    if (!parentItem) {
        qFatal("TreeViewWidget::addNewItem() called with a null parentItem");
    }

    QString defaultName;
    if (isFolder) {
        // Determine a unique default name for folders, e.g., NewFolder, NewFolder_2, etc.
        QString baseName = "NewFolder";
        defaultName = baseName;
        int suffix = 2;
        bool duplicateFound;
        do {
            duplicateFound = false;
            const int rowCount = parentItem->rowCount();
            for (int i = 0; i < rowCount; ++i) {
                QStandardItem *sibling = parentItem->child(i);
                if (sibling->text().trimmed().compare(defaultName, Qt::CaseInsensitive) == 0) {
                    duplicateFound = true;
                    break;
                }
            }
            if (duplicateFound) {
                defaultName = baseName + "_" + QString::number(suffix++);
            }
        } while (duplicateFound);
    } else {
        defaultName = "NewFile";
    }

    // Create the new item with the determined default name.
    QStandardItem *newItem = new QStandardItem(defaultName);
    // Mark it as new so that its name will be validated after editing.
    newItem->setData(true, NewItemRole);

    if (isFolder) {
        // For folders, use a bold font.
        QFont font = newItem->font();
        font.setBold(true);
        newItem->setFont(font);
    } else {
        // Optionally set a file icon or similar handling for files.
    }

    // Append the new item to the parent item.
    parentItem->appendRow(newItem);
    expand(model->indexFromItem(parentItem));

    // Begin inline editing so the user can change the default name.
    QModelIndex newIndex = model->indexFromItem(newItem);
    edit(newIndex, QAbstractItemView::DoubleClicked);
}

// Overridden edit() method to store the previous text before editing starts.
// Note: Do not include the default argument for event in the definition.
bool TreeViewWidget::edit(const QModelIndex &index,
                          QAbstractItemView::EditTrigger trigger,
                          QEvent *event)
{
    QStandardItem *item = model->itemFromIndex(index);
    if (item) {
        // Save the current (valid) text to revert to if needed.
        item->setData(item->text(), PreviousNameRole);
    }
    return QTreeView::edit(index, trigger, event);
}

// Overridden context menu event to provide a right-click menu.
void TreeViewWidget::contextMenuEvent(QContextMenuEvent *event)
{
    // Get the item under the mouse, or default to the root.
    QModelIndex index = indexAt(event->pos());
    QStandardItem *selectedItem = (index.isValid()) ? model->itemFromIndex(index) : rootItem;

    // Create the context menu.
    QMenu contextMenu(this);
    QAction *addFolderAction = contextMenu.addAction("Add Folder");
    QAction *addFileAction   = contextMenu.addAction("Add File");
    QAction *removeAction    = contextMenu.addAction("Remove");

    // Disable the remove action if the selected item is the top-level (root) item.
    if (!selectedItem || !selectedItem->parent())
        removeAction->setEnabled(false);

    // Execute the menu.
    QAction *selectedAction = contextMenu.exec(event->globalPos());
    if (!selectedAction)
        return;

    if (selectedAction == addFolderAction) {
        addNewItem(selectedItem, true);
    } else if (selectedAction == addFileAction) {
        addNewItem(selectedItem, false);
    } else if (selectedAction == removeAction) {
        // If the item has children, confirm deletion.
        if (selectedItem->rowCount() > 0) {
            int ret = QMessageBox::warning(this,
                                           tr("Confirm Delete"),
                                           tr("Are you sure you want to delete the folder and its subdirectories?"),
                                           QMessageBox::Yes | QMessageBox::No,
                                           QMessageBox::No);
            if(ret != QMessageBox::Yes)
                return;
        }
        // Remove the selected item.
        QStandardItem *parentItem = selectedItem->parent();
        if (parentItem) {
            parentItem->removeRow(selectedItem->row());
        }
    }
}

// Slot to validate an item when its text changes.
void TreeViewWidget::onItemChanged(QStandardItem *item)
{
    // Retrieve stored previous name.
    QVariant previousVariant = item->data(PreviousNameRole);
    if (!previousVariant.isValid()) {
        // If no previous value is stored, skip validation.
        return;
    }

    QString oldName = previousVariant.toString();
    QString newName = item->text().trimmed();

    // Use the item's parent or root if no parent.
    QStandardItem *parentItem = item->parent();
    if (!parentItem)
        parentItem = rootItem;

    bool valid = true;
    // Check that the new name is not empty.
    if (newName.isEmpty()) {
        valid = false;
    }

    // Check for duplicates among siblings.
    const int rowCount = parentItem->rowCount();
    for (int i = 0; i < rowCount; ++i) {
        QStandardItem *sibling = parentItem->child(i);
        if (sibling == item)
            continue;
        if (sibling->text().trimmed().compare(newName, Qt::CaseInsensitive) == 0) {
            valid = false;
            break;
        }
    }

    // If the new name is invalid, revert to old name.
    if (!valid) {
        qDebug() << "Invalid name detected (" << newName << "); reverting to:" << oldName;
        item->setText(oldName);
    } else {
        // Valid name: update the stored previous name.
        item->setData(newName, PreviousNameRole);
    }
}
