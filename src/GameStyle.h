#ifndef GAMESTYLE_H
#define GAMESTYLE_H


#include <Bitmap.h>
#include <Entry.h>
#include <ObjectList.h>
#include <String.h>


class StyleData {
public:
					StyleData();
					StyleData(const char* stylename, const char* stylepath, bool is_broken);

		BString 	name;
		BString 	path;
		bool		broken;
};


class GameStyle {
public:
					GameStyle(const char* path = NULL);
					~GameStyle();

	int32			CountStyles();
	const char*		StyleAt(const int32& index);
	bool			SetStyle(const char* name = NULL);
	const char*		StyleName();

	BRect			TileSize();
	BRect			TileRect(int x, int y);

	BBitmap* 		BaseSprite();
	BBitmap* 		BoxSprite();
	BBitmap* 		BoxDownSprite();
	BBitmap* 		MineSprite();
	BBitmap* 		NotMineSprite();
	BBitmap* 		QuestionSprite();
	BBitmap* 		FlagSprite();
	BBitmap* 		HitSprite();
	BBitmap* 		LEDSprite(uint8 value);
	BBitmap* 		NumberSprite(uint8 count);

	BBitmap* 		SmileyUp();
	BBitmap* 		SmileyDown();
	BBitmap* 		WinUp();
	BBitmap* 		WinDown();
	BBitmap* 		LoseUp();
	BBitmap* 		LoseDown();
	BBitmap* 		Worry();

	BBitmap**		LEDSprites();
	BBitmap**		NumberSprites();

private:
	void			MakeEmpty();
	void			ScanStyles();
	BBitmap*		GetStyleBitmap(entry_ref dir, const char* name);

	BBitmap*		fSmileyUp;
	BBitmap*		fSmileyDown;
	BBitmap*		fWinUp;
	BBitmap*		fWinDown;
	BBitmap*		fLoseUp;
	BBitmap*		fLoseDown;
	BBitmap*		fWorry;

	BBitmap*		fBoxSprite;
	BBitmap*		fBoxDownSprite;
	BBitmap*		fBaseSprite;
	BBitmap*		fHitSprite;
	BBitmap*		fMineSprite;
	BBitmap*		fNotMineSprite;
	BBitmap*		fFlagSprite;
	BBitmap*		fQuestionSprite;
	BBitmap*		fNumbers[8];
	BBitmap*		fLEDNumbers[10];

	BObjectList<StyleData>	fStyleList;

	BString			fStyleName;
};

extern GameStyle* gGameStyle;

#endif
