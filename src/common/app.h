#ifndef APP_H
#define APP_H

#include <QUdpSocket>
#include <QObject>
#include "common/communicationmodule.h"

/**
 * @brief The App class
 */
class App : public QObject
{
public:
    CommunicationModule commModule; /**< Communication module */
};

#endif // APP_H
