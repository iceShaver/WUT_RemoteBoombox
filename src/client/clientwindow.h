#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>

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
    void selectServerDialogShow();

private:
    Ui::ClientWindow *ui;
    ClientApp &clientApp;
    QMenu *pVisualizationMenu;
    QAction *pSetVisualizationSpectogramAction;
    QAction *pSetVisualizationVolumeBarAction;
    QAction *pSetVisualizationColorAction;
    QMenu *pViewMenu;
    QAction *pSetFullscreenAction;
    QAction *pSetWindowAction;
    QLabel *infoLabel;

    QMenu *pConnectionMenu;
    QAction *pSelectServerAction;

    QLabel *pStatus;
};

#endif // CLIENTWINDOW_H
