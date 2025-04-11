#include "LandingPage.h"
#include "ui_LandingPage.h"
#include "ProjectConfigurator.h"
#include <QPushButton>
#include <QDebug>

LandingPage::LandingPage(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::LandingPage)
{
    ui->setupUi(this);

    // Connect the begin button's clicked signal to launchProjectConfigurator slot
    connect(ui->beginButton, &QPushButton::clicked, this, &LandingPage::launchProjectConfigurator);
}

LandingPage::~LandingPage()
{
    qDebug() << "Landing Page close";
    delete ui;
}

void LandingPage::launchProjectConfigurator()
{
    // Create and execute the project configurator dialog
    ProjectConfigurator configurator(this);
    int result = configurator.exec();

    // If the user accepts the dialog, retrieve configuration details and emit the signal
    if (result == QDialog::Accepted) {
        QString projectName = configurator.getProjectName();
        QList<QString> dependencies = configurator.getSelectedDependencies();

        qDebug() << "User accepted project configuration.";
        qDebug() << "Project Name:" << projectName;
        qDebug() << "Dependencies:" << dependencies;

        emit projectConfigured(projectName, dependencies);
    } else {
        qDebug() << "User canceled project configuration.";
    }

}
