/*
 * Copyright 2007, DarkWyrm
 * Copyright 2013-2023, HaikuArchives Team
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		DarkWyrm (original author)
 *		Diver
 *		Humdinger
 *		Johan Wagenheim
 *		Janus2
 *		JStressman
 *		KevinAdams05
 *		Raheem Idowu
 */


#include <AboutWindow.h>
#include <Alert.h>
#include <Application.h>
#include <Catalog.h>
#include <File.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <Message.h>
#include <Screen.h>
#include <View.h>

#include <stdio.h>

#include "AchievementWindow.h"
#include "BitmapButton.h"
#include "CounterView.h"
#include "CustomWindow.h"
#include "FieldView.h"
#include "Globals.h"
#include "HelpWindow.h"
#include "MainWindow.h"
#include "NewScoreWindow.h"
#include "ScoreWindow.h"
#include "StringFormat.h"
#include "TimerView.h"

enum {
	M_NEW_GAME = 'nwgm',
	M_SHOW_HELP = 'help',
	M_SET_THEME = 'stth',
	M_SHOW_SCORES = 'shsc',
	M_TOGGLE_SCALING = 'tgsc',
	M_TOGGLE_SOUNDS = 'tgsn',
	M_SHOW_CUSTOM = 'shcs',
	M_SHOW_ACHIEVEMENTS = 'shac'
};

GameStyle* gGameStyle = NULL;

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "MainWindow"


MainWindow::MainWindow(BRect frame)
	:
	BWindow(frame, B_TRANSLATE_SYSTEM_NAME("BeMines"), B_TITLED_WINDOW, B_NOT_RESIZABLE),
	fSmileyState(FACE_NORMAL)
{
	LoadSettings();

	BView* top = new BView(Bounds(), "back", B_FOLLOW_ALL, B_WILL_DRAW);
	top->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(top);

	BRect r(Bounds());
	r.bottom = 20;
	fMenuBar = new BMenuBar(r, "menubar");

	BMenu* menu = new BMenu(B_TRANSLATE("Game"));
	menu->AddItem(new BMenuItem(B_TRANSLATE("New"), new BMessage(M_NEW_GAME), 'N'));

	BMenu* submenu = NULL;

	menu->AddItem(
		new BMenuItem(B_TRANSLATE("Pause"), new BMessage(M_PAUSE_GAME), 'P', B_COMMAND_KEY));

	menu->AddSeparatorItem();

	menu->AddItem(new BMenuItem(B_TRANSLATE("High scores"), new BMessage(M_SHOW_SCORES)));
	menu->AddItem(new BMenuItem(B_TRANSLATE("Achievements"), new BMessage(M_SHOW_ACHIEVEMENTS)));

	menu->AddSeparatorItem();

	menu->AddItem(new BMenuItem(B_TRANSLATE("Help"), new BMessage(M_SHOW_HELP)));
	menu->AddItem(new BMenuItem(B_TRANSLATE("About BeMines"), new BMessage(B_ABOUT_REQUESTED)));

	menu->AddSeparatorItem();

	menu->AddItem(new BMenuItem(B_TRANSLATE("Quit"), new BMessage(B_QUIT_REQUESTED), 'Q'));

	fMenuBar->AddItem(menu);

	menu = new BMenu(B_TRANSLATE("Settings"));

	submenu = new BMenu(B_TRANSLATE("Difficulty"));

	BMessage* menumsg = new BMessage(M_SET_DIFFICULTY);
	menumsg->AddInt32("level", DIFFICULTY_BEGINNER);
	submenu->AddItem(new BMenuItem(B_TRANSLATE("Beginner"), menumsg));

	menumsg = new BMessage(M_SET_DIFFICULTY);
	menumsg->AddInt32("level", DIFFICULTY_INTERMEDIATE);
	submenu->AddItem(new BMenuItem(B_TRANSLATE("Intermediate"), menumsg));

	menumsg = new BMessage(M_SET_DIFFICULTY);
	menumsg->AddInt32("level", DIFFICULTY_EXPERT);
	submenu->AddItem(new BMenuItem(B_TRANSLATE("Expert"), menumsg));

	menumsg = new BMessage(M_SHOW_CUSTOM);
	submenu->AddItem(new BMenuItem(B_TRANSLATE("Custom" B_UTF8_ELLIPSIS), menumsg));
	menu->AddItem(submenu);

	BMenuItem* item = submenu->ItemAt(gDifficulty);
	if (item)
		item->SetMarked(true);

	submenu = new BMenu(B_TRANSLATE("Theme"));
	for (int32 i = 0; i < gGameStyle->CountStyles(); i++) {
		menumsg = new BMessage(M_SET_THEME);
		menumsg->AddString("name", gGameStyle->StyleAt(i));
		submenu->AddItem(new BMenuItem(TranslateWellKnownThemes(gGameStyle->StyleAt(i)), menumsg));
	}
	menu->AddItem(submenu);
	submenu->SetRadioMode(true);
	item = submenu->FindItem(TranslateWellKnownThemes(gGameStyle->StyleName()));
	if (item)
		item->SetMarked(true);

	item = new BMenuItem(B_TRANSLATE("Scale 2x"), new BMessage(M_TOGGLE_SCALING));
	menu->AddItem(item);
	item->SetMarked(gScale == 2);

	menu->AddSeparatorItem();

	item = new BMenuItem(B_TRANSLATE("Sound effects"), new BMessage(M_TOGGLE_SOUNDS));
	menu->AddItem(item);
	item->SetMarked(gPlaySounds);

	fMenuBar->AddItem(menu);

	top->AddChild(fMenuBar);

	fCounterView = new CounterView();
	fCounterView->MoveTo(10, fMenuBar->Frame().bottom + 10);
	top->AddChild(fCounterView);

	r = gGameStyle->SmileyUp()->Bounds();
	r.right = r.left + r.Width() * gScale;
	r.bottom = r.top + r.Height() * gScale;

	fSmileyButton = new BitmapButton(r, "smiley", gGameStyle->SmileyUp(), gGameStyle->SmileyDown(),
		new BMessage(M_NEW_GAME), false, B_FOLLOW_TOP | B_FOLLOW_H_CENTER);
	top->AddChild(fSmileyButton);

	fTimerView = new TimerView();
	fTimerView->SetResizingMode(B_FOLLOW_RIGHT | B_FOLLOW_TOP);
	top->AddChild(fTimerView);

	fFieldView = new FieldView(gDifficulty);
	top->AddChild(fFieldView);
	fFieldView->MakeFocus(true);

	fCounterView->SetCount(fFieldView->MineCount());
	ResetLayout();

	AddCommonFilter(new SpaceBarFilter(M_NEW_GAME));
}


void
MainWindow::AboutRequested()
{
	BAboutWindow* about
		= new BAboutWindow(B_TRANSLATE_SYSTEM_NAME("BeMines"), "application/x-vnd.dw-BeMines");

	const char* kCopyright = "The HaikuArchives Team";

	const char* kExtraCopyrights[] = {"2009 DarkWyrm", NULL};

	const char* kAuthors[] = { B_TRANSLATE(
		"DarkWyrm (original author)"),
		"Adrien Destugues",
		"Humdinger",
		"Johan Wagenheim",
		"Janus",
		"Justin Stressman",
		"Kevin Adams",
		"Raheem Idowu",
		NULL};
	about->AddDescription(B_TRANSLATE("A themable, open-source rendition of Minesweeper."));
	about->AddCopyright(2015, kCopyright, kExtraCopyrights);
	about->AddAuthors(kAuthors);
	about->AddExtraInfo(B_TRANSLATE(
		"Click sound by Sebastian\n"
		"Lose and win sounds by Mike Koenig"));
	about->Show();
}


bool
MainWindow::QuitRequested()
{
	SaveSettings();
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}


void
MainWindow::MessageReceived(BMessage* msg)
{
	switch (msg->what) {
		case B_ABOUT_REQUESTED:
		{
			AboutRequested();
		} break;
		case M_SHOW_HELP:
		{
			HelpWindow* helpwin = new HelpWindow();
			helpwin->Show();
		} break;
		case M_SHOW_CUSTOM:
		{
			CustomWindow* cswin = new CustomWindow(Frame());
			cswin->Show();
			} break;
		case M_NEW_GAME:
		{
			SetFace(FACE_NORMAL);
			SetupGame();
		} break;
		case M_SHOW_SCORES:
		{
			ScoreWindow* scorewin = new ScoreWindow();
			scorewin->Show();
		} break;
		case M_SHOW_ACHIEVEMENTS:
		{
			AchievementWindow* achievementwin = new AchievementWindow();
			achievementwin->Show();
		} break;
		case M_PAUSE_GAME:
		{
			BMenuItem* item = fMenuBar->FindItem(M_PAUSE_GAME);
			if (fTimerView->GetState() == TIMER_START) {
				fTimerView->SetState(TIMER_STOP);
				if (item)
					item->SetMarked(true);
				fFieldView->SetPauseMode(true);
			} else if (item->IsMarked()) {
				fTimerView->SetState(TIMER_START);
				if (item)
					item->SetMarked(false);
				fFieldView->SetPauseMode(false);
			}
		} break;
		case M_SET_DIFFICULTY:
		{
			int32 level;
			if (msg->FindInt32("level", &level) != B_OK)
				break;

			BMenuItem* item = fMenuBar->FindItem(M_SET_DIFFICULTY);
			BMenu* menu = item ? item->Menu() : NULL;

			item = menu->FindMarked();
			if (item)
				item->SetMarked(false);
			menu->ItemAt(level)->SetMarked(true);

			gDifficulty = level;
			SetupGame();
			fFieldView->SetPauseMode(false);
		} break;
		case M_UPDATE_COUNT:
		{
			fCounterView->SetCount(fFieldView->MineCount() - fFieldView->FlagCount());
		} break;
		case M_START_TIMER:
		{
			fTimerView->SetState(TIMER_RESET);
			fTimerView->SetState(TIMER_START);
			gGameState = GAME_STARTED;
		} break;
		case M_SONAR_PENALTY:
		{
			fTimerView->SetState(TIMER_STOP);
			if (fTimerView->GetTime() < 979)
				fTimerView->SetTime(fTimerView->GetTime() + 20);
			else
				fTimerView->SetTime(999);
			fTimerView->SetState(TIMER_START);
			if (fFieldView->CheckWin())
				fFieldView->DoWin();
		} break;
		case M_SIZE_CHANGED:
		{
			ResizeTo(fFieldView->Frame().right + 10, fFieldView->Frame().bottom + 10);
		} break;
		case M_SET_THEME:
		{
			BString name;
			if (msg->FindString("name", &name) == B_OK)
				SetTheme(name.String());

		} break;
		case M_TOGGLE_SOUNDS:
		{
			gPlaySounds = gPlaySounds ? false : true;
			BMenuItem* item = fMenuBar->FindItem(M_TOGGLE_SOUNDS);
			if (item)
				item->SetMarked(!item->IsMarked());
		} break;
		case M_TOGGLE_SCALING:
		{
			// Don't allow 2x scaling if the custom board won't fit on screen
			if (!CanScale())
				break;

			gScale = 3 - gScale; // Toggle between 1 and 2
			fFieldView->StyleChanged();
			fCounterView->StyleChanged();
			fTimerView->StyleChanged();
			ResetLayout();
			BMenuItem* item = fMenuBar->FindItem(M_TOGGLE_SCALING);
			if (item)
				item->SetMarked(!item->IsMarked());
		} break;
		default:
		{
			BWindow::MessageReceived(msg);
		}
	}
}


void
MainWindow::WindowActivated(bool active)
{
	if (fFieldView->GetGameState() != GAME_STARTED)
		return;

	BMenuItem* item = fMenuBar->FindItem(M_PAUSE_GAME);
	if (!item->IsMarked())
		PostMessage(M_PAUSE_GAME);
}


void
MainWindow::SetupGame()
{
	BMenuItem* item = fMenuBar->FindItem(M_PAUSE_GAME);
	if (item)
		item->SetMarked(false);
	fFieldView->SetPauseMode(false);
	fTimerView->SetState(TIMER_RESET);
	fFieldView->SetDifficulty(gDifficulty);
	fCounterView->SetCount(fFieldView->MineCount());
}


void
MainWindow::SetFace(int32 face)
{
	switch (face) {
		case FACE_WIN:
		{
			fSmileyButton->SetBitmaps(gGameStyle->WinUp(), gGameStyle->WinDown());
			fTimerView->SetState(TIMER_STOP);
			AchievementCheck();
			HighScoreCheck();
		} break;
		case FACE_LOSE:
		{
			fSmileyButton->SetBitmaps(gGameStyle->LoseUp(), gGameStyle->LoseDown());
			fTimerView->SetState(TIMER_STOP);
		} break;
		case FACE_WORRY:
		{
			fSmileyButton->SetBitmaps(gGameStyle->Worry(), gGameStyle->SmileyDown());
		} break;
		default:
		{
			fSmileyButton->SetBitmaps(gGameStyle->SmileyUp(), gGameStyle->SmileyDown());
		}
	}
	fSmileyState = face;
}


void
MainWindow::HighScoreCheck()
{
	uint16 seconds = fTimerView->GetTime();

	if (gDifficulty < DIFFICULTY_CUSTOM && seconds < gBestTimes[gDifficulty].time && !gCheatMode) {
		NewScoreWindow* win = new NewScoreWindow(seconds, gDifficulty);
		win->Show();
	}
}


void
MainWindow::AchievementCheck()
{
	uint16 seconds = fTimerView->GetTime();
	int numAchieved = 0;

	gGamesWon++;

	// General achievement
	if (gGamesWon > 0 && !gAchievements[3][0]) {
		gAchievements[3][0] = true;
		numAchieved++;
	}

	if (gGamesWon > 4 && !gAchievements[3][1]) {
		gAchievements[3][1] = true;
		numAchieved++;
	}

	// Time based achievements for each difficulty
	if (gDifficulty < DIFFICULTY_CUSTOM && seconds < 100 && !gAchievements[gDifficulty][0]) {
		gAchievements[gDifficulty][0] = true;
		numAchieved++;
	}
	if (gDifficulty < DIFFICULTY_CUSTOM && seconds < 50 && !gAchievements[gDifficulty][0]) {
		gAchievements[gDifficulty][1] = true;
		numAchieved++;
	}

	if (numAchieved > 0) {
		AchievementWindow* achievementwin = new AchievementWindow();
		BString title;
		static BStringFormat format(B_TRANSLATE(
			"{0, plural,"
			"=1{New achievement unlocked!}"
			"other{New achievements unlocked!}}"));
		format.Format(title, numAchieved);
		achievementwin->SetTitle(title);
		achievementwin->Show();
	}
}


void
MainWindow::SetTheme(const char* name)
{
	gGameStyle->SetStyle(name);
	fFieldView->StyleChanged();
	fCounterView->StyleChanged();
	fTimerView->StyleChanged();

	switch (fSmileyState) {
		case FACE_LOSE:
		{
			fSmileyButton->SetBitmaps(gGameStyle->LoseUp(), gGameStyle->LoseDown());
		} break;
		case FACE_WIN:
		{
			fSmileyButton->SetBitmaps(gGameStyle->WinUp(), gGameStyle->WinDown());
		} break;
		default:
		{
			fSmileyButton->SetBitmaps(gGameStyle->SmileyUp(), gGameStyle->SmileyDown());
		}
	}

	ResetLayout();

	BMenuItem* item = fMenuBar->FindItem(TranslateWellKnownThemes(gGameStyle->StyleName()));
	if (item)
		item->SetMarked(true);
}


const char*
MainWindow::TranslateWellKnownThemes(const char* name)
{
	if (strcmp(name, "Be") == 0)
		return B_TRANSLATE_COMMENT("Be", "Theme name");
	else if (strcmp(name, "Default") == 0)
		return B_TRANSLATE_COMMENT("Default", "Theme name");
	else if (strcmp(name, "Classic") == 0)
		return B_TRANSLATE_COMMENT("Classic", "Theme name");
	else if (strcmp(name, "Gray") == 0)
		return B_TRANSLATE_COMMENT("Gray", "Theme name");
	else if (strcmp(name, "Gnome") == 0)
		return B_TRANSLATE_COMMENT("Gnome", "Theme name");
	else
		return name;
}


void
MainWindow::ResetLayout()
{
	fCounterView->MoveTo(10, fMenuBar->Frame().bottom + 10);
	float heightCounter = fCounterView->Bounds().Height();

	BRect smileRect = gGameStyle->SmileyUp()->Bounds();

	smileRect.right = smileRect.left + smileRect.Width() * gScale;
	smileRect.bottom = smileRect.top + smileRect.Height() * gScale;

	float smileHeight = smileRect.Height();
	float toolBarHeight = MAX(heightCounter, smileHeight);

	float offsetSmile = (toolBarHeight - smileHeight) / 2;
	float offsetCounter = (toolBarHeight - heightCounter) / 2;

	fTimerView->MoveTo(Bounds().right - 10 - fTimerView->Bounds().Width(),
		fCounterView->Frame().top + offsetCounter);

	fSmileyButton->ResizeTo(smileRect.Width(), smileRect.Height());

	fSmileyButton->MoveTo(
		(Bounds().Width() - smileRect.Width()) / 2.0, fCounterView->Frame().top + offsetSmile);
	fCounterView->MoveBy(0, offsetCounter);

	float bottom = MAX(fCounterView->Frame().bottom, fSmileyButton->Frame().bottom);
	fFieldView->MoveTo(10, bottom + 10);
	ResizeTo(fFieldView->Frame().right + 10, fFieldView->Frame().bottom + 10);
}


void
MainWindow::LoadSettings()
{
	BFile file("/boot/home/config/settings/BeMines", B_READ_ONLY);
	if (file.InitCheck() != B_OK)
		return;

	BMessage settings;
	if (settings.Unflatten(&file) == B_OK) {
		BString name;
		if (settings.FindString("theme", &name) == B_OK) {
			gThemeName = name;
			gGameStyle->SetStyle(gThemeName.String());
		}

		int32 diff;
		if (settings.FindInt32("level", &diff) == B_OK)
			gDifficulty = diff;

		bool sound;
		if (settings.FindBool("playsounds", &sound) == B_OK)
			gPlaySounds = sound;

		uint16 scale;
		if (settings.FindInt16("scale", (int16*) &scale) == B_OK)
			gScale = scale;

		uint16 seconds;
		if (settings.FindInt16("begbest", (int16*) &seconds) == B_OK)
			gBestTimes[DIFFICULTY_BEGINNER].time = seconds;

		if (settings.FindInt16("intbest", (int16*) &seconds) == B_OK)
			gBestTimes[DIFFICULTY_INTERMEDIATE].time = seconds;

		if (settings.FindInt16("expbest", (int16*) &seconds) == B_OK)
			gBestTimes[DIFFICULTY_EXPERT].time = seconds;

		if (settings.FindString("begname", &name) == B_OK)
			gBestTimes[DIFFICULTY_BEGINNER].name = name;

		if (settings.FindString("intname", &name) == B_OK)
			gBestTimes[DIFFICULTY_INTERMEDIATE].name = name;

		if (settings.FindString("expname", &name) == B_OK)
			gBestTimes[DIFFICULTY_EXPERT].name = name;

		uint16 size;
		if (settings.FindInt16("customwidth", (int16*) &size) == B_OK)
			gCustomWidth = size;
		if (settings.FindInt16("customheight", (int16*) &size) == B_OK)
			gCustomHeight = size;
		if (settings.FindInt16("customcount", (int16*) &size) == B_OK)
			gCustomMines = size;

		// Achievements
		bool achieved;
		if (settings.FindBool("beg0", &achieved) == B_OK)
			gAchievements[DIFFICULTY_BEGINNER][0] = achieved;
		if (settings.FindBool("beg1", &achieved) == B_OK)
			gAchievements[DIFFICULTY_BEGINNER][1] = achieved;

		if (settings.FindBool("int0", &achieved) == B_OK)
			gAchievements[DIFFICULTY_INTERMEDIATE][0] = achieved;
		if (settings.FindBool("int1", &achieved) == B_OK)
			gAchievements[DIFFICULTY_INTERMEDIATE][1] = achieved;

		if (settings.FindBool("exp0", &achieved) == B_OK)
			gAchievements[DIFFICULTY_EXPERT][0] = achieved;
		if (settings.FindBool("exp1", &achieved) == B_OK)
			gAchievements[DIFFICULTY_EXPERT][1] = achieved;

		if (settings.FindBool("gen0", &achieved) == B_OK)
			gAchievements[3][0] = achieved;
		if (settings.FindBool("gen1", &achieved) == B_OK)
			gAchievements[3][1] = achieved;

		// Games won
		int32 num;
		if (settings.FindInt32("gameswon", &num) == B_OK)
			gGamesWon = num;
	}
}


void
MainWindow::SaveSettings()
{
	BMessage settings;
	settings.AddBool("playsounds", gPlaySounds);
	settings.AddInt16("scale", gScale);
	settings.AddInt32("level", gDifficulty);
	settings.AddString("theme", gGameStyle->StyleName());
	settings.AddInt16("begbest", gBestTimes[DIFFICULTY_BEGINNER].time);
	settings.AddInt16("intbest", gBestTimes[DIFFICULTY_INTERMEDIATE].time);
	settings.AddInt16("expbest", gBestTimes[DIFFICULTY_EXPERT].time);
	settings.AddString("begname", gBestTimes[DIFFICULTY_BEGINNER].name);
	settings.AddString("intame", gBestTimes[DIFFICULTY_INTERMEDIATE].name);
	settings.AddString("expname", gBestTimes[DIFFICULTY_EXPERT].name);

	settings.AddInt16("customwidth", gCustomWidth);
	settings.AddInt16("customheight", gCustomHeight);
	settings.AddInt16("customcount", gCustomMines);

	settings.AddBool("beg0", gAchievements[DIFFICULTY_BEGINNER][0]);
	settings.AddBool("beg1", gAchievements[DIFFICULTY_BEGINNER][1]);
	settings.AddBool("int0", gAchievements[DIFFICULTY_INTERMEDIATE][0]);
	settings.AddBool("int1", gAchievements[DIFFICULTY_INTERMEDIATE][1]);
	settings.AddBool("exp0", gAchievements[DIFFICULTY_EXPERT][0]);
	settings.AddBool("exp1", gAchievements[DIFFICULTY_EXPERT][1]);
	settings.AddBool("gen0", gAchievements[3][0]);
	settings.AddBool("gen1", gAchievements[3][1]);

	settings.AddInt32("gameswon", gGamesWon);


	BFile file("/boot/home/config/settings/BeMines", B_READ_WRITE | B_ERASE_FILE | B_CREATE_FILE);
	if (file.InitCheck() != B_OK)
		return;
	settings.Flatten(&file);
}


bool
MainWindow::CanScale()
{
	if (gDifficulty == 3 && gScale == 1) {
		BRect screen = BScreen().Frame();
		BRect trect = gGameStyle->TileSize();
		uint16 maxTileWidth = uint16((screen.Width() * .9) / trect.Width()) / 2;
		maxTileWidth = maxTileWidth > 80 ? 80 : maxTileWidth;
		// Also compensate for stuff like the menu, smiley button, and titlebar height

		float usableHeight
			= (screen.Height() * .9) - 20 - 20 - gGameStyle->SmileyUp()->Bounds().Height();
		uint16 maxTileHeight = uint16(usableHeight / trect.Height()) / 2;
		maxTileHeight = maxTileHeight > 80 ? 80 : maxTileHeight;

		if (gCustomWidth > maxTileWidth || gCustomHeight > maxTileHeight) {
			BString errorMessage;
			errorMessage = (B_TRANSLATE(
				"Your current game would not fit your current screen size if scaled.\n\n"
				"Please set the custom board to no more than "
				"%d tiles wide and %d tiles high before scaling."));
			errorMessage.SetToFormat(errorMessage, maxTileWidth, maxTileHeight);
			BAlert* alert = new BAlert(
				B_TRANSLATE_SYSTEM_NAME("BeMines"), errorMessage.String(), B_TRANSLATE("OK"));
			alert->Go();
			return false;
		}
	}
	return true;
}
