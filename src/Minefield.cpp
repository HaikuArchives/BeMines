/*
 * Copyright 2007, DarkWyrm
 * Copyright 2013-2023, HaikuArchives Team
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		DarkWyrm (original author)
 *		JStressman
 *		KevinAdams05
 */


#include "Minefield.h"

#include <OS.h>

#include <stdlib.h>
#include <string.h>


Minefield::Minefield(uint16 width, uint16 height)
	:
	fWidth(width),
	fHeight(height)
{
	fData = new uint8[width * height];
	fDisplayData = new uint8[width * height];
}


Minefield::~Minefield()
{
	delete[] fData;
	delete[] fDisplayData;
}


void
Minefield::Reset(const uint16 count)
{
	srand(system_time());
	uint32 bufferSize = fHeight * fWidth;
	memset(fData, 0, bufferSize);
	memset(fDisplayData, 0, bufferSize);

	fCount = count;

	uint16 left = count;
	while (left) {
		uint16 index = rand() % bufferSize;
		if (fData[index] == 0) {
			fData[index] = 1;
			left--;
		}
	}
}


uint16
Minefield::MineCount() const
{
	return fCount;
}


uint16
Minefield::Width() const
{
	return fWidth;
}


uint16
Minefield::Height() const
{
	return fHeight;
}


uint8
Minefield::TileDigit(const IntPoint& pt) const
{
	uint8 count = TileDigit(pt.x, pt.y);
	return count;
}


uint8
Minefield::TileDigit(const uint16& x, const uint16& y) const
{
	if (x > fWidth || y > fHeight)
		debugger("Invalid (x,y)");

	uint8 mineCount = 0;

	if (y > 0) {
		if (x > 0 && IsMine(x - 1, y - 1))
			mineCount++;

		if (IsMine(x, y - 1))
			mineCount++;

		if (x < (Width() - 1) && IsMine(x + 1, y - 1))
			mineCount++;
	}

	if (x > 0 && IsMine(x - 1, y))
		mineCount++;

	if (x < (Width() - 1) && IsMine(x + 1, y))
		mineCount++;

	if (y < (Height() - 1)) {
		if (x > 0 && IsMine(x - 1, y + 1))
			mineCount++;

		if (IsMine(x, y + 1))
			mineCount++;

		if (x < (Width() - 1) && IsMine(x + 1, y + 1))
			mineCount++;
	}

	return mineCount;
}


bool
Minefield::IsMine(const IntPoint& pt) const
{
	bool value = IsMine(pt.x, pt.y);
	return value;
}


bool
Minefield::IsMine(const uint16& x, const uint16& y) const
{
	if (x > fWidth || y > fHeight)
		debugger("Invalid (x,y)");

	return fData[(fWidth * y) + x] != 0;
}


uint16
Minefield::TilesInState(const BoxState& state)
{
	uint32 bufferSize = fHeight * fWidth;
	uint16 count = 0;
	for (uint32 i = 0; i < bufferSize; i++) {
		if (fDisplayData[i] == state)
			count++;
	}

	return count;
}


void
Minefield::SetState(const IntPoint& pt, const BoxState& s)
{
	if (pt.x > fWidth || pt.y > fHeight)
		debugger("Invalid (x,y)");

	fDisplayData[(fWidth * pt.y) + pt.x] = s;
}


BoxState
Minefield::GetState(const IntPoint& pt)
{
	if (pt.x > fWidth || pt.y > fHeight)
		debugger("Invalid (x,y)");

	return (BoxState) fDisplayData[(fWidth * pt.y) + pt.x];
}


BoxState
Minefield::GetState(const uint16& x, const uint16& y)
{
	return GetState(IntPoint(x, y));
}


void
Minefield::TeleportMine(const IntPoint& pt)
{
	if (pt.x > fWidth || pt.y > fHeight || !IsMine(pt))
		return;

	uint32 bufferSize = fHeight * fWidth;
	fData[(fWidth * pt.y) + pt.x] = 0;

	bool done = false;
	while (!done) {
		uint16 index = rand() % bufferSize;
		if (fData[index] == 0) {
			fData[index] = 1;
			done = true;
		}
	}
}


bool
Minefield::FireSonar(const IntPoint& pt)
{
	bool revealedNewBoxes = false;
	uint16 x = pt.x;
	uint16 y = pt.y;

	if (x > fWidth || y > fHeight)
		return revealedNewBoxes;

	IntPoint temp;
	if (y > 0) {
		if (x > 0) {
			temp.x = x - 1;
			temp.y = y - 1;

			if (IsBoxBeingRevealed(temp))
				revealedNewBoxes = true;

			if (IsMine(temp))
				SetState(temp, BOX_MARKED);
			else
				SetState(temp, BOX_REVEALED);
		}

		temp.x = x;
		temp.y = y - 1;

		if (IsBoxBeingRevealed(temp))
			revealedNewBoxes = true;

		if (IsMine(temp))
			SetState(temp, BOX_MARKED);
		else
			SetState(temp, BOX_REVEALED);

		if (x < (Width() - 1)) {
			temp.x = x + 1;
			temp.y = y - 1;

			if (IsBoxBeingRevealed(temp))
				revealedNewBoxes = true;

			if (IsMine(temp))
				SetState(temp, BOX_MARKED);
			else
				SetState(temp, BOX_REVEALED);
		}
	}

	if (x > 0) {
		temp.x = x - 1;
		temp.y = y;

		if (IsBoxBeingRevealed(temp))
			revealedNewBoxes = true;

		if (IsMine(temp))
			SetState(temp, BOX_MARKED);
		else
			SetState(temp, BOX_REVEALED);
	}

	if (IsBoxBeingRevealed(pt))
		revealedNewBoxes = true;

	if (IsMine(pt))
		SetState(pt, BOX_MARKED);
	else
		SetState(pt, BOX_REVEALED);

	if (x < (Width() - 1)) {
		temp.x = x + 1;
		temp.y = y;

		if (IsBoxBeingRevealed(temp))
			revealedNewBoxes = true;

		if (IsMine(temp))
			SetState(temp, BOX_MARKED);
		else
			SetState(temp, BOX_REVEALED);
	}

	if (y > 0) {
		if (x > 0) {
			temp.x = x - 1;
			temp.y = y + 1;

			if (IsBoxBeingRevealed(temp))
				revealedNewBoxes = true;

			if (IsMine(temp))
				SetState(temp, BOX_MARKED);
			else
				SetState(temp, BOX_REVEALED);
		}

		temp.x = x;
		temp.y = y + 1;

		if (IsBoxBeingRevealed(temp))
			revealedNewBoxes = true;

		if (IsMine(temp))
			SetState(temp, BOX_MARKED);
		else
			SetState(temp, BOX_REVEALED);

		if (x < (Width() - 1)) {
			temp.x = x + 1;
			temp.y = y + 1;

			if (IsBoxBeingRevealed(temp))
				revealedNewBoxes = true;

			if (IsMine(temp))
				SetState(temp, BOX_MARKED);
			else
				SetState(temp, BOX_REVEALED);
		}
	}

	return revealedNewBoxes;
}


bool
Minefield::IsBoxBeingRevealed(const IntPoint& pt)
{
	if (GetState(pt) == BOX_MARKED || GetState(pt) == BOX_QUESTION || GetState(pt) == BOX_REVEALED
		|| GetState(pt) == BOX_HIT || GetState(pt) == BOX_MINE || GetState(pt) == BOX_NOTMINE
		|| GetState(pt) == BOX_INVALID) {
		return false;
	}

	return true;
}


IntPoint::IntPoint(uint16 px, uint16 py)
{
	x = px;
	y = py;
}


IntPoint::IntPoint()
{
	x = 0;
	y = 0;
}


IntPoint::IntPoint(const IntPoint& pt)
{
	x = pt.x;
	y = pt.y;
}


IntPoint&
IntPoint::operator=(const IntPoint& pt)
{
	x = pt.x;
	y = pt.y;
	return *this;
}
