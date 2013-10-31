#ifndef GAMESTYLE_H
#define GAMESTYLE_H

#include <Bitmap.h>
#include <Entry.h>
#include <String.h>

#include "ObjectList.h"

class StyleData
{
public:
	StyleData(void);
	StyleData(const char *stylename, const char *stylepath, bool is_broken);
	
	BString name;
	BString path;
	bool	broken;
};

class GameStyle
{
public:
				GameStyle(const char *path = NULL);
				~GameStyle(void);
	
	int32		CountStyles(void);
	const char *StyleAt(const int32 &index);
	bool		SetStyle(const char *name = NULL);
	const char *StyleName(void);
	
	BRect		TileSize(void);
	BRect		TileRect(int x, int y);
	
	BBitmap *	BaseSprite(void);
	BBitmap *	BoxSprite(void);
	BBitmap *	BoxDownSprite(void);
	BBitmap *	MineSprite(void);
	BBitmap *	NotMineSprite(void);
	BBitmap *	QuestionSprite(void);
	BBitmap *	FlagSprite(void);
	BBitmap *	HitSprite(void);
	BBitmap *	LEDSprite(uint8 value);
	BBitmap *	NumberSprite(uint8 count);
	
	BBitmap *	SmileyUp(void);
	BBitmap *	SmileyDown(void);
	BBitmap *	WinUp(void);
	BBitmap *	WinDown(void);
	BBitmap *	LoseUp(void);
	BBitmap *	LoseDown(void);
	BBitmap *	Worry(void);
	
	BBitmap **	LEDSprites(void);
	BBitmap **	NumberSprites(void);
	
private:
	void		MakeEmpty(void);
	void		ScanStyles(void);
	BBitmap	*	GetStyleBitmap(entry_ref dir, const char *name);
	
	BBitmap		*fSmileyUp,
				*fSmileyDown,
				*fWinUp,
				*fWinDown,
				*fLoseUp,
				*fLoseDown,
				*fWorry;
	
	BBitmap		*fBoxSprite,
				*fBoxDownSprite,
				*fBaseSprite,
				*fHitSprite,
				*fMineSprite,
				*fNotMineSprite,
				*fFlagSprite,
				*fQuestionSprite,
				*fNumbers[8],
				*fLEDNumbers[10];
	
	BObjectList<StyleData>	fStyleList;

	BString		fStyleName;
};

extern GameStyle *gGameStyle;

#endif
