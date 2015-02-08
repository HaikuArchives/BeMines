#ifndef FIELDVIEW_H
#define FIELDVIEW_H

#include <View.h>
#include <Entry.h>
#include <String.h>
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

class FieldView : public BView
{
public:
				FieldView(int32 level);
				~FieldView(void);
	void		AttachedToWindow(void);
	void		Draw(BRect update);
	void		DrawBox(uint16 x, uint16 y, bool down);
	uint16		MineCount(void);
	uint16		FlagCount(void);

	void		MouseDown(BPoint pt);
	void		MouseMoved(BPoint pt, uint32 transit, const BMessage *msg);
	void		MouseUp(BPoint pt);

	void		InvokeTile(const BPoint &pt, uint32 button);
	void		InvokeTile(const IntPoint &pt, uint32 button);
	void		SelectTile(const BPoint &pt);
	IntPoint	GetSelection(void) const;

	void		ClickBox(const IntPoint &pt);
	void		ToggleBox(const IntPoint &pt);
	void		OpenBox(const IntPoint &pt);
	void		FloodReveal(IntPoint pt);
	void		NumberReveal(IntPoint pt);
	void		DoSonar(IntPoint pt);

	bool		CheckWin(void);
	void		DoWin(void);
	void		DoLose(void);
	void		SetDifficulty(uint8 level);
	void		SetBoard(uint8 width, uint8 height, uint8 count);
	void		StyleChanged(void);

	void		SetPauseMode(bool paused);
	int8		GetGameState(void) const;

private:
	void		SetSoundRefs(void);

	Minefield	*fField;
	bool		fTracking;
	uint32		fButtons;
	IntPoint	fSelection;
	uint16		fFlagCount;
	MainWindow	*fMainWin;
	BString		fThemePath;

	entry_ref	fWinSoundRef,
				fLoseSoundRef,
				fClickSoundRef;
	bool		fPauseMode;
	uint8		fDifficulty;
};

#endif
