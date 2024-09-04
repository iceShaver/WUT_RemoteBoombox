
#include <QMenuBar>
#include <QActionGroup>
#include <QContextMenuEvent>
#include <QMessageBox>
#include <QPainter>
#include <QLineEdit>
#include <QIntValidator>
#include <QPushButton>
#include <QTimer>
#include <QDialogButtonBox>
#include "clientwindow.h"
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


    // view menu
    this->pSetFullscreenAction = new QAction {
        QIcon::fromTheme(QIcon::ThemeIcon::ViewFullscreen), tr("&Fullscreen"), this
    };
    this->connect(this->pSetFullscreenAction, &QAction::triggered, this, [this] {
        this->showFullScreen();
    });

    this->pSetWindowAction = new QAction {
        QIcon::fromTheme(QIcon::ThemeIcon::WindowNew), tr("&Windowed"), this
    };
    this->connect(this->pSetWindowAction, &QAction::triggered, this, [this] {
        this->showNormal();
    });

    this->pViewMenu = this->menuBar()->addMenu(tr("Vi&ew"));

    this->pViewMenu->addAction(this->pSetFullscreenAction);
    this->pViewMenu->addAction(this->pSetWindowAction);

    setWindowTitle(tr("RemoteBoomBox Client"));

    // server select menu
    this->pSelectServerAction = new QAction {
        QIcon::fromTheme(QIcon::ThemeIcon::NetworkWired), tr("&Select server"), this
    };
    this->connect(this->pSelectServerAction, &QAction::triggered, this, [this]{
        this->selectServerDialogShow();
    });

    this->pConnectionMenu = this->menuBar()->addMenu(tr("&Connection"));
    this->pConnectionMenu->addAction(pSelectServerAction);


    pStatus = new QLabel(this);
    pStatus->setText("Status: Disconnected");
    statusBar()->addPermanentWidget(pStatus);
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

void ClientWindow::selectServerDialogShow()
{
    auto dialog = QDialog();
    auto buttonBox = QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    auto formLayout = QFormLayout();
    auto serverIpLabel = QLabel();
    auto serverPortLabel = QLabel();
    auto serverIpInput = QLineEdit();
    auto serverPortInput = QLineEdit();

    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(rejected()));

    serverIpLabel.setText("IP address");
    serverIpInput.setText(CommunicationModule::DEFAULT_SERVER_ADDR);
    serverPortLabel.setText("Port");
    serverPortInput.setText(QString::number(CommunicationModule::DEFAULT_SERVER_PORT));

    serverPortInput.setValidator(new QIntValidator(0, UINT16_MAX, &dialog));

    formLayout.addRow(&serverIpLabel, &serverIpInput);
    formLayout.addRow(&serverPortLabel, &serverPortInput);
    formLayout.addWidget(&buttonBox);

    dialog.setLayout(&formLayout);

    if (QDialog::Accepted == dialog.exec())
    {
        qDebug() << "changed server";
        clientApp.start(QHostAddress(serverIpInput.text()), serverPortInput.text().toUInt());
    }
}

void ClientWindow::connected(void)
{
    pStatus->setText("Status: Connected");
}

void ClientWindow::disconnected(void)
{
    pStatus->setText("Status: Disconnected");
}
