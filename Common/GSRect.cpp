#include "GSRect.h"

GSRect::GSRect(void) : m_nX(0), m_nY(0), m_nWidth(-1), m_nHeight(-1)
{
}

GSRect::GSRect( Int32 nX, Int32 nY, Int32 nWidth, Int32 nHeight )
	: m_nX(nX), m_nY(nY), m_nWidth(nWidth), m_nHeight(nHeight)
{
}

GSRect::~GSRect(void)
{
}

Int32 GSRect::GetX() const
{
	return m_nX;
}

Int32 GSRect::GetY() const
{
	return m_nY;
}

Int32 GSRect::GetWidth() const
{
	return m_nWidth;
}

Int32 GSRect::GetHeight() const
{
	return m_nHeight;
}

void GSRect::SetX( Int32 nX )
{
	m_nX = nX;
}

void GSRect::SetY( Int32 nY )
{
	m_nY = nY;
}

void GSRect::SetWidth( Int32 nWidth )
{
	m_nWidth = nWidth;
}

void GSRect::SetHeight( Int32 nHeight )
{
	m_nHeight = nHeight;
}
