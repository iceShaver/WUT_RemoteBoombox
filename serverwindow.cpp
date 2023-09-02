#include "serverwindow.h"
#include "ui_serverwindow.h"
#include "serverapp.h"

ServerWindow::ServerWindow(ServerApp &serverApp, QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::ServerWindow),
	serverApp(serverApp)
{
	ui->setupUi(this);
	connect(ui->startStopBtn, &QPushButton::released, this, &ServerWindow::handleButton);
}

ServerWindow::~ServerWindow()
{
	delete ui;
}

void ServerWindow::handleButton()
{
	qDebug() << "Button released";
	QString const &currentStr = ui->startStopBtn->text();

	if ("Start" == currentStr) {
		//		start server
		serverApp.start();
		ui->startStopBtn->setText("Stop");
	} else {
		//		stop server
		serverApp.stop();
		ui->startStopBtn->setText("Start");
	}
}
