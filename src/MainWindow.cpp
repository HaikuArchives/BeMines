#include "MainWindow.h"
#include <Application.h>
#include <File.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <Message.h>
#include <View.h>

#include "AboutWindow.h"
#include "BitmapButton.h"
#include "CounterView.h"
#include "CustomWindow.h"
#include "FieldView.h"
#include "Globals.h"
#include "ScoreWindow.h"
#include "NewScoreWindow.h"
#include "TimerView.h"

enum
{
	M_NEW_GAME = 'nwgm',
	M_SET_THEME = 'stth',
	M_SHOW_SCORES = 'shsc',
	M_TOGGLE_SOUNDS = 'tgsn',
	M_SHOW_CUSTOM = 'shcs'
};

GameStyle *gGameStyle = NULL;

MainWindow::MainWindow(BRect frame)
	:	BWindow(frame,"BeMines",B_TITLED_WINDOW, B_NOT_RESIZABLE),
		fSmileyState(FACE_NORMAL)
{
	LoadSettings();
	
	BView *top = new BView(Bounds(),"back",B_FOLLOW_ALL,B_WILL_DRAW);
	top->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(top);

	BRect r(Bounds());
	r.bottom = 20;
	fMenuBar = new BMenuBar(r, "menubar");
	
	BMenu *menu = new BMenu("Game");
	menu->AddItem(new BMenuItem("New",new BMessage(M_NEW_GAME),'N'));
	menu->AddSeparatorItem();
	
	BMenu *submenu = NULL;
	
	menu->AddItem(new BMenuItem("Pause Game",new BMessage(M_PAUSE_GAME),'P',
								B_COMMAND_KEY));
	
	menu->AddSeparatorItem();

	menu->AddItem(new BMenuItem("High Scores…",new BMessage(M_SHOW_SCORES)));
	
	menu->AddSeparatorItem();

	menu->AddItem(new BMenuItem("About BeMines…",new BMessage(B_ABOUT_REQUESTED)));
	
	fMenuBar->AddItem(menu);
	
	menu = new BMenu("Settings");
	
	submenu = new BMenu("Difficulty");
	
	BMessage *menumsg = new BMessage(M_SET_DIFFICULTY);
	menumsg->AddInt32("level",DIFFICULTY_BEGINNER);
	submenu->AddItem(new BMenuItem("Beginner",menumsg));
	
	menumsg = new BMessage(M_SET_DIFFICULTY);
	menumsg->AddInt32("level",DIFFICULTY_INTERMEDIATE);
	submenu->AddItem(new BMenuItem("Intermediate",menumsg));
	
	menumsg = new BMessage(M_SET_DIFFICULTY);
	menumsg->AddInt32("level",DIFFICULTY_EXPERT);
	submenu->AddItem(new BMenuItem("Expert",menumsg));
	
	menumsg = new BMessage(M_SHOW_CUSTOM);
	submenu->AddItem(new BMenuItem("Custom…",menumsg));
	menu->AddItem(submenu);
	
	BMenuItem *item = submenu->ItemAt(gDifficulty);
	if (item)
		item->SetMarked(true);
	
	submenu = new BMenu("Theme");
	for (int32 i = 0; i < gGameStyle->CountStyles(); i++)
	{
		menumsg = new BMessage(M_SET_THEME);
		menumsg->AddString("name",gGameStyle->StyleAt(i));
		submenu->AddItem(new BMenuItem(gGameStyle->StyleAt(i),menumsg));
	}
	menu->AddItem(submenu);
	submenu->SetRadioMode(true);
	item = submenu->FindItem(gGameStyle->StyleName());
	if (item)
		item->SetMarked(true);
	
	menu->AddSeparatorItem();
	
	item = new BMenuItem("Play Sounds",new BMessage(M_TOGGLE_SOUNDS));
	menu->AddItem(item);
	item->SetMarked(gPlaySounds);
	
	fMenuBar->AddItem(menu);
	
	top->AddChild(fMenuBar);
	
	fCounterView = new CounterView();
	fCounterView->MoveTo(10,fMenuBar->Frame().bottom + 5);
	top->AddChild(fCounterView);
	
	r = gGameStyle->SmileyUp()->Bounds();
	fSmileyButton = new BitmapButton(r,"smiley",gGameStyle->SmileyUp(),
									gGameStyle->SmileyDown(), new BMessage(M_NEW_GAME),false,
									B_FOLLOW_TOP | B_FOLLOW_H_CENTER);
	top->AddChild(fSmileyButton);
	
	fTimerView = new TimerView();
	fTimerView->SetResizingMode(B_FOLLOW_RIGHT | B_FOLLOW_TOP);
	top->AddChild(fTimerView);
	
	fFieldView = new FieldView(gDifficulty);
	top->AddChild(fFieldView);
	fFieldView->MakeFocus(true);
	
	fCounterView->SetCount(fFieldView->MineCount());
	ResetLayout();
}


bool
MainWindow::QuitRequested(void)
{
	SaveSettings();
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}


void
MainWindow::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		case B_ABOUT_REQUESTED:
		{
			AboutWindow *abwin = new AboutWindow();
			abwin->Show();
			break;
		}
		case M_SHOW_CUSTOM:
		{
			CustomWindow *cswin = new CustomWindow();
			cswin->Show();
			break;
		}
		case M_NEW_GAME:
		{
			SetFace(FACE_NORMAL);
			SetupGame();
			break;
		}
		case M_SHOW_SCORES:
		{
			ScoreWindow *scorewin = new ScoreWindow();
			scorewin->Show();
			break;
		}
		case M_PAUSE_GAME:
		{
			BMenuItem *item = fMenuBar->FindItem(M_PAUSE_GAME);
			if (fTimerView->GetState() == TIMER_START)
			{
				fTimerView->SetState(TIMER_STOP);
				if (item)
					item->SetMarked(true);
				fFieldView->SetPauseMode(true);
			}
			else
			{
				if (item->IsMarked())
				{
					fTimerView->SetState(TIMER_START);
					if (item)
						item->SetMarked(false);
					fFieldView->SetPauseMode(false);
				}
			}
			break;
		}
		case M_SET_DIFFICULTY:
		{
			int32 level;
			if (msg->FindInt32("level",&level) != B_OK)
				break;
			
			BMenuItem *item = fMenuBar->FindItem("Beginner");
			BMenu *menu = item ? item->Menu() : NULL;
			
			item = menu->FindMarked();
			if (item)
				item->SetMarked(false);
			menu->ItemAt(level)->SetMarked(true);
			
			gDifficulty = level;
			SetupGame();
			break;
		}
		case M_UPDATE_COUNT:
		{
			fCounterView->SetCount(fFieldView->MineCount() - fFieldView->FlagCount());
			break;
		}
		case M_START_TIMER:
		{
			fTimerView->SetState(TIMER_RESET);
			fTimerView->SetState(TIMER_START);
			gGameState = GAME_STARTED;
			break;
		}
		case M_SONAR_PENALTY:
		{
			fTimerView->SetState(TIMER_STOP);
			if (fTimerView->GetTime() < 979)
				fTimerView->SetTime(fTimerView->GetTime() + 20);
			else
				fTimerView->SetTime(999);
			fTimerView->SetState(TIMER_START);
			break;
		}
		case M_SIZE_CHANGED:
		{
			ResizeTo(fFieldView->Frame().right + 10,fFieldView->Frame().bottom + 10);
			break;
		}
		case M_SET_THEME:
		{
			BString name;
			if (msg->FindString("name",&name) == B_OK)
				SetTheme(name.String());
			
			break;
		}
		case M_TOGGLE_SOUNDS:
		{
			gPlaySounds = gPlaySounds ? false : true;
			BMenuItem *item = fMenuBar->FindItem(M_TOGGLE_SOUNDS);
			if (item)
				item->SetMarked(!item->IsMarked());
			break;
		}
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}


void
MainWindow::WindowActivated(bool active)
{
	if (fFieldView->GetGameState() != GAME_STARTED)
		return;
	
	BMenuItem *item = fMenuBar->FindItem(M_PAUSE_GAME);
	if (!item->IsMarked())
		PostMessage(M_PAUSE_GAME);
}


void
MainWindow::SetupGame(void)
{
	BMenuItem *item = fMenuBar->FindItem(M_PAUSE_GAME);
	if (item)
		item->SetMarked(false);
	fTimerView->SetState(TIMER_RESET);
	fFieldView->SetDifficulty(gDifficulty);
	fCounterView->SetCount(fFieldView->MineCount());
}


void
MainWindow::SetFace(int32 face)
{
	switch (face)
	{
		case FACE_WIN:
		{
			fSmileyButton->SetBitmaps(gGameStyle->WinUp(),gGameStyle->WinDown());
			fTimerView->SetState(TIMER_STOP);
			HighScoreCheck();
			break;
		}
		case FACE_LOSE:
		{
			fSmileyButton->SetBitmaps(gGameStyle->LoseUp(),gGameStyle->LoseDown());
			fTimerView->SetState(TIMER_STOP);
			break;
		}
		case FACE_WORRY:
		{
			fSmileyButton->SetBitmaps(gGameStyle->Worry(),gGameStyle->SmileyDown());
			break;
		}
		default:
		{
			fSmileyButton->SetBitmaps(gGameStyle->SmileyUp(),gGameStyle->SmileyDown());
			break;
		}
	}
	fSmileyState = face;
}


void
MainWindow::HighScoreCheck(void)
{
	uint16 seconds = fTimerView->GetTime();
	
	if (seconds < gBestTimes[gDifficulty].time && !gCheatMode)
	{
		NewScoreWindow *win = new NewScoreWindow(seconds,gDifficulty);
		win->Show();
	}
}


void
MainWindow::SetTheme(const char *name)
{
	gGameStyle->SetStyle(name);
	fFieldView->StyleChanged();
	fCounterView->StyleChanged();
	fTimerView->StyleChanged();
	
	switch (fSmileyState)
	{
		case FACE_LOSE:
		{
			fSmileyButton->SetBitmaps(gGameStyle->LoseUp(),gGameStyle->LoseDown());
			break;
		}
		case FACE_WIN:
		{
			fSmileyButton->SetBitmaps(gGameStyle->WinUp(),gGameStyle->WinDown());
			break;
		}
		default:
		{
			fSmileyButton->SetBitmaps(gGameStyle->SmileyUp(),gGameStyle->SmileyDown());
			break;
		}
	}
	
	ResetLayout();
	
	BMenuItem *item = fMenuBar->FindItem(gGameStyle->StyleName());
	if (item)
		item->SetMarked(true);
}


void
MainWindow::ResetLayout(void)
{
	fTimerView->MoveTo(Bounds().right - 10 - fTimerView->Bounds().Width(),
						fCounterView->Frame().top);
	
	BRect r = gGameStyle->SmileyUp()->Bounds();
	fSmileyButton->ResizeTo(r.Width(),r.Height());
	fSmileyButton->MoveTo( (Bounds().Width() - r.Width()) / 2.0,
							fCounterView->Frame().top);
	
	float bottom  = MAX(fCounterView->Frame().bottom,
						fSmileyButton->Frame().bottom);
	fFieldView->MoveTo(10, bottom + 10);
	ResizeTo(fFieldView->Frame().right + 10,fFieldView->Frame().bottom + 10);
}


void
MainWindow::LoadSettings(void)
{
	BFile file("/boot/home/config/settings/BeMines", B_READ_ONLY);
	if (file.InitCheck() != B_OK)
		return;
	
	BMessage settings;
	if (settings.Unflatten(&file) == B_OK)
	{
		BString s;
		if (settings.FindString("theme",&s) == B_OK)
		{
			gThemeName = s;
			gGameStyle->SetStyle(gThemeName.String());
		}
		
		int32 diff;
		if (settings.FindInt32("level",&diff) == B_OK)
			gDifficulty = diff;
		
		bool b;
		if (settings.FindBool("playsounds",&b) == B_OK)
			gPlaySounds = b;
		
		uint16 seconds;
		if (settings.FindInt16("begbest",(int16*)&seconds) == B_OK)
			gBestTimes[DIFFICULTY_BEGINNER].time = seconds;
		
		if (settings.FindInt16("intbest",(int16*)&seconds) == B_OK)
			gBestTimes[DIFFICULTY_INTERMEDIATE].time = seconds;
		
		if (settings.FindInt16("expbest",(int16*)&seconds) == B_OK)
			gBestTimes[DIFFICULTY_EXPERT].time = seconds;

		if (settings.FindString("begname",&s) == B_OK)
			gBestTimes[DIFFICULTY_BEGINNER].name = s;
		
		if (settings.FindString("intname",&s) == B_OK)
			gBestTimes[DIFFICULTY_INTERMEDIATE].name = s;
		
		if (settings.FindString("expname",&s) == B_OK)
			gBestTimes[DIFFICULTY_EXPERT].name = s;
		
		uint16 size;
		if (settings.FindInt16("customwidth",(int16*)&size) == B_OK)
			gCustomWidth = size;
		if (settings.FindInt16("customheight",(int16*)&size) == B_OK)
			gCustomHeight = size;
		if (settings.FindInt16("customcount",(int16*)&size) == B_OK)
			gCustomMines = size;
		
	}
}


void
MainWindow::SaveSettings(void)
{
	BMessage settings;
	settings.AddBool("playsounds",gPlaySounds);
	settings.AddInt32("level",gDifficulty);
	settings.AddString("theme",gGameStyle->StyleName());
	settings.AddInt16("begbest",gBestTimes[DIFFICULTY_BEGINNER].time);
	settings.AddInt16("intbest",gBestTimes[DIFFICULTY_INTERMEDIATE].time);
	settings.AddInt16("expbest",gBestTimes[DIFFICULTY_EXPERT].time);
	settings.AddString("begname",gBestTimes[DIFFICULTY_BEGINNER].name);
	settings.AddString("intame",gBestTimes[DIFFICULTY_INTERMEDIATE].name);
	settings.AddString("expname",gBestTimes[DIFFICULTY_EXPERT].name);
	
	settings.AddInt16("customwidth",gCustomWidth);
	settings.AddInt16("customheight",gCustomHeight);
	settings.AddInt16("customcount",gCustomMines);
	
	BFile file("/boot/home/config/settings/BeMines", B_READ_WRITE | B_ERASE_FILE |
													B_CREATE_FILE);
	if (file.InitCheck() != B_OK)
		return;
	settings.Flatten(&file);
}

