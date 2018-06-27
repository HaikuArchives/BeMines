#include "NewScoreWindow.h"
#include <LayoutBuilder.h>
#include <Screen.h>
#include <StringView.h>
#include "Globals.h"

NewScoreWindow::NewScoreWindow(uint16 time, int32 difficulty)
	:	DWindow(BRect(0,0,300,300),"BeMines", B_TITLED_WINDOW,
				B_ASYNCHRONOUS_CONTROLS | B_NOT_RESIZABLE
				| B_AUTO_UPDATE_SIZE_LIMITS),
		fDifficulty(difficulty),
		fSeconds(time)
{
	BView *top = GetBackgroundView();

	fTextBox = new BTextControl("scorename","",gBestTimes[difficulty].name.String(), NULL);
	fTextBox->MakeFocus(true);
	fTextBox->TextView()->SelectAll();

	BLayoutBuilder::Group<>(top, B_VERTICAL, B_USE_SMALL_INSETS)
		.SetInsets(B_USE_WINDOW_INSETS, B_USE_WINDOW_INSETS,
			B_USE_WINDOW_INSETS, B_USE_WINDOW_INSETS)
		.Add(new BStringView("label1", "You have made a new best time!"))
		.Add(new BStringView("lable2","Please enter your name."))
		.Add(fTextBox)
		.AddGroup(B_HORIZONTAL)
			.AddGlue()
			.Add(fClose = new BButton("close","Close",new BMessage(B_QUIT_REQUESTED)));

	fClose->MakeDefault(true);

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
