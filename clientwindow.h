#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>

class ClientApp;

namespace Ui {
class ClientWindow;
}

class ClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClientWindow(ClientApp &clientApp, QWidget *parent = nullptr);
    ~ClientWindow();
    void setColor(double const value);
    void updateSpectogram(QVector<double> const &spectogram);

private:
    Ui::ClientWindow *ui;
    ClientApp &clientApp;
    QVector<double> x_axis_vals;

};

#endif // CLIENTWINDOW_H
