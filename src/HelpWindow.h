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
