#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Window.h>
#include "GameStyle.h"

class BitmapButton;
class CounterView;
class TimerView;
class FieldView;

enum
{
	FACE_NORMAL = 0,
	FACE_WORRY,
	FACE_WIN,
	FACE_LOSE
};

enum
{
	M_PAUSE_GAME = 'psgm',
	M_SET_DIFFICULTY = 'stdf'
};

class MainWindow : public BWindow
{
public:
			MainWindow(BRect r);
	bool	QuitRequested(void);
	void	MessageReceived(BMessage *msg);
	void	WindowActivated(bool active);
	void	SetupGame(void);
	void	SetFace(int32 value);
	void	HighScoreCheck(void);
	void	SetTheme(const char *name);
private:
	void	ResetLayout(void);
	void	LoadSettings(void);
	void	SaveSettings(void);
	
	BitmapButton	*fSmileyButton;
	int8			fSmileyState;
	TimerView		*fTimerView;
	FieldView		*fFieldView;
	CounterView		*fCounterView;
	BMenuBar		*fMenuBar;
};

#endif
