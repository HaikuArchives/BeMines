#include "FieldView.h"
#include <Alert.h>
#include <Application.h>
#include <Catalog.h>
#include <Message.h>
#include <Path.h>
#include <Roster.h>
#include <stdio.h>

#include "GameStyle.h"
#include "Globals.h"
#include "MainWindow.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "FieldWindow"

FieldView::FieldView(int32 level)
	:	BView(BRect(0,0,1,1),"fieldview",B_FOLLOW_LEFT | B_FOLLOW_TOP,
				B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE | B_ASYNCHRONOUS_CONTROLS),
		fField(NULL),
		fTracking(false),
		fFlagCount(0),
		fMainWin(NULL),
		fPauseMode(false),
		fWinPlayer(NULL),
		fLosePlayer(NULL),
		fClickPlayer(NULL)
{
	SetDifficulty(level);
	SetDrawingMode(B_OP_ALPHA);

	app_info ai;
	be_app->GetAppInfo(&ai);
	BPath path(&ai.ref);
	path.GetParent(&path);
	path.Append("themes");
	fThemePath = path.Path();
	SetSoundRefs();
}


FieldView::~FieldView(void)
{
	delete fWinPlayer;
	delete fLosePlayer;
	delete fClickPlayer;
}


void
FieldView::AttachedToWindow(void)
{
	fMainWin = (MainWindow*)Window();
}


void
FieldView::Draw(BRect update)
{
	if (fPauseMode)
	{
		SetHighColor(128,128,128);
		FillRect(Bounds());
		SetHighColor(255,255,255);
		SetDrawingMode(B_OP_ALPHA);

		BFont font;
		font.SetSize(28.0);
		font_height fh;
		font.GetHeight(&fh);
		float width = font.StringWidth(B_TRANSLATE("PAUSED"));
		SetFont(&font);
		float y = (Bounds().Height() / 3) + fh.ascent;
		DrawString(B_TRANSLATE("PAUSED"),BPoint( (Bounds().Width() - width) / 2.0,y));
		SetFont(be_plain_font);
		y += fh.descent + fh.leading + 10.0;
		width = be_plain_font->StringWidth(B_TRANSLATE("Click to resume"));
		DrawString(B_TRANSLATE("Click to resume"),BPoint( (Bounds().Width() - width) / 2.0,y));
	}
	else
	{
		// The corresponding box numbers for the update rectangle
		uint16 leftx,rightx,topy,bottomy;

		BRect r = gGameStyle->TileSize();

		// We have to do this because of a bug in BRect::Width()/Height()
		// A rect of (0,0,23,23) will return a Width/Height of 23, not 24.
		int32 tilewidth = r.IntegerWidth() + 1;
		int32 tileheight = r.IntegerHeight() + 1;
		leftx = uint16(update.left / tilewidth);
		rightx = uint16(update.right / tilewidth);
		topy = uint16(update.top / tileheight);
		bottomy = uint16(update.bottom / tileheight);

		for (uint16 y = topy; y <= bottomy; y++)
			for (uint16 x = leftx; x <= rightx; x++)
			{
				bool down = false;
				if (fTracking && fSelection.x == x && fSelection.y == y)
					down = true;
				DrawBox(x,y, down);
			}

		if (gGameState == GAME_OVER)
		{
			SetHighColor(64,64,64,72);
			FillRect(Bounds());
		}
	}
}


void
FieldView::DrawBox(uint16 x, uint16 y, bool down)
{
	BoxState state = fField->GetState(IntPoint(x,y));

	BRect r(gGameStyle->TileRect(x,y));

	switch (state)
	{
		case BOX_UNMARKED:
		{
			if (down)
				DrawBitmap(gGameStyle->BoxDownSprite(),r);
			else
				DrawBitmap(gGameStyle->BoxSprite(),r);
			break;
		}
		case BOX_HIT:
		{
			DrawBitmap(gGameStyle->HitSprite(),r);
			break;
		}
		case BOX_REVEALED:
		{
			uint8 count = fField->TileDigit(IntPoint(x,y));
			if (count > 0)
				DrawBitmap(gGameStyle->NumberSprite(count),r);
			else
				DrawBitmap(gGameStyle->BaseSprite(),r);
			break;
		}
		case BOX_MARKED:
		{
			DrawBitmap(gGameStyle->FlagSprite(),r);
			break;
		}
		case BOX_MINE:
		{
			DrawBitmap(gGameStyle->MineSprite(),r);
			break;
		}
		case BOX_NOTMINE:
		{
			DrawBitmap(gGameStyle->NotMineSprite(),r);
			break;
		}
		case BOX_QUESTION:
		{
			DrawBitmap(gGameStyle->QuestionSprite(),r);
			break;
		}
		default:
		{
			break;
		}
	}

	if (gCheatMode && fField->IsMine(IntPoint(x,y)))
	{
		SetHighColor(255,0,0,64);
		FillRect(r);
	}

}


uint16
FieldView::MineCount(void)
{
	return fField->MineCount();
}


uint16
FieldView::FlagCount(void)
{
	return fFlagCount;
}


void
FieldView::MouseDown(BPoint pt)
{
	if (fPauseMode || gGameState == GAME_OVER)
		return;

	SetMouseEventMask(B_POINTER_EVENTS, B_LOCK_WINDOW_FOCUS);
	fTracking = true;

	GetMouse(&pt, &fButtons, true);
	if (gGameState == GAME_STOPPED)
	{
		Window()->PostMessage(M_START_TIMER);
		IntPoint tilePt;
		BRect r(gGameStyle->TileSize());
		tilePt.x = uint16(pt.x / (r.Width() + 1.0));
		tilePt.y = uint16(pt.y / (r.Height() + 1.0));

		if (fButtons == B_PRIMARY_MOUSE_BUTTON)
			fField->TeleportMine(tilePt);
		Invalidate();
	}
	if (gPlaySounds && gGameState != GAME_OVER)
		fClickPlayer->StartPlaying();

	SelectTile(pt);
	fMainWin->SetFace(FACE_WORRY);
}


void
FieldView::MouseMoved(BPoint pt, uint32 transit, const BMessage *msg)
{
	if (!fTracking)
		return;

	if (Bounds().Contains(pt))
		SelectTile(pt);
}


void
FieldView::MouseUp(BPoint pt)
{
	if (fPauseMode)
		Window()->PostMessage(M_PAUSE_GAME);
	if (!fTracking)
		return;

	SelectTile(pt);

	if (Bounds().Contains(pt))
		InvokeTile(pt, fButtons);

	fTracking = false;
}


void
FieldView::InvokeTile(const BPoint &pt, uint32 button)
{
	// This function handles click events on the grid
	IntPoint tilePt;
	BRect r(gGameStyle->TileSize());
	tilePt.x = uint16(pt.x / (r.Width() + 1.0));
	tilePt.y = uint16(pt.y / (r.Height() + 1.0));

	InvokeTile(tilePt,button);
}


void
FieldView::InvokeTile(const IntPoint &tilePt, uint32 button)
{
	if (tilePt.x >= fField->Width() || tilePt.y >= fField->Height())
		return;

	if (gGameState == GAME_OVER)
		return;

	fMainWin->SetFace(FACE_NORMAL);

	if (button & B_PRIMARY_MOUSE_BUTTON)
	{
		int32 mod = modifiers();

		if ( (button & B_SECONDARY_MOUSE_BUTTON) || (mod & B_SHIFT_KEY) )
			NumberReveal(tilePt);
		else if (mod & B_COMMAND_KEY)
			DoSonar(tilePt);
		else
			ClickBox(tilePt);
	}
	else if (button == B_SECONDARY_MOUSE_BUTTON)
		ToggleBox(tilePt);
	else if (button == B_TERTIARY_MOUSE_BUTTON)
		DoSonar(tilePt);
}


void
FieldView::SelectTile(const BPoint &pt)
{
	BRect r(gGameStyle->TileSize());

	IntPoint oldSelection(fSelection.x,fSelection.y);
	fSelection.x = uint16(pt.x / (r.Width() + 1.0));
	fSelection.y = uint16(pt.y / (r.Height() + 1.0));

	Invalidate(gGameStyle->TileRect(oldSelection.x,oldSelection.y));
	Invalidate(gGameStyle->TileRect(fSelection.x,fSelection.y));
	Window()->UpdateIfNeeded();
}


IntPoint
FieldView::GetSelection(void) const
{
	return fSelection;
}


void
FieldView::ClickBox(const IntPoint &pt)
{
	if (pt.x >= fField->Width() || pt.y >= fField->Height())
		return;

	if (fField->TileDigit(pt) == 0)
	{
		FloodReveal(pt);
		Window()->UpdateIfNeeded();
	}
	else
		OpenBox(pt);
}


void
FieldView::ToggleBox(const IntPoint &pt)
{
	switch (fField->GetState(pt))
	{
		case BOX_MARKED:
		{
			fField->SetState(pt,BOX_QUESTION);
			fFlagCount--;
			Window()->PostMessage(M_UPDATE_COUNT);
			break;
		}
		case BOX_QUESTION:
		{
			fField->SetState(pt,BOX_UNMARKED);
			break;
		}
		case BOX_UNMARKED:
		{
			if (fFlagCount == fField->MineCount())
			{
				BAlert *alert = new BAlert(B_TRANSLATE_SYSTEM_NAME("BeMines"),B_TRANSLATE("You have already set as many "
									"flags as there are mines. Unflag a tile "
									"before trying to flag another one."),B_TRANSLATE("OK"));
				alert->Go();
				break;
			}

			fField->SetState(pt,BOX_MARKED);
			fFlagCount++;
			Window()->PostMessage(M_UPDATE_COUNT);

			if (CheckWin())
			{
				DoWin();
				break;
			}
			break;
		}
		default:
		{
			break;
		}
	}
	Invalidate(gGameStyle->TileRect(pt.x,pt.y));
}


void
FieldView::OpenBox(const IntPoint &pt)
{
	if (pt.x >= fField->Width() || pt.y >= fField->Height() )
		return;

	if (fField->GetState(pt) == BOX_UNMARKED)
	{
		if (fField->IsMine(pt))
		{
			fField->SetState(pt,BOX_HIT);
			DoLose();
		}
		else
		{
			fField->SetState(pt,BOX_REVEALED);
			if (CheckWin())
				DoWin();
		}
	}
	Invalidate(gGameStyle->TileRect(pt.x,pt.y));
}


void
FieldView::FloodReveal(IntPoint pt)
{
	OpenBox(pt);

	IntPoint temppt;
	if (pt.y > 0)
	{
		if (pt.x > 0)
		{
			temppt = pt;
			temppt.x--;
			temppt.y--;
			if (fField->GetState(temppt) == BOX_UNMARKED)
			{
				if (fField->TileDigit(temppt) == 0)
					FloodReveal(temppt);
				else
					OpenBox(temppt);
			}
		}

		temppt = pt;
		temppt.y--;
		if (fField->GetState(temppt) == BOX_UNMARKED)
		{
			if (fField->TileDigit(temppt) == 0)
				FloodReveal(temppt);
			else
				OpenBox(temppt);
		}

		if (pt.x < (fField->Width() - 1))
		{
			temppt = pt;
			temppt.x++;
			temppt.y--;
			if (fField->GetState(temppt) == BOX_UNMARKED)
			{
				if (fField->TileDigit(temppt) == 0)
					FloodReveal(temppt);
				else
					OpenBox(temppt);
			}
		}
	}

	if (pt.x > 0)
	{
		temppt = pt;
		temppt.x--;
		if (fField->GetState(temppt) == BOX_UNMARKED)
		{
			if (fField->TileDigit(temppt) == 0)
				FloodReveal(temppt);
			else
				OpenBox(temppt);
		}
	}

	if (pt.x < (fField->Width() - 1))
	{
		temppt = pt;
		temppt.x++;
		if (fField->GetState(temppt) == BOX_UNMARKED)
		{
			if (fField->TileDigit(temppt) == 0)
				FloodReveal(temppt);
			else
				OpenBox(temppt);
		}
	}

	if (pt.y < (fField->Height() - 1))
	{
		if (pt.x > 0)
		{
			temppt = pt;
			temppt.x--;
			temppt.y++;
			if (fField->GetState(temppt) == BOX_UNMARKED)
			{
				if (fField->TileDigit(temppt) == 0)
					FloodReveal(temppt);
				else
					OpenBox(temppt);
			}
		}

		temppt = pt;
		temppt.y++;
		if (fField->GetState(temppt) == BOX_UNMARKED)
		{
			if (fField->TileDigit(temppt) == 0)
				FloodReveal(temppt);
			else
				OpenBox(temppt);
		}

		if (pt.x < (fField->Width() - 1))
		{
			temppt = pt;
			temppt.x++;
			temppt.y++;
			if (fField->GetState(temppt) == BOX_UNMARKED)
			{
				if (fField->TileDigit(temppt) == 0)
					FloodReveal(temppt);
				else
					OpenBox(temppt);
			}
		}
	}
}


void
FieldView::NumberReveal(IntPoint pt)
{
	if (fField->GetState(pt) != BOX_REVEALED)
	{
		Invalidate(gGameStyle->TileRect(pt.x,pt.y));
		return;
	}

	if (pt.x > fField->Width() || pt.y > fField->Height())
		return;

	uint8 mineCount = fField->TileDigit(pt);

	uint8 flagCount = 0;

	if (pt.y > 0)
	{
		if (pt.x > 0 && fField->GetState(pt.x - 1, pt.y - 1) == BOX_MARKED)
			flagCount++;

		if (fField->GetState(pt.x, pt.y - 1) == BOX_MARKED)
			flagCount++;

		if (pt.x < (fField->Width() - 1) && fField->GetState(pt.x + 1, pt.y - 1) == BOX_MARKED)
			flagCount++;
	}

	if (pt.x > 0 && fField->GetState(pt.x - 1, pt.y) == BOX_MARKED)
		flagCount++;

	if (pt.x < (fField->Width() - 1) && fField->GetState(pt.x + 1, pt.y) == BOX_MARKED)
		flagCount++;

	if (pt.y < (fField->Height() - 1))
	{
		if (pt.x > 0 && fField->GetState(pt.x - 1, pt.y + 1) == BOX_MARKED)
			flagCount++;

		if (fField->GetState(pt.x, pt.y + 1) == BOX_MARKED)
			flagCount++;

		if (pt.x < (fField->Width() - 1) && fField->GetState(pt.x + 1, pt.y + 1) == BOX_MARKED)
			flagCount++;
	}

	if (mineCount == flagCount)
	{
		ClickBox(IntPoint(pt.x - 1, pt.y - 1));
		ClickBox(IntPoint(pt.x, pt.y - 1));
		ClickBox(IntPoint(pt.x + 1, pt.y - 1));

		ClickBox(IntPoint(pt.x - 1, pt.y));
		ClickBox(IntPoint(pt.x + 1, pt.y));

		ClickBox(IntPoint(pt.x - 1, pt.y + 1));
		ClickBox(IntPoint(pt.x, pt.y + 1));
		ClickBox(IntPoint(pt.x + 1, pt.y + 1));
	}
}


void
FieldView::DoSonar(IntPoint pt)
{
	bool revealedNewBoxes = fField->FireSonar(pt);
	Invalidate();

	if(revealedNewBoxes == true)
		Window()->PostMessage(M_SONAR_PENALTY);

	fFlagCount = fField->TilesInState(BOX_MARKED);
	Window()->PostMessage(M_UPDATE_COUNT);
}


bool
FieldView::CheckWin(void)
{
	for (uint16 y = 0; y < fField->Height(); y++)
		for (uint16 x = 0; x < fField->Width(); x++)
		{
			BoxState state = fField->GetState(IntPoint(x,y));
			if (state != BOX_REVEALED && !fField->IsMine(x,y))
				return false;
		}
	return true;
}


void
FieldView::DoWin(void)
{
	if (gPlaySounds)
		fWinPlayer->StartPlaying();

	gGameState = GAME_OVER;
	fMainWin->SetFace(FACE_WIN);
	Invalidate();
}

void
FieldView::DoLose(void)
{
	if (gPlaySounds)
		fLosePlayer->StartPlaying();

	fMainWin->SetFace(FACE_LOSE);
	gGameState = GAME_OVER;

	// I want the animated reveal to take no more than about 1/4 to 1/3 sec and
	// 580 usec is a rough (educated) guess of redraw time based on my own machine.
	// This should yield an acceptable result
	int32 tileCount = fField->Height() * fField->Width();
	bigtime_t snoozeTime = (300000 / tileCount) - 580;
	for (uint16 y = 0; y < fField->Height(); y++)
		for (uint16 x = 0; x < fField->Width(); x++)
		{
			IntPoint pt(x,y);
			BoxState state = fField->GetState(pt);
			switch (state)
			{
				case BOX_QUESTION:
				case BOX_UNMARKED:
				{
					if (fField->IsMine(pt))
						fField->SetState(pt,BOX_MINE);
					else
						fField->SetState(pt,BOX_REVEALED);
					break;
				}
				case BOX_MARKED:
				{
					if (fField->IsMine(pt))
						fField->SetState(pt,BOX_MINE);
					else
						fField->SetState(pt,BOX_NOTMINE);
				}
				default:
				{
					break;
				}
			}

			if (tileCount < 500)
			{
				Invalidate(gGameStyle->TileRect(pt.x,pt.y));
				Window()->UpdateIfNeeded();
				if (snoozeTime > 100)
					snooze(snoozeTime);
			}
		}

	if (tileCount >= 500)
		Invalidate();
}


void
FieldView::SetDifficulty(uint8 level)
{
	switch (level)
	{
		case DIFFICULTY_BEGINNER:
		{
			fDifficulty = level;
			SetBoard(8,8,10);
			break;
		}
		case DIFFICULTY_INTERMEDIATE:
		{
			fDifficulty = level;
			SetBoard(16,16,40);
			break;
		}
		case DIFFICULTY_EXPERT:
		{
			fDifficulty = level;
			SetBoard(30,16,99);
			break;
		}
		case DIFFICULTY_CUSTOM:
		{
			fDifficulty = level;
			SetBoard(gCustomWidth,gCustomHeight,gCustomMines);
			break;
		}
		default:
		{
			return;
			break;
		}
	}
}


void
FieldView::SetBoard(uint16 width, uint16 height, uint16 count)
{
	delete fField;

	if (width < 2)
		width = 2;
	if (height < 2)
		height = 2;

	BRect r = gGameStyle->TileSize();
	ResizeTo(((r.Width() + 1.0) * width) - 1.0,
			((r.Height() + 1.0) * height) - 1.0);

	gGameState = 0;
	fFlagCount = 0;

	fField = new Minefield(width,height);
	fField->Reset(count);
	if (Window())
		Window()->PostMessage(M_SIZE_CHANGED);
	Invalidate();
}


void
FieldView::StyleChanged(void)
{
	SetSoundRefs();
	BRect r = gGameStyle->TileSize();
	ResizeTo(((r.Width() + 1.0) * fField->Width()) - 1.0,
			((r.Height() + 1.0) * fField->Height()) - 1.0);

	if (Window())
		Window()->PostMessage(M_SIZE_CHANGED);
	Invalidate();
}


void
FieldView::SetPauseMode(bool paused)
{
	fPauseMode = paused;
	Invalidate();
}


int8
FieldView::GetGameState(void) const
{
	return gGameState;
}


void
FieldView::SetSoundRefs(void)
{
	BPath winpath(fThemePath.String());
	winpath.Append(gGameStyle->StyleName());
	winpath.Append("win.ogg");
	entry_ref winSoundRef;
	BEntry(winpath.Path()).GetRef(&winSoundRef);
	fWinPlayer = new BFileGameSound(&winSoundRef, false);
	fWinPlayer->Preload();

	BPath losepath(fThemePath.String());
	losepath.Append(gGameStyle->StyleName());
	losepath.Append("lose.ogg");
	entry_ref loseSoundRef;
	BEntry(losepath.Path()).GetRef(&loseSoundRef);
	fLosePlayer = new BFileGameSound(&loseSoundRef, false);
	fLosePlayer->Preload();

	BPath clickpath(fThemePath.String());
	clickpath.Append(gGameStyle->StyleName());
	clickpath.Append("click.ogg");
	entry_ref clickSoundRef;
	BEntry(clickpath.Path()).GetRef(&clickSoundRef);
	fClickPlayer = new BFileGameSound(&clickSoundRef, false);
	fClickPlayer->Preload();
}
