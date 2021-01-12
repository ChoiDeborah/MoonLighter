#include "stdafx.h"
#include "LineMgr.h"
#include "Line.h"

IMPLEMENT_SINGLETON(CLineMgr)

CLineMgr::CLineMgr()
{
}


CLineMgr::~CLineMgr()
{
	Release();
}

HRESULT CLineMgr::Initialize(void)
{
	return E_NOTIMPL;
}

int CLineMgr::Progress(void)
{
	return 0;
}

void CLineMgr::Render(void)
{
	for (auto & pLine : m_listLine)
	{
		pLine->Render();
	}
}

void CLineMgr::Release(void)
{
	for_each(m_listLine.begin(), m_listLine.end(), DeleteObj());
	//for (auto& pLine : m_listLine)
	//{
	//	if (pLine)
	//	{
	//		delete pLine;
	//		pLine = nullptr;
	//	}
	//}
	m_listLine.clear();
}
