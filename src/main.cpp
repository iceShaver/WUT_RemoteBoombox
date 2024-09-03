#include <iostream>
#include <QApplication>
#include <QCommandLineParser>
#include "common/config.h"
#include "common/apploader.h"
#include "client/clientapp.h"
#include "server/serverapp.h"

int main(int argc, char *argv[])
{
    std::cout << "Starting RemoteBoomBox" << std::endl;
    //   display selection window
    QApplication app(argc, argv);
    QApplication::setOrganizationName(QApplication::translate("main", "kamil"));
    QApplication::setOrganizationName(QApplication::translate("main", "RemoteBoomBox"));
    QApplication::setApplicationVersion("1.0");

    QCommandLineParser cmdParser;
    cmdParser.setApplicationDescription(
        QApplication::translate("main", "Application displaying remote visualizations"));
    cmdParser.addHelpOption();
    cmdParser.addVersionOption();
    cmdParser.addOption(
        {{"s", "server"}, QApplication::translate("main", "Run application in server mode")});
    cmdParser.addOption(
        {{"c", "client"}, QApplication::translate("main", "Run application in client mode")});
    cmdParser.addOption(
        {{"t", "tui"}, QApplication::translate("main", "Run application in text mode")});

    cmdParser.process(app);
    Config::textMode = cmdParser.isSet("t");
    std::unique_ptr<App> pApp = nullptr;

    if (true == cmdParser.isSet("s")) {
        pApp = std::make_unique<ServerApp>();
    } else if (true == cmdParser.isSet("c")) {
        pApp = std::make_unique<ClientApp>();
    } else {
        AppLoader appLoader;
    }

//	MainWindow w;
//	w.show();

    auto const retval = app.exec();
    pApp.release();

    return retval;
}
