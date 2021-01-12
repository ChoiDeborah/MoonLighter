#pragma once
#include "Obj.h"

class CPlayer;

class CPunch :
	public CObj
{
private:
	enum PUNCH_STATE { PUNCH_CRACK, PUNCH_DOWN, PUNCH_SHADOW, PUNCH_UP, PUNCH_END };
public:
	CPunch();
	virtual ~CPunch();

	// CObj을(를) 통해 상속됨
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;


public:
	void Frame(void);
	void StateChange(void);

private:
	PUNCH_STATE m_eCurState;
	PUNCH_STATE m_eNextState;

private:
	CPlayer* m_pPlayer;

private:
	D3DXVECTOR3 m_vPlayerPos;

private:
	float m_fOriginX;
	float m_fOriginY;

public:
	void Set_OriginPosition(float fX, float fY);
public:
	void Set_Player(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	
public:
	void Moving();

public:
	void PatternChange();

private:
	float m_fPatternTime;
	float m_fLifeTime;

private:
	bool m_bIsCrack;
	bool m_bIsDown;

private:
	float m_fShadowScaleX;
	float m_fShadowScaleY;

public:
	void Render_Rect();


};

