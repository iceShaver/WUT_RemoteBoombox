#ifndef CLIENTAPP_H
#define CLIENTAPP_H
#include "common/app.h"
#include "clientwindow.h"
#include "visualization.h"

/**
 * @brief The Client application logic
 */
class ClientApp : public App
{
public:
    /**
     * @brief ClientApp constructor
     */
    ClientApp();

    /**
     * @brief ClientApp destructor
     */
    ~ClientApp();

    /**
     * @brief Method sets and runs given visualization in the client window
     * @param vis Requested visualization enum
     */
    void setVisualization(EVisualization const vis);

    /**
     * @brief Function initializes client connection to serveer and starts receiving audio data
     * @param serverAddr Server IP address
     * @param serverPort Server port
     */
    void start(QHostAddress serverAddr, uint16_t serverPort);

    /**
     * @brief Function closes client connection to server and stops visualization
     */
    void stop();

private:
    ClientWindow mClientWindow;
    std::unique_ptr<Visualization> pActiveVisualization = nullptr;

};

#endif // CLIENTAPP_H
