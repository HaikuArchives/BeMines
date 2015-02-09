#ifndef GLOBALS_H
#define GLOBALS_H

#include <String.h>

enum
{
	GAME_OVER = -1,
	GAME_STOPPED = 0,
	GAME_STARTED = 1,
	GAME_PAUSED = 2
};

enum
{
	DIFFICULTY_BEGINNER = 0,
	DIFFICULTY_INTERMEDIATE,
	DIFFICULTY_EXPERT,
	DIFFICULTY_CUSTOM,
	DIFFICULTY_END
};

class HighScore
{
public:
	HighScore(void)
	{
		Reset();
	}
	HighScore(uint16 scoretime, const char *scorename)
	{
		time = scoretime;
		name = scorename;
	}
	void Reset(void)
	{
		time = 999;
		name = "Anonymous";
	}

	uint16 time;
	BString name;
};

extern bool gCheatMode;
extern bool gPlaySounds;
extern BString gThemeName;
extern int32 gDifficulty;

extern uint16 gCustomWidth;
extern uint16 gCustomHeight;
extern uint16 gCustomMines;

extern HighScore *gBestTimes;

extern int8	gGameState;

#endif
