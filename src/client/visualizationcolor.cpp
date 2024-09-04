#include "visualizationcolor.h"

const constexpr auto COLOR_CHNG_MIN_INTERVAL = 2u;

QWidget *VisualizationColor::getWidget(void)
{
    mpWidget = new QWidget;
    mpWidget->setAutoFillBackground(true);
    palette.setColor(mpWidget->backgroundRole(), Qt::GlobalColor::red);
    mpWidget->setPalette(palette);

    return mpWidget;
}

void VisualizationColor::update(QVector<double> &spectogram)
{
    auto static avg = 0.0;
    auto const end = spectogram.end();
    auto const sum = std::min(1000.0, std::accumulate(spectogram.begin(), end, 0.0));

    auto static chng_min_interval_cntr = 0u;
    avg = ((avg * 9.0) + sum) / 10.0;

    // auto trigger_selector = [&](auto item){return item > (avg * 40.0);};

    if (0u == chng_min_interval_cntr)
    {
        // if (end !=  std::find_if(spectogram.begin(), end, trigger_selector))
        if (sum > ((avg * 1.2) + 5))
        {
            chng_min_interval_cntr = COLOR_CHNG_MIN_INTERVAL;
            setNextColor();
        }
    }
    else
    {
        chng_min_interval_cntr--;
    }
}

void VisualizationColor::setNextColor(void)
{
    currentColor = (Qt::GlobalColor)((currentColor + 1) % Qt::GlobalColor::transparent);
    palette.setColor(mpWidget->backgroundRole(), currentColor);
    mpWidget->setPalette(palette);

}
