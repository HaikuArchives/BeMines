#include "ScoreWindow.h"
#include <Button.h>
#include <Catalog.h>
#include <LayoutBuilder.h>
#include "EscapeCancelFilter.h"
#include "Globals.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "ScoreWindow"

#define M_RESET_SCORES 'rssc'

ScoreWindow::ScoreWindow(void)
	:	DWindow(BRect(100,500,100,400),B_TRANSLATE("High scores"),B_TITLED_WINDOW,
				B_NOT_RESIZABLE | B_NOT_ZOOMABLE | B_AUTO_UPDATE_SIZE_LIMITS)
{
	AddCommonFilter(new EscapeCancelFilter());

	BView *top = GetBackgroundView();

	BButton *reset = new BButton("reset", B_TRANSLATE("Reset"), new BMessage(M_RESET_SCORES));
	BButton *close = new BButton("ok", B_TRANSLATE("OK"), new BMessage(B_QUIT_REQUESTED));

	BStringView *begLabel, *intLabel, *expLabel;

	BLayoutBuilder::Group<>(top, B_VERTICAL, 0)
		.SetInsets(B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING)
		.AddGrid(B_USE_DEFAULT_SPACING, B_USE_SMALL_SPACING)
			.Add(begLabel = new BStringView("beglabel", B_TRANSLATE("Beginner:")), 0, 0)
			.Add(fBegScore = new BStringView("begscore",""), 1, 0)
			.Add(intLabel = new BStringView("intlabel", B_TRANSLATE("Intermediate:")), 0,1)
			.Add(fIntScore = new BStringView("intscore",""), 1, 1)
			.Add(expLabel = new BStringView("explabel", B_TRANSLATE("Expert:")), 0, 2)
			.Add(fExpScore = new BStringView("expscore",""), 1, 2)
		.End()
		.AddStrut(B_USE_BIG_SPACING)
		.AddGroup(B_HORIZONTAL)
			.AddGlue()
			.Add(reset)
			.Add(close)
		.End();

	begLabel->SetFont(be_bold_font);
	intLabel->SetFont(be_bold_font);
	expLabel->SetFont(be_bold_font);

	UpdateLabels();

	MakeCenteredOnShow(true);

	close->MakeDefault(true);
}


void
ScoreWindow::MessageReceived(BMessage *msg)
{
	if (msg->what == M_RESET_SCORES)
	{
		gBestTimes[DIFFICULTY_BEGINNER].Reset();
		gBestTimes[DIFFICULTY_INTERMEDIATE].Reset();
		gBestTimes[DIFFICULTY_EXPERT].Reset();
		UpdateLabels();
	}
	else
		BWindow::MessageReceived(msg);
}


void
ScoreWindow::UpdateLabels(void)
{
	fBegScore->SetText(_GetLabelForDifficulty(DIFFICULTY_BEGINNER).String());
	fIntScore->SetText(_GetLabelForDifficulty(DIFFICULTY_INTERMEDIATE).String());
	fExpScore->SetText(_GetLabelForDifficulty(DIFFICULTY_EXPERT).String());
}


BString
ScoreWindow::_GetLabelForDifficulty(uint32 difficulty) const
{
	BString scoreLabel;

	if (gBestTimes[difficulty].name == B_TRANSLATE("Anonymous") &&
		(int)gBestTimes[difficulty].time == 999){
		scoreLabel = B_UTF8_ELLIPSIS;
	} else {
		scoreLabel = B_TRANSLATE("%name%, %time% seconds ");
		scoreLabel.ReplaceAll("%name%", gBestTimes[difficulty].name);
		BString time;
		time.SetToFormat("%d", gBestTimes[difficulty].time);
		scoreLabel.ReplaceAll("%time%", time);
	}
	return scoreLabel;
}
