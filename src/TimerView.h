#ifndef TIMERVIEW_H
#define TIMERVIEW_H


#include <View.h>
#include <Bitmap.h>
#include <MessageRunner.h>


typedef enum
{
	TIMER_STOP = 0,
	TIMER_START,
	TIMER_RESET
} TimerState;


class TimerView : public BView {
public:
					TimerView();
					~TimerView();

	void			MessageReceived(BMessage* msg);
	void			Draw(BRect update);

	void			SetState(TimerState state);
	TimerState		GetState();

	void			StyleChanged();
	uint16			GetTime();
	void			SetTime(uint16 time);

private:
	TimerState		fState;
	BMessageRunner*	fRunner;
	uint16			fTime;
	BBitmap**		fBitmaps;
};


#endif
