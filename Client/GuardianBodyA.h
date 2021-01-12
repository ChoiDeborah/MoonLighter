#pragma once
#include "Obj.h"
class CGuardianBodyA :
	public CObj
{
private:
	enum BODY_A_STATE 
	{
		IDLE,
		ATTACK_A,
		ATTACK_B,
		DEATH,
		STATE_END
	};

private:
	BODY_A_STATE m_eCurState;
	BODY_A_STATE m_eNextState;

private:
	LINEINFO testLine;

public:
	void ChangeState();
	void Frame();

public:
	CGuardianBodyA();
	virtual ~CGuardianBodyA();

	// CObj을(를) 통해 상속됨
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

public:
	void Moving();

public:
	void AllMoveStop();
	void AllMoveActivate();

public:
	void Hit(bool bIsY);

public:
	void Render_Line();

	void CollisionLock();

};

