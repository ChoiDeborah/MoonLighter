#pragma once
#include "Obj.h"
class CLine;
class CGuardianBodyC :
	public CObj
{
private:
	enum BODY_C_STATE
	{
		IDLE,
		ATTACK_A,
		ATTACK_B,
		DEATH,
		STATE_END
	};

private:
	BODY_C_STATE m_eCurState;
	BODY_C_STATE m_eNextState;

public:
	void ChangeState();

public:
	CGuardianBodyC();
	virtual ~CGuardianBodyC();

	// CObj을(를) 통해 상속됨
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

private:
	list<CLine*> m_listLine;

private:
	float m_fLineTime;

private:
	D3DXVECTOR2 PrevPoint;
	D3DXVECTOR2 CurPoint;

public:
	void makeNewLine();

public:
	void Moving();

public:
	void AllMoveStop();
	void AllMoveActivate();

public:
	void Frame(void);

};

