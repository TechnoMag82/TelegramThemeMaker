#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("About"));
    setFixedSize(this->width(), this->height());
    ui->labelAppName->setText(QCoreApplication::applicationName()
                              + " " + QCoreApplication::applicationVersion());
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
