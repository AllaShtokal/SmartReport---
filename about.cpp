#include "about.h"
#include "ui_about.h"
#include <QPushButton>

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);

}

About::~About()
{
    delete ui;
}

void About::on_pushButton_clicked()
{
    close();
}
