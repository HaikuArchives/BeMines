/*
 * Copyright 2007, DarkWyrm
 * Copyright 2013-2023, HaikuArchives Team
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Humdinger
 *		Raheem Idowu
 */

#ifndef ACHIVEMENTWINDOW_H
#define ACHIVEMENTWINDOW_H


#include "DWindow.h"
#include <StringView.h>


class AchievementWindow : public DWindow {
public:
					AchievementWindow();

			void	MessageReceived(BMessage *msg);

private:
			void	UpdateLabels();

	BStringView*	beg1Label, *beg2Label;
	BStringView*	int1Label, *int2Label;
	BStringView*	exp1Label, *exp2Label;
	BStringView*	gen1Label, *gen2Label;
	BStringView*	labelList[3][2];
};

#endif
