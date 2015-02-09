#include "NewScoreWindow.h"
#include <Screen.h>
#include "Globals.h"

NewScoreWindow::NewScoreWindow(uint16 time, int32 difficulty)
	:	DWindow(BRect(0,0,300,300),"BeMines", B_TITLED_WINDOW,
				B_ASYNCHRONOUS_CONTROLS | B_NOT_RESIZABLE),
		fDifficulty(difficulty),
		fSeconds(time)
{
	BView *top = GetBackgroundView();

	BRect r(Bounds().InsetByCopy(10,10));

	fTextView = new BTextView(r,"textview",r,B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP);
	top->AddChild(fTextView);

	fTextView->SetText("You have made a new best time! Please enter your name.");
	fTextView->ResizeTo(fTextView->Bounds().Width(),
						fTextView->TextHeight(0,fTextView->CountLines()) + 10.0);
	r = fTextView->Bounds().InsetByCopy(10,10);
	fTextView->SetTextRect(r);

	fTextView->MakeSelectable(false);
	fTextView->MakeEditable(false);
	fTextView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	r = fTextView->Frame();
	r.OffsetBy(0,r.Height() + 10.0);
	fTextBox = new BTextControl(r,"scorename","",gBestTimes[difficulty].name.String(),
							new BMessage, B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP);
	top->AddChild(fTextBox);
	fTextBox->ResizeToPreferred();
	fTextBox->ResizeTo(Bounds().Width() - 20,fTextBox->Bounds().Height());
	fTextBox->SetDivider(0);

	fClose = new BButton(BRect(0,0,1,1),"close","Close",new BMessage(B_QUIT_REQUESTED));
	fClose->ResizeToPreferred();
	top->AddChild(fClose);

	fClose->MoveTo(Bounds().right - 10 - fClose->Bounds().Width(),
					fTextBox->Frame().bottom + 10);

	ResizeTo(Bounds().Width(),fClose->Frame().bottom + 10);

	fClose->MakeDefault(true);
	fTextBox->MakeFocus(true);
	fTextBox->TextView()->SelectAll();

	MakeCenteredOnShow(true);
}


bool
NewScoreWindow::QuitRequested(void)
{
	BString name = fTextBox->Text();
	if (name.CountChars() < 1)
		name = "Anonymous";

	gBestTimes[fDifficulty].time = fSeconds;
	gBestTimes[fDifficulty].name = name;

	return true;
}
