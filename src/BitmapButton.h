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

#ifndef BITMAP_BUTTON_H
#define BITMAP_BUTTON_H


#include <Looper.h>
#include <Application.h>
#include <Window.h>
#include <Button.h>
#include <Bitmap.h>
#include <Rect.h>


class BitmapButton : public BButton {
public:
						BitmapButton(const BRect &frame, const char* name, BBitmap* up,
									BBitmap* down, BMessage* msg, bool own = true,
									const int32& resize = B_FOLLOW_LEFT | B_FOLLOW_TOP,
									const int32& flags = B_WILL_DRAW | B_NAVIGABLE);
	virtual				~BitmapButton();

	virtual	void		Draw(BRect update);
			void 		ResizeToPreferred();

			void 		SetBitmaps(BBitmap* up, BBitmap *down);
			BBitmap* 	UpBitmap();
			BBitmap* 	DownBitmap();

			void 		SetDisabledBitmap(BBitmap* disabled);
			BBitmap* 	DisabledBitmap() const;

			void 		SetFocusBitmaps(BBitmap* up, BBitmap* down);
			BBitmap* 	UpFocusBitmap();
			BBitmap* 	DownFocusBitmap();


private:
			bool		fOwnBitmaps;

			BBitmap* 	fUp;
			BBitmap* 	fDown;
			BBitmap* 	fDisabled;
			BBitmap* 	fFocusUp;
			BBitmap* 	fFocusDown;
};

#endif
