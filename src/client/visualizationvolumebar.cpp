#include <ranges>
#include <QChart>
#include <QBarSeries>
#include <QBarCategoryAxis>
#include <QChartView>
#include <QVBoxLayout>
#include <QValueAxis>
#include "visualizationvolumebar.h"
#include "server/soundcollector.h"

QWidget *VisualizationVolumeBar::getWidget(void)
{
    auto const constexpr X_AXIS_STEP = (RANGE_END - RANGE_START) / (NUM_SAMPLES / 4u);
    auto cur_x_val = RANGE_START;

    for (auto i = 0u; i < NUM_SAMPLES / 4u; ++i)
    {
        this->x_axis_vals.append(cur_x_val += X_AXIS_STEP);
    }

    auto pChart = new QChart();
    auto pAxisX = new QBarCategoryAxis();
    auto pAxisY = new QValueAxis();
    pChart->setAnimationOptions(QChart::NoAnimation);
    pChart->setTheme(QChart::ChartThemeDark);
    pChart->setTitle("Sound spectrum");

    pAxisX->setLabelsVisible();
    pAxisY->setLabelsVisible();
    pAxisY->setMin(0);
    pAxisY->setMax(10);
    for (int i = 0; i < USEFUL_SPECTOGRAM_DATA_LEN; ++i)
    {
        pAxisX->append(QString::number(this->x_axis_vals[i]));
    }
    pChart->legend()->hide();
    pChart->addAxis(pAxisX, Qt::AlignBottom);
    pChart->addAxis(pAxisY, Qt::AlignLeft);

    for (int i = 0; i < USEFUL_SPECTOGRAM_DATA_LEN; ++i)
    {
        auto pSeries = new QBarSeries();
        pSets.append(new QBarSet(QString::number(this->x_axis_vals[i])));
        *pSets.last() << 1.0;
        pSeries->append(pSets.last());
        pChart->addSeries(pSeries);
        pSeries->attachAxis(pAxisY);
    }

    auto pChartView = new QChartView(pChart);

    return pChartView;
}

void VisualizationVolumeBar::update(QVector<double> &spectogram)
{
    for (auto &&[set, newValue] : std::views::zip(pSets, spectogram))
    {
        set->replace(0, std::min(10.0, std::max(newValue, ((set->at(0u) * 9u) + newValue) / 10u)));
    }
}
