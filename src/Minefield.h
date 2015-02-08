#ifndef MINEFIELD_H
#define MINEFIELD_H

#include <SupportDefs.h>

typedef enum
{
	BOX_UNMARKED = 0,
	BOX_MARKED,
	BOX_QUESTION,
	BOX_REVEALED,
	BOX_HIT,
	BOX_MINE,
	BOX_NOTMINE,
	BOX_INVALID
} BoxState;


class IntPoint
{
public:
	IntPoint(uint16 px, uint16 py);
	IntPoint(void);
	IntPoint(const IntPoint &pt);
	IntPoint &operator=(const IntPoint &pt);

	uint16 x;
	uint16 y;

};

class Minefield;

typedef int32 (*TileFunction) (Minefield *f, IntPoint pt);

class Minefield
{
public:
			Minefield(uint16 width, uint16 height);
			~Minefield(void);

			// Resets the playing field
	void	Reset(const uint16 count);
	uint16	MineCount(void) const;
	uint16	Width(void) const;
	uint16	Height(void) const;
	uint8	TileDigit(const IntPoint &pt) const;
	uint8	TileDigit(const uint16 &x, const uint16 &y) const;

	bool	IsMine(const IntPoint &pt) const;
	bool	IsMine(const uint16 &x, const uint16 &y) const;

	uint16	TilesInState(const BoxState &state);

	void		SetState(const IntPoint &pt, const BoxState &s);
	BoxState	GetState(const IntPoint &pt);
	BoxState	GetState(const uint16 &x, const uint16 &y);

				// Takes a mine at the specified location and moves it to
				// another randomly-selected location in the grid
	void		TeleportMine(const IntPoint &pt);

				// Reveals all mines in a 1-tile radius of the clicked tile
	void		FireSonar(const IntPoint &pt);

private:
	uint8	*fData, *fDisplayData;
	uint16	fWidth,
			fHeight,
			fCount;
};

#endif
