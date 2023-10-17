#include "Globals.h"

#include <Catalog.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Globals"

bool gCheatMode = false;
bool gPlaySounds = true;
uint16 gScale = 1;

BString gThemeName = "Default";
int32 gDifficulty = DIFFICULTY_BEGINNER;

uint16 gBeginnerBest = 999;
uint16 gIntermediateBest = 999;
uint16 gExpertBest = 999;

BString gBeginnerName = B_TRANSLATE("Anonymous");
BString gIntermediateName = B_TRANSLATE("Anonymous");
BString gExpertName = B_TRANSLATE("Anonymous");

int8 gGameState = GAME_STOPPED;

uint16 gCustomWidth = 8;
uint16 gCustomHeight = 8;
uint16 gCustomMines = 10;

static HighScore sBestTimes[3];
HighScore* gBestTimes = &sBestTimes[0];

bool gAchievements[4][2] = {{false, false}, {false, false}, {false, false}, {false, false}};
int32 gGamesWon = 0;
