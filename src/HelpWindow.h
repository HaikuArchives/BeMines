/*
 * Copyright 2007, DarkWyrm
 * Copyright 2013-2023, HaikuArchives Team
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Humdinger
 */

#ifndef HELPWINDOW_H
#define HELPWINDOW_H


#include "DWindow.h"

#include <TextView.h>


class HelpWindow : public DWindow{ 
public:
			HelpWindow();
};


class HelpText : public BTextView {
public:
			HelpText(const char* name, const char* text);
};

#endif
