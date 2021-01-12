#include "stdafx.h"
#include "Line.h"

#include "GraphicDev.h"
#include "ScrollMgr.h"



CLine::CLine()
{
}


CLine::~CLine()
{
}

void CLine::Render(void)
{
	D3DXVECTOR2			vPoint[3] = 
	{
		D3DXVECTOR2(m_tLine.vPrevPoint.x + CScrollMgr::Get_ScrollX(), m_tLine.vPrevPoint.y + CScrollMgr::Get_ScrollY())
		, D3DXVECTOR2(m_tLine.vNextPoint.x + CScrollMgr::Get_ScrollX(), m_tLine.vNextPoint.y + CScrollMgr::Get_ScrollY())
		, D3DXVECTOR2(m_tLine.vPrevPoint.x + CScrollMgr::Get_ScrollX(), m_tLine.vPrevPoint.y + CScrollMgr::Get_ScrollY())
	};


	CGraphicDev::Get_Instance()->Render_End();
	CGraphicDev::Get_Instance()->Render_Begin();

	CGraphicDev::Get_Instance()->GetLine()->Draw(vPoint, 3, D3DCOLOR_ARGB(255, 255, 0, 255));

	CGraphicDev::Get_Instance()->Render_End();
	CGraphicDev::Get_Instance()->Render_Begin();
}
