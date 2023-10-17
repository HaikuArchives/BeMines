#ifndef COUNTERVIEW_H
#define COUNTERVIEW_H


#include <View.h>
#include <Bitmap.h>


class CounterView : public BView {
public:
				CounterView();

	void		Draw(BRect update);

	void		SetCount(uint16 count);
	uint16		GetCount();
	void		StyleChanged();

private:
	uint16		fCount;
	BBitmap**	fBitmaps;
};

#endif
