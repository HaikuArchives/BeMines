#ifndef CUSTOMWINDOW_H
#define CUSTOMWINDOW_H

#include "DWindow.h"
#include <TextControl.h>

class CustomWindow : public DWindow
{
public:
			CustomWindow(void);
	void	MessageReceived(BMessage *msg);

private:
	void	MakeNumberBox(BTextControl *box);
	void	CheckValues(void);

	BTextControl	*fWidth,
					*fHeight,
					*fMines;
};

#endif
