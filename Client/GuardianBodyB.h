#pragma once
#include "Obj.h"
class CLine;
class CGuardianBodyB :
	public CObj
{

private:
	enum BODY_B_STATE
	{
		IDLE,
		ATTACK_A,
		ATTACK_B,
		DEATH,
		STATE_END
	};

private:
	BODY_B_STATE m_eCurState;
	BODY_B_STATE m_eNextState;


private:
	list<CLine*> m_listLine;

private:
	float m_fLineTime;

private:
	D3DXVECTOR2 PrevPoint;
	D3DXVECTOR2 CurPoint;
public:
	void ChangeState();

public:
	void makeNewLine();

public:
	void Moving();
public:
	void Frame(void);

public:
	void AllMoveStop();
	void AllMoveActivate();

public:
	CGuardianBodyB();
	virtual ~CGuardianBodyB();

	// CObj을(를) 통해 상속됨
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;
};

