#ifndef APP_H
#define APP_H

#include <QUdpSocket>
#include "communicationmodule.h"

/**
 * @brief The App class
 */
class App
{
public:
    CommunicationModule commModule; /**< Communication module */
};

#endif // APP_H
