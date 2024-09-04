
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
    , mClientApp(clientApp)
{
    ui->setupUi(this);

    auto pSetVisualizationSpectogramAction = new QAction{
        QIcon::fromTheme(QIcon::ThemeIcon::AudioCard), tr("Frequency spectrum - &plot"), this
    };

    pSetVisualizationSpectogramAction
        ->setStatusTip("Display frequency spectrum plot visualization");

    connect(pSetVisualizationSpectogramAction, &QAction::triggered, this, [&]{
        clientApp.setVisualization(EVisualization::FREQ_SPECTRUM_PLOT);
    });

    auto pSetVisualizationVolumeBarAction = new QAction{
        QIcon::fromTheme(QIcon::ThemeIcon::AudioCard), tr("Frequency spectrum - &bars"), this
    };

    pSetVisualizationVolumeBarAction
        ->setStatusTip("Display frequency spectrum bars visualization");

    connect(pSetVisualizationVolumeBarAction, &QAction::triggered, this, [&]{
        clientApp.setVisualization(EVisualization::FREQ_SPECTRUM_BARS);
    });

    auto pSetVisualizationColorAction = new QAction{
        QIcon::fromTheme(QIcon::ThemeIcon::AudioCard), tr("&Color"), this
    };

    pSetVisualizationColorAction->setStatusTip("Display color visualization");
    connect(pSetVisualizationColorAction, &QAction::triggered, this, [&](){
        clientApp.setVisualization(EVisualization::COLOR);
    });

    auto pVisualizationMenu = menuBar()->addMenu(tr("&Visualization"));
    pVisualizationMenu->addAction(pSetVisualizationSpectogramAction);
    pVisualizationMenu->addAction(pSetVisualizationColorAction);
    pVisualizationMenu->addAction(pSetVisualizationVolumeBarAction);


    // view menu
    auto pSetFullscreenAction = new QAction {
        QIcon::fromTheme(QIcon::ThemeIcon::ViewFullscreen), tr("&Fullscreen"), this
    };
    connect(pSetFullscreenAction, &QAction::triggered, this, [&] {
        showFullScreen();
    });

    auto pSetWindowAction = new QAction {
        QIcon::fromTheme(QIcon::ThemeIcon::WindowNew), tr("&Windowed"), this
    };
    connect(pSetWindowAction, &QAction::triggered, this, [&] {
        showNormal();
    });

    auto pViewMenu = menuBar()->addMenu(tr("Vi&ew"));
    pViewMenu->addAction(pSetFullscreenAction);
    pViewMenu->addAction(pSetWindowAction);

    setWindowTitle(tr("RemoteBoomBox Client"));

    // server select menu
    auto pSelectServerAction = new QAction {
        QIcon::fromTheme(QIcon::ThemeIcon::NetworkWired), tr("&Select server"), this
    };
    connect(pSelectServerAction, &QAction::triggered, this, [&]{
        selectServerDialogShow();
    });

    auto pDisconnectAction = new QAction {
        QIcon::fromTheme(QIcon::ThemeIcon::NetworkWired), tr("&Disconnect"), this
    };

    connect(pDisconnectAction, &QAction::triggered, this, [&]{
        clientApp.stop();
        setVisualizationWidget(nullptr);
        pStatus->setText("Status: Disconnected");
    });

    auto pConnectionMenu = menuBar()->addMenu(tr("&Connection"));
    pConnectionMenu->addAction(pSelectServerAction);
    pConnectionMenu->addAction(pDisconnectAction);


    pStatus = new QLabel(this);
    pStatus->setText("Status: Disconnected");
    statusBar()->addPermanentWidget(pStatus);
}

ClientWindow::~ClientWindow()
{
    delete ui;
}


void ClientWindow::setVisualizationWidget(QWidget *pWidget)
{
    setCentralWidget(pWidget);
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
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

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
        mClientApp.start(QHostAddress(serverIpInput.text()), serverPortInput.text().toUInt());
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
