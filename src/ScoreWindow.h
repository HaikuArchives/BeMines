#ifndef SCOREWINDOW_H
#define SCOREWINDOW_H

#include "DWindow.h"
#include <StringView.h>

class ScoreWindow : public DWindow
{
public:
			ScoreWindow(void);
	void	MessageReceived(BMessage *msg);

private:
	void	UpdateLabels(void);

	BStringView	*fBegScore,
				*fIntScore,
				*fExpScore;
};

#endif
