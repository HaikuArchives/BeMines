#include "AchievementWindow.h"
#include <Button.h>
#include <Catalog.h>
#include "Globals.h"
#include <LayoutBuilder.h>

#define M_RESET_ACHIEVEMENTS 'rsac'

//#define B_TRANSLATION_CONTEXT "AchivementWindow"

//TODO support translations

AchievementWindow::AchievementWindow(void)
	:	DWindow(BRect(100,500,100,400),"Achivements",
		B_TITLED_WINDOW, B_NOT_RESIZABLE |B_NOT_ZOOMABLE |
		B_AUTO_UPDATE_SIZE_LIMITS)

{
	BView *top = GetBackgroundView();

	BStringView *begTitle, *intTitle, *expTitle, *genTitle;
	BButton *close = new BButton("ok", "OK", new BMessage(B_QUIT_REQUESTED));
	BButton *reset = new BButton("reset", "Reset", new BMessage(M_RESET_ACHIEVEMENTS));

	BLayoutBuilder::Group<>(top, B_VERTICAL, 0)
		.SetInsets(B_USE_WINDOW_INSETS)
		.AddGroup(B_VERTICAL, 0)
			//General achievements
			.Add(genTitle = new BStringView("genTitle", "General"))
			.Add(labelList[3][0] = new BStringView("gen1Label", "Basic Training - Win a game"))
			.Add(labelList[3][1] = new BStringView("gen2Label", "Active Duty - Win 5 games"))
			.AddStrut(B_USE_DEFAULT_SPACING)
			//Beginner achivements
			.Add(begTitle = new BStringView("begTitle", "Difficulty: Beginner"))
			.Add(labelList[0][0] = new BStringView("beg1Label", "Recruit - Win within 100 seconds"))
			.Add(labelList[0][1] = new BStringView("beg2Label",  "Bomb Buster - Win within 50 seconds"))
			.AddStrut(B_USE_DEFAULT_SPACING)
			//Intermediate achievements
			.Add(intTitle = new BStringView("intTitle", "Difficulty: Intermediate"))
			.Add(labelList[1][0] = new BStringView("int1Label", "Officer - Win within 100 seconds"))
			.Add(labelList[1][1] = new BStringView("int2Label", "Master Defuser - Win within 50 seconds"))
			.AddStrut(B_USE_DEFAULT_SPACING)
			//Expert achievements
			.Add(expTitle = new BStringView("expTitle", "Difficulty: Expert"))
			.Add(labelList[2][0] = new BStringView("exp1Label", "Commando - Win within 100 seconds"))
			.Add(labelList[2][1] = new BStringView("exp1Label", "Mine Zone Veteran - Win within 50 seconds"))
		.End()
		.AddStrut(B_USE_BIG_SPACING)
		.AddGroup(B_HORIZONTAL)
			.AddGlue()
			.Add(close)
			.Add(reset)
		.End();

	genTitle->SetFont(be_bold_font);
	begTitle->SetFont(be_bold_font);
	intTitle->SetFont(be_bold_font);
	expTitle->SetFont(be_bold_font);

	UpdateLabels();

}

void
AchievementWindow::UpdateLabels(void)
{
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 2; y++) {
			if (gAchievements[x][y]) {
				labelList[x][y]->SetHighColor(1,50,32);
				labelList[x][y]->SetToolTip("Unlocked!");
			} else {
				labelList[x][y]->SetHighColor(0,0,0);
				labelList[x][y]->SetToolTip("Locked!");
			}
		}
	}
}

void
AchievementWindow::MessageReceived(BMessage *msg)
{
	if (msg->what == M_RESET_ACHIEVEMENTS)
	{
		for (int x = 0; x < 4; x++){
			for (int y = 0; y < 2; y++){
				gAchievements[x][y] = false;
			}
		}
		gGamesWon = 0;
		UpdateLabels();
	}
	else
		BWindow::MessageReceived(msg);
}
