/*
 * Copyright 2007, DarkWyrm
 * Copyright 2013-2023, HaikuArchives Team
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		DarkWyrm (original author)
 *		Janus2
 *		JStressman
 */

#ifndef DWINDOW_H
#define DWINDOW_H


#include <Window.h>


class DWindow : public BWindow {
public:
						DWindow(BRect frame, const char* title,
							window_type type = B_TITLED_WINDOW,
							uint32 flags = 0,
							uint32 workspace = B_CURRENT_WORKSPACE);
						DWindow(BRect frame, const char* title,
							window_look look, window_feel feel,
							uint32 flags = 0,
							uint32 workspace = B_CURRENT_WORKSPACE);
	virtual				~DWindow();

	virtual	void		WorkspaceActivated(int32 index, bool active);
	virtual	void		ScreenChanged(BRect frame, color_space mode);
	virtual	void		Show();
			void		ConstrainToScreen();

	virtual	void		Zoom(BPoint origin, float width, float height);

			void		MakeCenteredOnShow(bool value);
			bool		IsCenteredOnShow() const;
			void		MoveToCenter();

			BView* 		GetBackgroundView();
			void		SetBackgroundColor(uint8 r, uint8 g, uint8 b);
			void		SetBackgroundColor(const rgb_color &color);
			rgb_color	GetBackgroundColor() const;


private:
			void		DWindowInit();

			BPoint		fOldLocation;
			BRect		fScreenFrame;
			bool		fCenterOnShow;
			BView*		fBackgroundView;
};

#endif
