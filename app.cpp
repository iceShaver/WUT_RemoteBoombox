#include "app.h"

App::App(bool const textModeFlag)
	: textModeFlag(textModeFlag)
	, pUdpSocket(new QUdpSocket())
{}
