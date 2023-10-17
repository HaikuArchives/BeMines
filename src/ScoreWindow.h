/*
 * Copyright 2007, DarkWyrm
 * Copyright 2013-2023, HaikuArchives Team
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		DarkWyrm (original author)
 *		Humdinger
 *		Janus2
 *		JStressman
 *		KevinAdams05
 */

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
