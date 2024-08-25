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

    ui->widget->setBackgroundRole(QPalette::Base);
    ui->widget->setAutoFillBackground(true);
    ui->widget->setMinimumHeight(30);
    ui->widget->setMinimumWidth(200);

    auto painter = QPainter{ui->widget};
    painter.setPen(Qt::black);
    auto const frame = painter.viewport() - QMargins{10, 10, 10, 10};
    painter.drawRect(frame);
    auto const pos = frame.width() - 1;
    painter.fillRect(frame.left() + 1, frame.top() + 1, pos, frame.height() - 1, Qt::red);

    ui->progressBar->setMaximum(1'000'000);
    ui->progressBar->setMinimum(0);
}

ClientWindow::~ClientWindow()
{
    delete ui;
}

void ClientWindow::setColor(double const value)
{
    static int prev = 0;
    prev = ((5 * prev) + value) / 6;
    ui->progressBar->setValue(prev);
    qDebug() << "set color to: " << value;
}
