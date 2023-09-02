#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include <memory>

class ServerApp;

namespace Ui {
class ServerWindow;
}

class ServerWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit ServerWindow(ServerApp &serverApp, QWidget *parent = nullptr);
	~ServerWindow();

public slots:
	void handleButton();

private:
	Ui::ServerWindow *ui;
	ServerApp &serverApp;

};

#endif // SERVERWINDOW_H
