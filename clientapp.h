#ifndef CLIENTAPP_H
#define CLIENTAPP_H

#include "clientwindow.h"
#include "app.h"

class ClientApp : App
{
public:
	ClientApp(bool const textModeFlag);

private:
	std::unique_ptr<ClientWindow> pClientWindow;

};

#endif // CLIENTAPP_H
