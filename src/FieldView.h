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

#ifndef FIELDVIEW_H
#define FIELDVIEW_H


#include <Entry.h>
#include <FileGameSound.h>
#include <String.h>
#include <View.h>

#include "Minefield.h"


class GameStyle;

enum
{
	M_SHOW_WORRY = 'ShWr',
	M_HIDE_WORRY = 'HdWr',
	M_SHOW_WIN = 'ShWn',
	M_SHOW_LOSE = 'ShLs',
	M_UPDATE_COUNT = 'UpCn',
	M_START_TIMER = 'StTm',
	M_SIZE_CHANGED = 'SzCh',
	M_SONAR_PENALTY = 'SnPn'
};

class MainWindow;

class FieldView : public BView {
public:
					FieldView(int32 level);
					~FieldView();

	void			AttachedToWindow();
	void			Draw(BRect update);

	void			MouseDown(BPoint pt);
	void			MouseMoved(BPoint pt, uint32 transit, const BMessage *msg);
	void			MouseUp(BPoint pt);

	void			DrawBox(uint16 x, uint16 y, bool down);
	uint16			MineCount();
	uint16			FlagCount();

	void			InvokeTile(const BPoint &pt, uint32 button);
	void			InvokeTile(const IntPoint &pt, uint32 button);
	void			SelectTile(const BPoint &pt);
	IntPoint		GetSelection() const;

	void			ClickBox(const IntPoint &pt);
	void			ToggleBox(const IntPoint &pt);
	void			OpenBox(const IntPoint &pt);
	void			FloodReveal(IntPoint pt);
	void			NumberReveal(IntPoint pt);
	void			DoSonar(IntPoint pt);

	bool			CheckWin();
	void			DoWin();
	void			DoLose();
	void			SetDifficulty(uint8 level);
	void			SetBoard(uint16 width, uint16 height, uint16 count);
	void			StyleChanged();

	void			SetPauseMode(bool paused);
	int8			GetGameState() const;

private:
	void			SetSoundRefs();

	Minefield*		fField;
	bool			fTracking;
	uint32			fButtons;
	IntPoint		fSelection;
	uint16			fFlagCount;
	MainWindow*		fMainWin;
	BString			fThemePath;

	BFileGameSound*	fWinPlayer;
	BFileGameSound*	fLosePlayer;
	BFileGameSound*	fClickPlayer;

	bool			fPauseMode;
	uint8			fDifficulty;
};

#endif
