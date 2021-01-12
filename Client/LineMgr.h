#pragma once

#include "Include.h"

class CLine;
class CLineMgr
{
	DECLARE_SINGLETON(CLineMgr)

public:
	CLineMgr();
	~CLineMgr();

public:
	HRESULT		Initialize(void);
	int			Progress(void);
	void		Render(void);
	void		Release(void);

private:
	list<CLine*> m_listLine;



public:
	list<CLine*>* Get_ListLine() { return &m_listLine; }
};

