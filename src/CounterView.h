#ifndef COUNTERVIEW_H
#define COUNTERVIEW_H

#include <View.h>
#include <Bitmap.h>

class CounterView : public BView
{
public:
				CounterView(void);
	void		SetCount(uint16 count);
	uint16		GetCount(void);
	
	void		Draw(BRect update);
	void		StyleChanged(void);
	
private:
	uint16			fCount;
	BBitmap			**fBitmaps;
};


#endif
