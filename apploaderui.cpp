#include "apploaderui.h"
#include "./ui_apploaderui.h"

AppLoaderUi::AppLoaderUi(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AppLoaderUi)
{
    ui->setupUi(this);
}

AppLoaderUi::~AppLoaderUi()
{
    delete ui;
}

