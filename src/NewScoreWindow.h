#ifndef STRING_WINDOW_H
#define STRING_WINDOW_H

#include <Button.h>
#include <Messenger.h>
#include <Message.h>
#include <TextControl.h>
#include "DWindow.h"

class NewScoreWindow : public DWindow
{
public:
					NewScoreWindow(uint16 time, int32 difficulty);
	bool			QuitRequested(void);
	virtual void	MessageReceived(BMessage *msg);

private:
	BTextControl	*fTextBox;
	BButton			*fClose;
	int32			fDifficulty;
	uint16			fSeconds;
};

#endif
