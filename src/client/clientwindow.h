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

/**
 * @brief Client application user interface
 */
class ClientWindow : public QMainWindow
{
    Q_OBJECT;
    friend class ClientApp;

public:
    /**
     * @brief ClientWindow constructor
     * @param clientApp Reference to client application
     * @param parent Qt parent widget
     */
    ClientWindow(ClientApp &clientApp, QWidget *parent = nullptr);

    /**
     * @brief ClientWindow destructor
     */
    ~ClientWindow();

    /**
     * @brief Function sets given visualization widget as the main widget of the client window
     * @param pWidget Widget pointer
     */
    void setVisualizationWidget(QWidget *pWidget);

    /**
     * @brief Function prompts user for server IP address and port
     */
    void selectServerDialogShow();


public slots:
    /**
     * @brief Slot for server connected event notification
     */
    void connected(void);

    /**
     * @brief Slot for server disconnected event notification
     */
    void disconnected(void);

private:
    Ui::ClientWindow *ui;
    ClientApp &mClientApp;
    QLabel *pStatus;

};

#endif // CLIENTWINDOW_H
