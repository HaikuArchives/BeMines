/*
 * Copyright 2007, DarkWyrm
 * Copyright 2013-2023, HaikuArchives Team
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		DarkWyrm (original author)
 *		Johan Wagenheim
 *		JStressman
 */


#include "GameStyle.h"
#include "Globals.h"

#include <Application.h>
#include <Catalog.h>
#include <Directory.h>
#include <Path.h>
#include <Roster.h>
#include <String.h>
#include <TranslationUtils.h>
#include <TranslatorFormats.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "GameStyle"


GameStyle::GameStyle(const char* path)
	:
	fSmileyUp(NULL),
	fSmileyDown(NULL),
	fWinUp(NULL),
	fWinDown(NULL),
	fLoseUp(NULL),
	fLoseDown(NULL),
	fWorry(NULL),
	fBoxSprite(NULL),
	fBoxDownSprite(NULL),
	fBaseSprite(NULL),
	fHitSprite(NULL),
	fMineSprite(NULL),
	fNotMineSprite(NULL),
	fFlagSprite(NULL),
	fQuestionSprite(NULL),
	fStyleList(20, true),
	fStyleName("Default")
{
	int32 i;
	for (i = 0; i < 8; i++)
		fNumbers[i] = NULL;

	for (i = 0; i < 10; i++)
		fLEDNumbers[i] = NULL;

	ScanStyles();
	if (!path || !SetStyle(path))
		SetStyle(NULL);
}


GameStyle::~GameStyle()
{
	MakeEmpty();
}


int32
GameStyle::CountStyles()
{
	return fStyleList.CountItems();
}


const char*
GameStyle::StyleAt(const int32& index)
{
	StyleData* data = fStyleList.ItemAt(index);
	return data ? data->name.String() : NULL;
}


bool
GameStyle::SetStyle(const char* stylename)
{
	StyleData* data = NULL;
	entry_ref ref;

	BString name;
	if (stylename) {
		for (int32 i = 0; i < fStyleList.CountItems(); i++) {
			StyleData* item = fStyleList.ItemAt(i);
			if (item->name.Compare(stylename) == 0) {
				data = item;
				break;
			}
		}

		if (data) {
			BEntry entry(data->path.String());
			entry.GetRef(&ref);
			name = ref.name;
		} else {
			fStyleName = "Default";
			return false;
		}
	}

	MakeEmpty();
	fBoxSprite = GetStyleBitmap(ref, "unmarked.png");
	fBoxDownSprite = GetStyleBitmap(ref, "down.png");
	fBaseSprite = GetStyleBitmap(ref, "tilebase.png");
	fHitSprite = GetStyleBitmap(ref, "hit.png");
	fMineSprite = GetStyleBitmap(ref, "mine.png");
	fNotMineSprite = GetStyleBitmap(ref, "notmine.png");
	fFlagSprite = GetStyleBitmap(ref, "flag.png");
	fQuestionSprite = GetStyleBitmap(ref, "question.png");

	fSmileyUp = GetStyleBitmap(ref, "smileybase.png");
	fSmileyDown = GetStyleBitmap(ref, "smileybasedown.png");
	fWinUp = GetStyleBitmap(ref, "smiley-win.png");
	fWinDown = GetStyleBitmap(ref, "smiley-windown.png");
	fLoseUp = GetStyleBitmap(ref, "smiley-lose.png");
	fLoseDown = GetStyleBitmap(ref, "smiley-losedown.png");
	fWorry = GetStyleBitmap(ref, "smiley-worry.png");

	int32 i;
	for (i = 1; i < 9; i++) {
		BString bmpname = "mine";
		bmpname << i << ".png";
		fNumbers[i - 1] = GetStyleBitmap(ref, bmpname.String());
	}

	for (i = 0; i < 10; i++) {
		BString ledname = "led";
		ledname << i << ".png";
		fLEDNumbers[i] = GetStyleBitmap(ref, ledname.String());
	}

	fStyleName = data ? data->name : BString("Default");
	return true;
}


const char*
GameStyle::StyleName()
{
	return fStyleName.String();
}


BRect
GameStyle::TileSize()
{
	BRect r = fBoxSprite->Bounds();
	r.Set(0, 0, r.Width() * gScale, r.Height() * gScale);
	return r;
}


BRect
GameStyle::TileRect(int x, int y)
{
	BRect r = TileSize();
	r.OffsetBy(((r.Width() + 1.0) * x), ((r.Height() + 1.0) * y));
	return r;
}


BBitmap*
GameStyle::BaseSprite()
{
	return fBaseSprite;
}


BBitmap*
GameStyle::BoxSprite()
{
	return fBoxSprite;
}


BBitmap*
GameStyle::BoxDownSprite()
{
	return fBoxDownSprite;
}


BBitmap*
GameStyle::MineSprite()
{
	return fMineSprite;
}


BBitmap*
GameStyle::NotMineSprite()
{
	return fNotMineSprite;
}


BBitmap*
GameStyle::QuestionSprite()
{
	return fQuestionSprite;
}


BBitmap*
GameStyle::FlagSprite()
{
	return fFlagSprite;
}


BBitmap*
GameStyle::HitSprite()
{
	return fHitSprite;
}


BBitmap*
GameStyle::LEDSprite(uint8 value)
{
	return fLEDNumbers[value];
}


BBitmap*
GameStyle::NumberSprite(uint8 count)
{
	if (count < 1 || count > 8)
		debugger("Bad number sprite value");

	return fNumbers[count - 1];
}


BBitmap*
GameStyle::SmileyUp()
{
	return fSmileyUp;
}


BBitmap*
GameStyle::SmileyDown()
{
	return fSmileyDown;
}


BBitmap*
GameStyle::WinUp()
{
	return fWinUp;
}


BBitmap*
GameStyle::WinDown()
{
	return fWinDown;
}


BBitmap*
GameStyle::LoseUp()
{
	return fLoseUp;
}


BBitmap*
GameStyle::LoseDown()
{
	return fLoseDown;
}


BBitmap*
GameStyle::Worry()
{
	return fWorry;
}


BBitmap**
GameStyle::LEDSprites()
{
	return fLEDNumbers;
}


BBitmap**
GameStyle::NumberSprites()
{
	return fNumbers;
}


void
GameStyle::MakeEmpty()
{
	delete fBoxSprite;
	delete fBoxDownSprite;
	delete fBaseSprite;
	delete fHitSprite;
	delete fMineSprite;
	delete fNotMineSprite;
	delete fFlagSprite;
	delete fQuestionSprite;

	int32 i;
	for (i = 0; i < 8; i++)
		delete fNumbers[i];

	for (i = 0; i < 10; i++)
		delete fLEDNumbers[i];
}


void
GameStyle::ScanStyles()
{
	static const char* bitmapNames[] =
	{
		"tilebase.png",
		"unmarked.png",
		"down.png",
		"mine.png",
		"notmine.png",
		"question.png",
		"flag.png",
		"hit.png",

		"smileybase.png",
		"smileybasedown.png",
		"smiley-worry.png",
		"smiley-win.png",
		"smiley-windown.png",
		"smiley-lose.png",
		"smiley-losedown.png",
		NULL
	};

	app_info ai;
	be_app->GetAppInfo(&ai);
	BPath path(&ai.ref);
	path.GetParent(&path);
	path.Append("themes");

	BDirectory dir(path.Path());
	dir.Rewind();
	BEntry entry;
	while (dir.GetNextEntry(&entry) == B_OK) {
		if (!entry.IsDirectory())
			continue;

		BString styleName, stylePath;
		bool is_broken = false;

		entry_ref ref;
		entry.GetRef(&ref);

		styleName = ref.name;
		path.SetTo(&ref);
		stylePath = path.Path();

		// Scan for the existence of all the necessary files. We can function without
		// a style being complete, but the user should be notified about the problems
		BPath temppath;
		for (int8 i = 0; bitmapNames[i] != NULL; i++) {
			temppath.SetTo(&ref);
			temppath.Append(bitmapNames[i]);
			if (!BEntry(temppath.Path()).Exists()) {
				is_broken = true;
				break;
			}
		}

		int32 i;
		if (!is_broken) {
			for (i = 1; i < 9; i++) {
				BString bmpname = "mine";
				bmpname << i << ".png";
				temppath.SetTo(&ref);
				bmpname.Prepend("/");
				bmpname.Prepend(temppath.Path());
				if (!BEntry(bmpname.String()).Exists()) {
					is_broken = true;
					break;
				}
			}
		}

		if (!is_broken) {
			for (i = 0; i < 10; i++) {
				BString ledname = "led";
				ledname << i << ".png";
				temppath.SetTo(&ref);
				ledname.Prepend("/");
				ledname.Prepend(temppath.Path());
				if (!BEntry(ledname.String()).Exists()) {
					is_broken = true;
					break;
				}
			}
		}

		fStyleList.AddItem(new StyleData(styleName.String(), stylePath.String(), is_broken));

	} // end style directory scanning loop
}


BBitmap*
GameStyle::GetStyleBitmap(entry_ref dir, const char* name)
{
	BPath path(&dir);
	path.Append(name);

	BBitmap* bmp = NULL;

	bmp = BTranslationUtils::GetBitmap(path.Path());
	if (!bmp)
		bmp = BTranslationUtils::GetBitmap(B_PNG_FORMAT, name);

	return bmp;
}


StyleData::StyleData()
	:
	broken(false)
{
}


StyleData::StyleData(const char* stylename, const char* stylepath, bool is_broken)
	:
	name(stylename),
	path(stylepath),
	broken(is_broken)
{
}
