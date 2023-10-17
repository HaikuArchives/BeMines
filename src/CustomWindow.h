#ifndef CUSTOMWINDOW_H
#define CUSTOMWINDOW_H

#include "Window.h"

#include <Spinner.h>


class CustomWindow : public BWindow
{
public:
				CustomWindow(BRect frame);
	void		MessageReceived(BMessage* msg);

private:
	uint16		GetMaxWidth();
	uint16		GetMaxHeight();
	void		UpdateDifficulty();

	uint16		fOrigWidth;
	uint16		fOrigHeight;
	uint16		fOrigMines;

	BSpinner*	fWidth;
	BSpinner*	fHeight;
	BSpinner*	fMines;
};

#endif
