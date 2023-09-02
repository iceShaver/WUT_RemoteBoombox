#ifndef APP_H
#define APP_H

#include <QUdpSocket>
#include <memory>

class App
{
public:
	App(bool const textModeFlag);

protected:
	bool const textModeFlag;
	std::unique_ptr<QUdpSocket> pUdpSocket;
};

#endif // APP_H
