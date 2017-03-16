#ifndef GSRECTF_H
#define GSRECTF_H

#include "CommonDef.h"

class GSRectF
{
public:
	GSRectF(void);
	GSRectF(GSReal rX, GSReal rY, GSReal rWidth, GSReal rHeight);
	~GSRectF(void);

	GSReal GetX() const;
	GSReal GetY() const;
	GSReal GetWidth() const;
	GSReal GetHeight() const;

	void SetX(GSReal rX);
	void SetY(GSReal rY);
	void SetWidth(GSReal rWidth);
	void SetHeight(GSReal rHeight);

private:
	GSReal m_rX;
	GSReal m_rY;
	GSReal m_rWidth;
	GSReal m_rHeight;
};

#endif // GSRECTF_H