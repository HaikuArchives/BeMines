/*
 * Copyright 2007, DarkWyrm
 * Copyright 2013-2023, HaikuArchives Team
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		DarkWyrm (original author)
 *		JStressman
 */


#ifndef ESCAPE_CANCEL_FILTER_H
#define ESCAPE_CANCEL_FILTER_H

/*
	This filter is most often useful in dialog windows where you would
	like to allow the user to effectively hit the Cancel button just by
	hitting the Escape key. Pass one of these to BWindow::AddCommonFilter
	and that is all that is necessary.
*/

#include <Handler.h>
#include <MessageFilter.h>


class EscapeCancelFilter : public BMessageFilter {
public:
					EscapeCancelFilter()
						: BMessageFilter(B_PROGRAMMED_DELIVERY, B_ANY_SOURCE,B_KEY_DOWN)
					{}

					~EscapeCancelFilter()
					{}

	filter_result	Filter(BMessage* msg, BHandler** target)
	{
		int32 rawchar;
		int32 mod;
		msg->FindInt32("raw_char", &rawchar);
		msg->FindInt32("modifiers", &mod);

		if (rawchar == B_ESCAPE && (mod & (B_SHIFT_KEY | B_COMMAND_KEY
				| B_OPTION_KEY | B_CONTROL_KEY)) == 0) {
			BLooper* loop = (*target)->Looper();
			if (loop) {
				BMessenger msgr(loop);
				msgr.SendMessage(B_QUIT_REQUESTED);
				return B_SKIP_MESSAGE;
			}
		}
		return B_DISPATCH_MESSAGE;
	}
};


#endif
