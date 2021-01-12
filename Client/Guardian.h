#pragma once
#include "Obj.h"

class CGuardianIntro;

class CGuardian :
	public CObj
{
private:
	enum GARDIAN_STATE 
	{
		INTRO
		, IDLE_UP, IDLE_DOWN, IDLE_RIGHT, IDLE_LEFT
		, CHARGE_UP, CHARGE_DOWN, CHARGE_RIGHT, CHARGE_LEFT
		, CRASH_UP, CRASH_DOWN, CRASH_RIGHT, CRASH_LEFT
		, STTACK_UP, STTACK_DOWN, STTACK_RIGHT, STTACK_LEFT
		, DEATH
		, STATE_END
	};

private:
	enum GARDIAN_PATTERN 
	{
		PATTERN_IDLE
		, PATTERN_INTRO
		, PATTERN_SCROLL
		, PATTERN_CHARGE
		, PATTERN_CRASH
		, PATTERN_STTACH
		, PATTERN_DEATH
		, PATTERN_END
	};

private:
	GARDIAN_STATE m_eCurState;
	GARDIAN_STATE m_eNextState;

private:
	GARDIAN_PATTERN m_eCurPattern;
	GARDIAN_PATTERN m_eNextPattern;

public:
	CGuardian();
	virtual ~CGuardian();

	// CObj을(를) 통해 상속됨
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

public:
	void Frame();
	void Matrix();


private:
	float m_fAngle;

public:
	void Render_Rect();

public:
	void makeNewLine();
	float m_fLineTime;

public:
	void ChangeState();

public:
	void UpdatePattern();
	void SwitchPattern();

public:
	void CheckPlayerPosition();

public:
	void AllMoveStop();
	void AllMoveActivate();

public:
	int m_iHp;
	int m_iMaxHp;

private:
	float m_fAnimationTime;

public:
	bool m_bStartStage;

private:
	bool m_bIsIntro;


private:
	CGuardianIntro* pIntro = nullptr;

public:
	void Moving();


private:
	int m_iOriginScrollX;
	int m_iOriginScrollY;
	int m_bScroll;

private:
	D3DXVECTOR2 PrevPoint;
	D3DXVECTOR2 CurPoint;

private:
	float m_fDirTime;
	// 랜덤한 시간 생성
	int m_iRandInterval;
	// 
	int m_iRandDir;

public:
	void DirectionReverse();
public:
	void MakeNewDirection();

public:
	void ChangeDirection();

private:
	float m_fRotateTime;
	float m_fTempAngle = 0.f;

public:
	void Rotate(int iDir);

	bool m_bRandNum = false;
	int  m_iRandNum = 0;
	int	m_iRandOffSet = 0;


public:
	void CollisionLock();
};

