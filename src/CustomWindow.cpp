#include "CustomWindow.h"

#include <Application.h>
#include <Button.h>
#include <Catalog.h>
#include <LayoutBuilder.h>
#include <Screen.h>
#include <SeparatorView.h>
#include <StringFormat.h>

#include <stdlib.h>

#include "GameStyle.h"
#include "Globals.h"
#include "MainWindow.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "CustomWindow"

#define M_CANCEL_CUSTOM	'cncs'
#define M_SET_CUSTOM	'stcs'
#define M_CHECK_VALUE	'ckvl'

static const uint16 kMinTiles = 6;


CustomWindow::CustomWindow(BRect frame)
	:
	BWindow(BRect(), B_TRANSLATE("Level settings"), B_TITLED_WINDOW,
		B_NOT_RESIZABLE | B_NOT_ZOOMABLE | B_NOT_CLOSABLE
		| B_NOT_MINIMIZABLE | B_AUTO_UPDATE_SIZE_LIMITS | B_CLOSE_ON_ESCAPE)
{
	fOrigWidth = gCustomWidth;
	fOrigHeight = gCustomHeight;
	fOrigMines = gCustomMines;

	fWidth = new BSpinner("width", B_TRANSLATE("Width:"), new BMessage(M_SET_CUSTOM));
	fWidth->SetMinValue(kMinTiles);
	fWidth->SetMaxValue(GetMaxWidth());
	fWidth->SetValue(gCustomWidth);

	fHeight = new BSpinner("height", B_TRANSLATE("Height:"), new BMessage(M_SET_CUSTOM));
	fHeight->SetMinValue(kMinTiles);
	fHeight->SetMaxValue(GetMaxHeight());
	fHeight->SetValue(gCustomHeight);

	fMines = new BSpinner("mines", B_TRANSLATE("Mines:"), new BMessage(M_SET_CUSTOM));
	fMines->SetMinValue(1);
	fMines->SetMaxValue(gCustomWidth * gCustomHeight  - 1);
	fMines->SetValue(gCustomMines);

	BButton *ok = new BButton("ok",B_TRANSLATE("OK"), new BMessage(B_QUIT_REQUESTED));
	ok->MakeDefault(true);

	BButton *cancel = new BButton("cancel", B_TRANSLATE("Cancel"), new BMessage(M_CANCEL_CUSTOM));

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.AddGrid()
		.SetInsets(B_USE_WINDOW_INSETS, B_USE_WINDOW_INSETS,
			B_USE_WINDOW_INSETS, B_USE_WINDOW_INSETS)
			.Add(fWidth->CreateLabelLayoutItem(), 0, 0)
			.Add(fWidth->CreateTextViewLayoutItem(), 1, 0)
			.Add(fHeight->CreateLabelLayoutItem(), 0, 1)
			.Add(fHeight->CreateTextViewLayoutItem(), 1, 1)
			.Add(fMines->CreateLabelLayoutItem(), 0, 2)
			.Add(fMines->CreateTextViewLayoutItem(), 1, 2)
		.End()
		.Add(new BSeparatorView(B_HORIZONTAL))
		.AddGroup(B_HORIZONTAL)
			.SetInsets(B_USE_WINDOW_INSETS, B_USE_WINDOW_INSETS,
				B_USE_WINDOW_INSETS, B_USE_WINDOW_INSETS)
			.Add(cancel)
			.Add(ok)
		.End();

	fWidth->MakeFocus(true);

	if (frame.IsValid()) {
		frame.OffsetBy(20.0, 0.0);
		MoveTo(frame.RightTop());
		MoveOnScreen(B_MOVE_IF_PARTIALLY_OFFSCREEN);
	} else
		CenterOnScreen();
}


void
CustomWindow::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		case M_SET_CUSTOM:
		{
			gCustomWidth = fWidth->Value();
			gCustomHeight = fHeight->Value();

			uint16 mines = fMines->Value();
			uint16 maxMines = gCustomWidth * gCustomHeight  - 1;
			fMines->SetMaxValue(maxMines);

			gCustomMines = fMines->Value();

			UpdateDifficulty();

			break;
		}
		case M_CANCEL_CUSTOM:
		{
			gCustomWidth = fOrigWidth;
			gCustomHeight = fOrigHeight;
			gCustomMines = fOrigMines;

			UpdateDifficulty();
			PostMessage(B_QUIT_REQUESTED);

			break;
		}

		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}


uint16
CustomWindow::GetMaxHeight()
{
	BRect screen = BScreen().Frame();
	BRect tileRect = gGameStyle->TileSize();
	// Also compensate for stuff like the menu, smiley button, and titlebar height
	float usableHeight = (screen.Height() * .9) - 20 - 20 -
		gGameStyle->SmileyUp()->Bounds().Height();
	uint16 maxTileHeight = uint16(usableHeight / tileRect.Height());
	maxTileHeight = maxTileHeight > 80 ? 80 : maxTileHeight;

	return maxTileHeight;
}


uint16
CustomWindow::GetMaxWidth()
{
	BRect screen = BScreen().Frame();
	BRect trect = gGameStyle->TileSize();
	uint16 maxTileWidth = uint16((screen.Width() * .9) / trect.Width());
	maxTileWidth = maxTileWidth > 80 ? 80 : maxTileWidth;

	return maxTileWidth;
}


void
CustomWindow::UpdateDifficulty()
{
	for (int32 i = 0; i < be_app->CountWindows(); i++) {
		if (strcmp(be_app->WindowAt(i)->Title(), B_TRANSLATE_SYSTEM_NAME("BeMines")) == 0) {
			BMessage setmsg(M_SET_DIFFICULTY);
			setmsg.AddInt32("level", DIFFICULTY_CUSTOM);
			be_app->WindowAt(i)->PostMessage(&setmsg);
			break;
		}
	}
}
