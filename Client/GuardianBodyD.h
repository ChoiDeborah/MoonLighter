#pragma once
#include "Obj.h"

class CLine;
class CGuardianBodyD :
	public CObj
{
private:
	enum BODY_D_STATE
	{
		IDLE,
		// ź��
		ATTACK_A,
		// ƨ��°�
		ATTACK_B,
		DEATH,
		STATE_END
	};

private:
	BODY_D_STATE m_eCurState;
	BODY_D_STATE m_eNextState;

public:
	void ChangeState();
public:
	CGuardianBodyD();
	virtual ~CGuardianBodyD();

	// CObj��(��) ���� ��ӵ�
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
	void AllMoveStop();
	void AllMoveActivate();

public:
	void makeNewLine();
	void Moving();

public:
	void Frame(void);
};

