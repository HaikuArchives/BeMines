#ifndef SCOREWINDOW_H
#define SCOREWINDOW_H


#include "DWindow.h"
#include <StringView.h>


class ScoreWindow : public DWindow {
public:
					ScoreWindow();
	void			MessageReceived(BMessage *msg);

private:
	void			UpdateLabels();
	BString			_GetLabelForDifficulty(uint32 difficulty) const;

	BStringView*	fBegScore;
	BStringView*	fIntScore;
	BStringView*	fExpScore;
};

#endif
