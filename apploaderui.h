#ifndef APPLOADERUI_H
#define APPLOADERUI_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class AppLoaderUi; }
QT_END_NAMESPACE

class AppLoaderUi : public QMainWindow
{
    Q_OBJECT

public:
    AppLoaderUi(QWidget *parent = nullptr);
    ~AppLoaderUi();

private:
    Ui::AppLoaderUi *ui;
};
#endif // APPLOADERUI_H
