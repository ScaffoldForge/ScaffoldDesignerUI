// TreeViewWidget.cpp

#include "TreeViewWidget.h"
#include "ProjectConfigurator.h"

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
    // Set up column headers
    model->setHorizontalHeaderLabels(QStringList() << "Name");
    setModel(model);

    // Show expand/collapse arrows
    setItemsExpandable(true);
    setRootIsDecorated(true);

    // Enable inline editing
    setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);

    // Connect changes in item names to validation logic
    connect(model, &QStandardItemModel::itemChanged,
            this, &TreeViewWidget::onItemChanged);
}

// Sets the root folder name (or updates existing root label)
void TreeViewWidget::setRootFolderName(const QString &name)
{
    if (!rootItem) {
        rootItem = new QStandardItem(name);
        rootItem->setFlags(rootItem->flags() & ~Qt::ItemIsEditable);
        rootItem->setData(true, IsFolderRole);  // Mark root as a folder
        model->appendRow(rootItem);
    } else {
        rootItem->setText(name);
    }
}

// Adds a new folder or file under a given parent item
void TreeViewWidget::addNewItem(QStandardItem *parentItem, bool isFolder)
{
    if (!parentItem) {
        qFatal("TreeViewWidget::addNewItem() called with null parentItem");
    }

    // Generate unique name
    QString baseName = isFolder ? "NewFolder" : "NewFile";
    QString defaultName = baseName;
    int suffix = 2;

    while (true) {
        bool duplicateFound = false;

        for (int i = 0; i < parentItem->rowCount(); ++i) {
            const auto *sibling = parentItem->child(i);
            if (sibling->text().trimmed().compare(defaultName, Qt::CaseInsensitive) == 0) {
                duplicateFound = true;
                break;
            }
        }

        if (!duplicateFound) break;
        defaultName = baseName + "_" + QString::number(suffix++);
    }

    // Create the item and mark metadata
    auto *newItem = new QStandardItem(defaultName);
    newItem->setData(isFolder, IsFolderRole);
    newItem->setData(true, NewItemRole);
    newItem->setData(defaultName, PreviousNameRole);

    // Bold font for folders
    if (isFolder) {
        QFont font = newItem->font();
        font.setBold(true);
        newItem->setFont(font);
    }

    parentItem->appendRow(newItem);
    expand(model->indexFromItem(parentItem));
    edit(model->indexFromItem(newItem), QAbstractItemView::DoubleClicked);
}

// Begins inline editing and stores previous value for validation fallback
bool TreeViewWidget::edit(const QModelIndex &index,
                          QAbstractItemView::EditTrigger trigger,
                          QEvent *event)
{
    if (auto *item = model->itemFromIndex(index)) {
        item->setData(item->text(), PreviousNameRole);
    }
    return QTreeView::edit(index, trigger, event);
}

// Adds a new library item under root using the ProjectConfigurator dialog
void TreeViewWidget::addLibrary(QStandardItem *parentItem)
{
    ProjectConfigurator dlg(this, true);  // Library mode

    if (dlg.exec() != QDialog::Accepted)
        return;

    QString libName = dlg.getProjectName().trimmed();
    if (libName.isEmpty())
        return;

    // Check for name conflicts at root level
    for (int i = 0; i < parentItem->rowCount(); ++i) {
        if (parentItem->child(i)->text().trimmed().compare(libName, Qt::CaseInsensitive) == 0) {
            QMessageBox::warning(this, "Duplicate", "A library with that name already exists.");
            return;
        }
    }

    // Create library item
    auto *libItem = new QStandardItem(libName);
    libItem->setData(true, IsFolderRole);  // Libraries behave like folders
    libItem->setData(true, NewItemRole);
    libItem->setData(libName, PreviousNameRole);
    libItem->setData(QStringList(dlg.getSelectedDependencies()), DependenciesRole);

    // Make it bold for visibility
    QFont font = libItem->font();
    font.setBold(true);
    libItem->setFont(font);

    parentItem->appendRow(libItem);
    expand(model->indexFromItem(parentItem));
}

// Context menu handler for right-click actions
void TreeViewWidget::contextMenuEvent(QContextMenuEvent *event)
{
    const QModelIndex index = indexAt(event->pos());
    QStandardItem *selectedItem = index.isValid() ? model->itemFromIndex(index) : rootItem;
    const bool canAdd = selectedItem->data(IsFolderRole).toBool();

    QMenu contextMenu(this);
    contextMenu.setStyleSheet(R"(
        QMenu::item {
            padding: 6px 16px;
            background-color: #2d2d2d;
            color: white;
        }
        QMenu::item:disabled {
            color: #777777;
            background-color: #2d2d2d;
        }
        QMenu::item:selected {
            background-color: #444444;
        }
    )");

    QAction *addFolderAction   = contextMenu.addAction("Add Folder");
    QAction *addFileAction     = contextMenu.addAction("Add File");
    QAction *addLibraryAction  = contextMenu.addAction("Add Library");
    QAction *removeAction      = contextMenu.addAction("Remove");

    addLibraryAction->setEnabled(selectedItem == rootItem);
    addFolderAction->setEnabled(canAdd);
    addFileAction->setEnabled(canAdd);
    removeAction->setEnabled(selectedItem && selectedItem->parent());

    if (QAction *selectedAction = contextMenu.exec(event->globalPos())) {
        if (selectedAction == addFolderAction) {
            addNewItem(selectedItem, true);
        } else if (selectedAction == addFileAction) {
            addNewItem(selectedItem, false);
        } else if (selectedAction == addLibraryAction) {
            addLibrary(selectedItem);
        } else if (selectedAction == removeAction) {
            if (selectedItem->rowCount() > 0) {
                int ret = QMessageBox::warning(this,
                                               tr("Confirm Delete"),
                                               tr("Are you sure you want to delete the folder and its subdirectories?"),
                                               QMessageBox::Yes | QMessageBox::No,
                                               QMessageBox::No);
                if (ret != QMessageBox::Yes)
                    return;
            }

            if (QStandardItem *parent = selectedItem->parent()) {
                parent->removeRow(selectedItem->row());
            }
        }
    }
}

// Validates new item name on change and enforces uniqueness
void TreeViewWidget::onItemChanged(QStandardItem *item)
{
    const QVariant previousVariant = item->data(PreviousNameRole);
    if (!previousVariant.isValid())
        return;

    const QString oldName = previousVariant.toString();
    const QString newName = item->text().trimmed();

    QStandardItem *parentItem = item->parent() ? item->parent() : rootItem;

    bool valid = !newName.isEmpty();
    for (int i = 0; i < parentItem->rowCount(); ++i) {
        QStandardItem *sibling = parentItem->child(i);
        if (sibling == item)
            continue;
        if (sibling->text().trimmed().compare(newName, Qt::CaseInsensitive) == 0) {
            valid = false;
            break;
        }
    }

    if (!valid) {
        qDebug() << "Invalid name detected (" << newName << "); reverting to:" << oldName;
        item->setText(oldName);
    } else {
        item->setData(newName, PreviousNameRole);
    }
}

// Returns the root node of the tree
QStandardItem* TreeViewWidget::getRootItem() const
{
    return rootItem;
}

// Returns the internal model (used for Designer access or external control)
QStandardItemModel* TreeViewWidget::getModel() const
{
    return model;
}
