#include "CounterView.h"
#include "GameStyle.h"
#include <stdio.h>

CounterView::CounterView(void)
	:	BView(BRect(0,0,1,1),"counterview",B_FOLLOW_NONE, B_WILL_DRAW),
		fCount(0),
		fBitmaps(gGameStyle->LEDSprites())
{
	if (!fBitmaps)
		debugger("BUG: empty counter theme");
	
	BBitmap *zero = fBitmaps[0];
	ResizeTo(zero->Bounds().Width() * 3,zero->Bounds().Height());
}


void
CounterView::SetCount(uint16 count)
{
	if (count > 999)
		count = 999;
	
	fCount = count;
	Draw(Bounds());
}


uint16
CounterView::GetCount(void)
{
	return fCount;
}


void
CounterView::Draw(BRect update)
{
	char countstr[5];
	sprintf(countstr,"%.3d",fCount);
	
	BPoint pt(0,0);
	DrawBitmap(fBitmaps[countstr[0] - 48],pt);
	pt.x += fBitmaps[0]->Bounds().Width();
	DrawBitmap(fBitmaps[countstr[1] - 48],pt);
	pt.x += pt.x;
	DrawBitmap(fBitmaps[countstr[2] - 48],pt);
}


void
CounterView::StyleChanged(void)
{
	fBitmaps = gGameStyle->LEDSprites();
	BBitmap *zero = fBitmaps[0];
	ResizeTo(zero->Bounds().Width() * 3,zero->Bounds().Height());
	Invalidate();
}

