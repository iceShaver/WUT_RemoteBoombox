#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include <QLabel>

class ClientApp;

namespace Ui {
class ClientWindow;
}

class ClientWindow : public QMainWindow
{
    Q_OBJECT
    friend class ClientApp;

public:
    explicit ClientWindow(ClientApp &clientApp, QWidget *parent = nullptr);
    ~ClientWindow();
    void setColor(double const value);
    void setVisualizationWidget(QWidget *pWidget);

private:
    Ui::ClientWindow *ui;
    ClientApp &clientApp;
    QMenu *pVisualizationMenu;
    QAction *pSetVisualizationSpectogramAction;
    QAction *pSetVisualizationVolumeBarAction;
    QAction *pSetVisualizationColorAction;
    QLabel *infoLabel;

};

#endif // CLIENTWINDOW_H
