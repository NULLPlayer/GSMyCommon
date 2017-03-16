#ifndef GSRECT_H
#define GSRECT_H

#include "CommonDef.h"

class GSRect
{
public:
	GSRect(void);
	GSRect(Int32 nX, Int32 nY, Int32 nWidth, Int32 nHeight);
	~GSRect(void);

public:
	Int32 GetX() const;
	Int32 GetY() const;
	Int32 GetWidth() const;
	Int32 GetHeight() const;

	void SetX(Int32 nX);
	void SetY(Int32 nY);
	void SetWidth(Int32 nWidth);
	void SetHeight(Int32 nHeight);

private:
	Int32 m_nX;
	Int32 m_nY;
	Int32 m_nWidth;
	Int32 m_nHeight;
};

#endif // GSRECT_H