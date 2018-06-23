#include "ScoreWindow.h"
#include <Button.h>

#include "EscapeCancelFilter.h"
#include "Globals.h"

#define M_RESET_SCORES 'rssc'

ScoreWindow::ScoreWindow(void)
	:	DWindow(BRect(100,500,100,400),"Best Times",B_TITLED_WINDOW,
				B_NOT_RESIZABLE | B_NOT_ZOOMABLE)
{
	AddCommonFilter(new EscapeCancelFilter());

	BView *top = GetBackgroundView();

	BStringView *begLabel = new BStringView(BRect(10,10,11,11),"beglabel",
											"Beginner:");
	begLabel->SetFont(be_bold_font);
	begLabel->ResizeToPreferred();
	top->AddChild(begLabel);

	BStringView *intLabel = new BStringView(BRect(10,10,11,11),"intlabel",
											"Intermediate:");
	intLabel->SetFont(be_bold_font);
	intLabel->ResizeToPreferred();
	intLabel->MoveBy(0,intLabel->Bounds().Height() + 10);
	top->AddChild(intLabel);

	BStringView *expLabel = new BStringView(BRect(10,10,11,11),"explabel",
											"Expert:");
	expLabel->SetFont(be_bold_font);
	expLabel->ResizeToPreferred();
	expLabel->MoveBy(0,(expLabel->Bounds().Height() + 10) * 2);
	top->AddChild(expLabel);

	fBegScore = new BStringView(BRect(10,10,11,11),"begscore","");
	fBegScore->MoveTo(intLabel->Frame().right + 50,begLabel->Frame().top);
	fBegScore->ResizeToPreferred();
	top->AddChild(fBegScore);

	fIntScore = new BStringView(BRect(10,10,11,11),"intscore","");
	fIntScore->MoveTo(intLabel->Frame().right + 50,intLabel->Frame().top);
	fIntScore->ResizeToPreferred();
	top->AddChild(fIntScore);

	fExpScore = new BStringView(BRect(10,10,11,11),"expscore","");
	fExpScore->MoveTo(intLabel->Frame().right + 50,expLabel->Frame().top);
	fExpScore->ResizeToPreferred();
	top->AddChild(fExpScore);

	BButton *reset = new BButton(BRect(0,0,1,1),"reset","Reset Times",
								new BMessage(M_RESET_SCORES),
								B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
	reset->ResizeToPreferred();
	top->AddChild(reset);

	BButton *close = new BButton(reset->Frame(),"close","Close",
								new BMessage(B_QUIT_REQUESTED),
								B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
	close->MoveTo(Bounds().right - close->Bounds().Width() - 10.0,
					Bounds().bottom - close->Bounds().Height() - 10.0);
	top->AddChild(close);

	reset->MoveTo(close->Frame().left - 10.0 - reset->Frame().Width(),
					close->Frame().top);

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
	BString scoreLabel;

	if (gBestTimes[DIFFICULTY_BEGINNER].name == "Anonymous" &&
		(int)gBestTimes[DIFFICULTY_BEGINNER].time == 999){
		scoreLabel = B_UTF8_ELLIPSIS;
	} else {
		scoreLabel << gBestTimes[DIFFICULTY_BEGINNER].name
					<< ", " << (int)gBestTimes[DIFFICULTY_BEGINNER].time
					<< " seconds";
	}
	fBegScore->SetText(scoreLabel.String());
	fBegScore->ResizeToPreferred();
	scoreLabel = "";

	if (gBestTimes[DIFFICULTY_INTERMEDIATE].name == "Anonymous" &&
		(int)gBestTimes[DIFFICULTY_INTERMEDIATE].time == 999){
		scoreLabel = B_UTF8_ELLIPSIS;
	} else {
		scoreLabel << gBestTimes[DIFFICULTY_INTERMEDIATE].name
					<< ", " << (int)gBestTimes[DIFFICULTY_INTERMEDIATE].time
					<< " seconds";
	}
	fIntScore->SetText(scoreLabel.String());
	fIntScore->ResizeToPreferred();
	scoreLabel = "";

	if (gBestTimes[DIFFICULTY_INTERMEDIATE].name == "Anonymous" &&
		(int)gBestTimes[DIFFICULTY_INTERMEDIATE].time == 999){
		scoreLabel = B_UTF8_ELLIPSIS;
	} else {
		scoreLabel << gBestTimes[DIFFICULTY_EXPERT].name
					<< ", " << (int)gBestTimes[DIFFICULTY_EXPERT].time
					<< " seconds";
	}
	fExpScore->SetText(scoreLabel.String());
	fExpScore->ResizeToPreferred();

	BView *close = FindView("close");

	float right = MAX(fBegScore->Frame().right, fIntScore->Frame().right);
	right = MAX(right, fExpScore->Frame().right);
	right += 20;

	if (right < 200)
		right = 200;

	float bottom = fExpScore->Frame().bottom + 20.0;
	bottom += (close) ? close->Bounds().Height() : 0;
	ResizeTo(right, bottom);
}
