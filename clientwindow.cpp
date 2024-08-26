
#include <QMenuBar>
#include <QActionGroup>
#include <QContextMenuEvent>
#include <QMessageBox>
#include "clientwindow.h"
#include "qpainter.h"
#include "ui_clientwindow.h"
#include "clientapp.h"

ClientWindow::ClientWindow(ClientApp &clientApp, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ClientWindow)
    , clientApp(clientApp)
{
    ui->setupUi(this);

    this->pSetVisualizationSpectogramAction = new QAction{
        QIcon::fromTheme(QIcon::ThemeIcon::AudioCard), tr("&Spectogram"), this
    };
    this->pSetVisualizationSpectogramAction->setStatusTip("Display spectogram visualization");
    this->connect(this->pSetVisualizationSpectogramAction, &QAction::triggered, this, [this](){
        this->clientApp.setVisualization(EVisualization::SPECTOGRAM);
    });

    this->pSetVisualizationVolumeBarAction = new QAction{
        QIcon::fromTheme(QIcon::ThemeIcon::AudioCard), tr("&Volume Bar"), this
    };
    this->pSetVisualizationVolumeBarAction->setStatusTip("Display volume bar visualization");
    this->connect(this->pSetVisualizationVolumeBarAction, &QAction::triggered, this, [this](){
        this->clientApp.setVisualization(EVisualization::VOLUME_BAR);
    });

    this->pSetVisualizationColorAction = new QAction{
        QIcon::fromTheme(QIcon::ThemeIcon::AudioCard), tr("&Color"), this
    };
    this->pSetVisualizationColorAction->setStatusTip("Display color visualization");
    this->connect(this->pSetVisualizationColorAction, &QAction::triggered, this, [this](){
        this->clientApp.setVisualization(EVisualization::COLOR);
    });

    this->pVisualizationMenu = this->menuBar()->addMenu(tr("&Visualization"));
    this->pVisualizationMenu->addAction(this->pSetVisualizationSpectogramAction);
    this->pVisualizationMenu->addAction(this->pSetVisualizationColorAction);
    this->pVisualizationMenu->addAction(this->pSetVisualizationVolumeBarAction);

    setWindowTitle(tr("RemoteBoomBox Client"));
}

ClientWindow::~ClientWindow()
{
    delete ui;
}

void ClientWindow::setColor(double const value)
{
    static int prev = 0;
    prev = ((5 * prev) + value) / 6;
    qDebug() << "set color to: " << value;
}

void ClientWindow::setVisualizationWidget(QWidget *pWidget)
{
    this->setCentralWidget(pWidget);
}
