#pragma once
#include "Obj.h"
class CRock :
	public CObj
{

private:
	enum STATE { FALLDOWN, IDLE, BROKEN, STATE_END };

private:
	STATE m_eCurState;
	STATE m_eNextState;


public:
	void ChangeState(void);

public:
	CRock();
	virtual ~CRock();

	// CObj을(를) 통해 상속됨
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

private:
	int m_iTypeOfRock;

public:
	void Render_Rect();
public:
	void SetOriginPosY();

public:
	void Break();

public:
	void Frame(void);

private:
	float m_fTime;

private:
	bool m_bStateChange;
	bool m_bShadow;
	bool m_bBroken;

private:
	float m_fOriginY;
	



};

