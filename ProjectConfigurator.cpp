#include "ProjectConfigurator.h"
#include "./ui_ProjectConfigurator.h"

#include <QAbstractItemView>
#include <QPushButton>
#include <QDebug>

ProjectConfigurator::ProjectConfigurator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    qDebug() << parent->accessibleName();

    // Populate the list widget with available libraries
    ui->dependenciesListWidget->addItem("Boost");
    ui->dependenciesListWidget->addItem("OpenCV");
    ui->dependenciesListWidget->addItem("OpenMP");
    ui->dependenciesListWidget->addItem("Eigen");
    // Add any additional libraries as needed

    // Allow multiple selections from the list
    ui->dependenciesListWidget->setSelectionMode(QAbstractItemView::MultiSelection);

    // Remove icons from the standard buttons
    QPushButton* okButton = ui->buttonBox->button(QDialogButtonBox::Ok);
    QPushButton* cancelButton = ui->buttonBox->button(QDialogButtonBox::Cancel);

    if (okButton) {
        okButton->setIcon(QIcon());  // Remove the default icon
        okButton->setText("OK");     // Ensure text is set if the icon was providing the label
        okButton->setDisabled(true);   // Disable the OK button initially
        okButton->setObjectName("okButton");
        // Apply a dedicated stylesheet directly on the OK button
        okButton->setStyleSheet(
            "QPushButton {"
            "    background-color: #2d2d2d;"
            "    color: #ffffff;"
            "    border: 1px solid #444444;"
            "    padding: 5px 12px;"
            "    border-radius: 4px;"
            "}"
            "QPushButton:disabled {"
            "    background-color: #555555;"  // Grayer tone when disabled
            "    color: #aaaaaa;"              // Lighter grey text when disabled
            "    border: 1px solid #444444;"
            "}"
            );
    }

    if (cancelButton) {
        cancelButton->setIcon(QIcon()); // Remove the default icon
        cancelButton->setText("Cancel");
    }

    // Connect the project name QLineEdit's textChanged signal to enable/disable the OK button
    connect(ui->projectNameLineEdit, &QLineEdit::textChanged, this, [okButton](const QString &text) {
        if (okButton) {
            okButton->setDisabled(text.trimmed().isEmpty());
        }
    });

    // Manually connect accepted/rejected signals from the button box
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &ProjectConfigurator::handleAccepted);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &ProjectConfigurator::handleRejected);
}

ProjectConfigurator::~ProjectConfigurator()
{
    qDebug() << "Proj Config close";
    delete ui;
}

QString ProjectConfigurator::getProjectName() const
{
    return ui->projectNameLineEdit->text();
}

QList<QString> ProjectConfigurator::getSelectedDependencies() const
{
    QList<QString> dependencies;
    for (auto *item : ui->dependenciesListWidget->selectedItems()) {
        dependencies.append(item->text());
    }
    return dependencies;
}

void ProjectConfigurator::handleAccepted()
{
    // Accept the dialog so that exec() returns QDialog::Accepted.
    accept();
}

void ProjectConfigurator::handleRejected()
{
    // Reject the dialog so that exec() returns QDialog::Rejected.
    reject();
}
