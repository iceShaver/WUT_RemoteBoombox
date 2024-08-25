#ifndef APP_H
#define APP_H

#include <QUdpSocket>
#include <memory>
#include "communicationmodule.h"

class App
{
public:
    App(bool const textModeFlag);
    std::unique_ptr<CommunicationModule> pCommModule;
protected:
    bool const textModeFlag;
//	std::unique_ptr<QUdpSocket> pUdpSocket;

};

#endif // APP_H
