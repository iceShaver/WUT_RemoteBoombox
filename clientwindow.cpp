#include "clientwindow.h"
#include "qpainter.h"
#include "ui_clientwindow.h"
#include "clientapp.h"

ClientWindow::ClientWindow(ClientApp &clientApp, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ClientWindow)
    , clientApp(clientApp)
{

    auto const constexpr X_AXIS_STEP = (RANGE_END - RANGE_START) / (NUM_SAMPLES / 4u);
    auto cur_x_val = RANGE_START;
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


    ui->widget_2->legend->setVisible(false);
    ui->widget_2->yAxis->setLabel("SPL [dB]");
    ui->widget_2->xAxis->setLabel("Frequency [Hz]");
    ui->widget_2->xAxis->setRange(RANGE_START, RANGE_END);
    ui->widget_2->yAxis->setRange(0.0, 10.0);
    ui->widget_2->clearGraphs();
    ui->widget_2->addGraph();
    ui->widget_2->graph()->setPen(QPen(Qt::red));
    ui->widget_2->graph()->setName("Spectrum");

    for (auto i = 0u; i < NUM_SAMPLES / 4u; ++i) {
        x_axis_vals.append(cur_x_val += X_AXIS_STEP);
    }
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

void ClientWindow::updateSpectogram(QVector<double> const &spectogram)
{
    ui->widget_2->graph(0)->setData(x_axis_vals, spectogram);
    ui->widget_2->replot();
}
