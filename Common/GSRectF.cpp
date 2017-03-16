#include "GSRectF.h"

GSRectF::GSRectF(void) : m_rX(0.), m_rY(0.), m_rWidth(0.), m_rHeight(0.)
{
}

GSRectF::GSRectF( GSReal rX, GSReal rY, GSReal rWidth, GSReal rHeight )
	: m_rX(rX), m_rY(rY), m_rWidth(rWidth), m_rHeight(rHeight)
{
}

GSRectF::~GSRectF(void)
{
}

GSReal GSRectF::GetX() const
{
	return m_rX;
}

GSReal GSRectF::GetY() const
{
	return m_rY;
}

GSReal GSRectF::GetWidth() const
{
	return m_rWidth;
}

GSReal GSRectF::GetHeight() const
{
	return m_rHeight;
}

void GSRectF::SetX( GSReal rX )
{
	m_rX = rX;
}

void GSRectF::SetY( GSReal rY )
{
	m_rY = rY;
}

void GSRectF::SetWidth( GSReal rWidth )
{
	m_rWidth = rWidth;
}

void GSRectF::SetHeight( GSReal rHeight )
{
	m_rHeight = rHeight;
}
