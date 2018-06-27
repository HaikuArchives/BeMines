#include "CustomWindow.h"

#include <Alert.h>
#include <Application.h>
#include <Button.h>
#include <LayoutBuilder.h>
#include <Screen.h>
#include <stdlib.h>

#include "EscapeCancelFilter.h"
#include "GameStyle.h"
#include "Globals.h"
#include "MainWindow.h"

#define M_SET_CUSTOM 'stcs'
#define M_CHECK_VALUE 'ckvl'

CustomWindow::CustomWindow(void)
	:	DWindow(BRect(100,100,300,300),"Custom Level Settings",
				B_TITLED_WINDOW, B_NOT_RESIZABLE | B_NOT_ZOOMABLE |
								B_NOT_MINIMIZABLE)
{
	AddCommonFilter(new EscapeCancelFilter());

	BView *top = GetBackgroundView();

	BString s;
	s << (int)gCustomWidth;
	fWidth = new BTextControl("width","Width: ",s.String(),
								new BMessage(M_CHECK_VALUE));
	MakeNumberBox(fWidth);

	s = "";
	s << (int)gCustomHeight;
	fHeight = new BTextControl("height","Height: ",s.String(),
								new BMessage(M_CHECK_VALUE));
	MakeNumberBox(fHeight);

	s = "";
	s << (int)gCustomMines;
	fMines = new BTextControl("mines","Mines: ",s.String(),
								new BMessage(M_CHECK_VALUE));
	MakeNumberBox(fMines);

	// Initially set label to Cancel so that the buttons are the same size
	BButton *ok = new BButton("ok","OK",new BMessage(M_SET_CUSTOM));
	ok->MakeDefault(true);

	BButton *cancel = new BButton("cancel","Cancel",
								new BMessage(B_QUIT_REQUESTED));

	//ResizeTo(Bounds().Width(), fMines->Frame().bottom + 20.0 + ok->Frame().Height());

	BLayoutBuilder::Group<>(top, B_VERTICAL, 0)
		.SetInsets(B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING)
		.AddGrid()
			.Add(fWidth->CreateLabelLayoutItem(), 0, 0)
			.Add(fWidth->CreateTextViewLayoutItem(), 1, 0)
			.Add(fHeight->CreateLabelLayoutItem(), 0, 1)
			.Add(fHeight->CreateTextViewLayoutItem(), 1, 1)
			.Add(fMines->CreateLabelLayoutItem(), 0, 2)
			.Add(fMines->CreateTextViewLayoutItem(), 1, 2)
		.End()
		.AddStrut(B_USE_DEFAULT_SPACING)
		.AddGroup(B_HORIZONTAL)
			.AddGlue()
			.Add(cancel)
			.Add(ok)
		.End();


	MakeCenteredOnShow(true);
	fWidth->MakeFocus(true);
}


void
CustomWindow::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		case M_SET_CUSTOM:
		{
			CheckValues();
			gCustomWidth = atoi(fWidth->Text());
			gCustomHeight = atoi(fHeight->Text());
			gCustomMines = atoi(fMines->Text());

			for (int32 i = 0; i < be_app->CountWindows(); i++)
			{
				if (strcmp(be_app->WindowAt(i)->Title(),"BeMines") == 0)
				{
					BMessage setmsg(M_SET_DIFFICULTY);
					setmsg.AddInt32("level",DIFFICULTY_CUSTOM);
					be_app->WindowAt(i)->PostMessage(&setmsg);
					break;
				}
			}

			PostMessage(B_QUIT_REQUESTED);

			break;
		}
		case M_CHECK_VALUE:
		{
			CheckValues();
			break;
		}
		default:
		{
			DWindow::MessageReceived(msg);
			break;
		}
	}
}


void
CustomWindow::MakeNumberBox(BTextControl *box)
{
	char c;
	for (c = 32; c < 48; c++)
		box->TextView()->DisallowChar(c);
	for (c = 58; c < 127; c++)
		box->TextView()->DisallowChar(c);
}


void
CustomWindow::CheckValues(void)
{
	BRect screen = BScreen().Frame();
	BRect trect = gGameStyle->TileSize();
	uint16 maxTileWidth = uint16((screen.Width() * .9) / trect.Width());

	// Also compensate for stuff like the menu, smiley button, and titlebar height

	float usableHeight = (screen.Height() * .9) - 20 - 20 -
							gGameStyle->SmileyUp()->Bounds().Height();
	uint16 maxTileHeight = uint16(usableHeight / trect.Height());

	uint16 width,height;
	BString s = fWidth->Text();
	if (s.CountChars() < 1)
	{
		fWidth->SetText("6");
		s = "6";
	}

	width = atoi(s.String());
	if (width < 6)
	{
		width = 6;
		fWidth->SetText("6");
	}
	else if (width > maxTileWidth)
	{
		BString errmsg;
		errmsg << "For your current screen size and theme, you can have "
				"a width of up to " << (int)maxTileWidth << " tiles.";
		BAlert *alert = new BAlert("BeMines",errmsg.String(),"OK");
		alert->Go();
		s = "";
		s << (int)maxTileWidth;
		fWidth->SetText(s.String());
		width = maxTileWidth;
	}

	s = fHeight->Text();
	if (s.CountChars() < 1)
	{
		fHeight->SetText("6");
		s = "6";
	}

	height = atoi(s.String());
	if (height < 6)
	{
		height = 6;
		fHeight->SetText("6");
	}
	else if (height > maxTileHeight)
	{
		BString errmsg;
		errmsg << "For your current screen size and theme, you can have "
				"a height of up to " << (int)maxTileHeight << " tiles.";
		BAlert *alert = new BAlert("BeMines",errmsg.String(),"OK");
		alert->Go();
		s = "";
		s << (int)maxTileHeight;
		fHeight->SetText(s.String());
		height = maxTileHeight;
	}

	uint16	count = 0,
			maxMines = ((width * height) - 1);
	s = fMines->Text();
	if (s.CountChars() < 1)
	{
		fMines->SetText("1");
		s = "1";
	}
	else if (count > maxMines)
	{
		BString errmsg;
		errmsg << "For your chosen width and height, you can't have more than "
			<< (int)maxMines << " mines.";
		BAlert *alert = new BAlert("BeMines",errmsg.String(),"OK");
		alert->Go();
		s = "";
		s << (int)maxMines;
		fMines->SetText(s.String());
	}

}
