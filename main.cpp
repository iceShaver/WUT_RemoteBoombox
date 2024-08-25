#include <iostream>

#include "apploader.h"
#include "clientapp.h"
#include "serverapp.h"
#include <QApplication>
#include <QCommandLineParser>

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
    auto const textModeFlag = cmdParser.isSet("t");
    std::unique_ptr<App> pApp = nullptr;



    if (true == cmdParser.isSet("s")) {
        pApp = std::make_unique<App>(new ServerApp(textModeFlag));
    } else if (true == cmdParser.isSet("c")) {
        pApp = std::make_unique<App>(new ClientApp(textModeFlag));
    } else {
        AppLoader appLoader(textModeFlag);
    }

//	MainWindow w;
//	w.show();
    return app.exec();
}
