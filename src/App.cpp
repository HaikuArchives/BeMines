/*
 * Copyright 2007, DarkWyrm
 * Copyright 2013-2023, HaikuArchives Team
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		DarkWyrm (original author)
 */


#include "App.h"
#include "GameStyle.h"
#include "Globals.h"
#include "MainWindow.h"


App::App()
	:
	BApplication("application/x-vnd.dw-BeMines")
{
	gGameStyle = new GameStyle(NULL);

	MainWindow* win = new MainWindow(BRect(100, 100, 500, 400));
	win->Show();
}


int
main(int argc, char** argv)
{
	if (argc == 2 && strcmp(argv[1], "-cheat") == 0)
		gCheatMode = true;

	App app;
	app.Run();

	return 0;
}
