#include "Globals.h"

bool gCheatMode = false;
bool gPlaySounds = true;

BString gThemeName = "Default";
int32 gDifficulty = DIFFICULTY_BEGINNER;

uint16 gBeginnerBest = 999;
uint16 gIntermediateBest = 999;
uint16 gExpertBest = 999;

BString gBeginnerName = "Anonymous";
BString gIntermediateName = "Anonymous";
BString gExpertName = "Anonymous";

int8 gGameState = GAME_STOPPED;

uint16 gCustomWidth = 8;
uint16 gCustomHeight = 8;
uint16 gCustomMines = 10;

static HighScore sBestTimes[3];
HighScore *gBestTimes = &sBestTimes[0];
