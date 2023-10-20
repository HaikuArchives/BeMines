/*
 * Copyright 2007, DarkWyrm
 * Copyright 2013-2023, HaikuArchives Team
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Humdinger
 */

#include <Button.h>
#include <Catalog.h>
#include <LayoutBuilder.h>
#include <StringView.h>

#include "HelpWindow.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "HelpWindow"


HelpText::HelpText(const char* name, const char* text)
	:
	BTextView(name)
{
	SetText(text);
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	SetLowUIColor(B_PANEL_BACKGROUND_COLOR);
	SetViewUIColor(B_PANEL_BACKGROUND_COLOR);
	MakeEditable(false);
	MakeSelectable(false);
	MakeResizable(false);
}


HelpWindow::HelpWindow()
	:
	DWindow(BRect(), B_TRANSLATE("Help"), B_TITLED_WINDOW,
		B_NOT_RESIZABLE | B_NOT_ZOOMABLE | B_AUTO_UPDATE_SIZE_LIMITS | B_CLOSE_ON_ESCAPE)
{
	BView* top = GetBackgroundView();

	HelpText* howto = new HelpText("howto",
		B_TRANSLATE("The goal of the game is to flag all mines that are hidden on the "
					"board. The counter on the top left shows the number of still hidden "
					"mines, the one on the right counts up the seconds."));

	BStringView* header1 = new BStringView("header1", B_TRANSLATE("Left click:"));
	HelpText* text1 = new HelpText(
		"text1", B_TRANSLATE("Reveal what's under a tile. If it's a mine, the game is over!"));

	BStringView* header2 = new BStringView("header2", B_TRANSLATE("Right click:"));
	HelpText* text2 = new HelpText("text2",
		B_TRANSLATE("Flag a tile as a mine, set a question mark if you're unsure, or unmark "
					"an already flagged tile."));

	BStringView* header3
		= new BStringView("header3", B_TRANSLATE("Middle click (or ALT + left click):"));
	HelpText* text3 = new HelpText("text3",
		B_TRANSLATE("A sonar ping reveals all mines in a 3x3 tile area without the danger "
					"of setting them off. It comes with a 20 second cost added to your time."));

	BStringView* header4 = new BStringView(
		"header4", B_TRANSLATE("Clicking with both buttons (or SHIFT + left click):"));
	header4->SetExplicitMinSize(BSize(450, B_SIZE_UNSET));
	HelpText* text4 = new HelpText("text4",
		B_TRANSLATE("If the tile is a number and the appropriate number of mines around it "
					"have been flagged, it will clear all the other tiles around the number. "
					"Of course, if you've made a mistake, you'll probably set off a mine..."));

	BButton* close = new BButton("ok", B_TRANSLATE("OK"), new BMessage(B_QUIT_REQUESTED));

	BLayoutBuilder::Group<>(top, B_VERTICAL, 0)
		.SetInsets(B_USE_WINDOW_INSETS)
		.AddGroup(B_VERTICAL, 0)
			.Add(howto)
			.AddStrut(B_USE_DEFAULT_SPACING)
			.Add(header1)
			.Add(text1)
			.AddStrut(B_USE_DEFAULT_SPACING)
			.Add(header2)
			.Add(text2)
			.AddStrut(B_USE_DEFAULT_SPACING)
			.Add(header3)
			.Add(text3)
			.AddStrut(B_USE_DEFAULT_SPACING)
			.Add(header4)
			.Add(text4)
		.End()
			.AddStrut(B_USE_BIG_SPACING)
			.AddGroup(B_HORIZONTAL)
			.AddGlue()
			.Add(close)
		.End();

	header1->SetFont(be_bold_font);
	header2->SetFont(be_bold_font);
	header3->SetFont(be_bold_font);
	header4->SetFont(be_bold_font);

	MakeCenteredOnShow(true);
	close->MakeDefault(true);
	ResizeToPreferred();
}
