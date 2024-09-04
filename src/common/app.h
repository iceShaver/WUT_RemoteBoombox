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
    /**
     * @brief Destructor of App
     */
    virtual ~App();

    /**
     * @brief Communication module for communicating with server or client
     */
    CommunicationModule commModule; /**< Communication module */
};



#endif // APP_H
