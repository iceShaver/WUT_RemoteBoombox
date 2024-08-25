#include "app.h"

App::App(bool const textModeFlag)
	: textModeFlag(textModeFlag)
	, pCommModule(new CommunicationModule())
{}
