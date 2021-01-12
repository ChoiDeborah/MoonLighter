#pragma once
#include "Obj.h"

class CPlayer;
class CGolem :
	public CObj
{
private:
	enum STATE { IDLE, DEATH, LAUNCH, NOARM_IDLE, RECOVER, SPAWN, WAKEUP, STATE_END };

private:
	STATE m_eCurState;
	STATE m_eNextState;

public:
	CGolem();
	virtual ~CGolem();

	// CObj을(를) 통해 상속됨
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

public:
	void Render_Rect(void);

public:
	void Frame(void);
	void Matrix(void);

public:
	void ChangeState(void);

public:
	void Create_Rock();

private:
	bool m_bCreateRock;

public:
	void Set_CreateRock(bool value) { m_bCreateRock = value; }

private:
	float m_fTime;
	float m_fAngle;

private:
	float m_fWaveTime;
private:
	enum ANIMATION_PATTERN 
	{ 
		ANI_IDLE, ANI_SCROLL, ANI_WAKEUP, ANI_SPAWN, ANI_ARM, ANI_ROCK, ANI_END 
	};
	
private:
	ANIMATION_PATTERN m_eCurAnimation;
	ANIMATION_PATTERN m_eNextAnimation;

public:
	void ChangeAnimation(void);

public:
	void SwitchPattern();

private:
	float m_fAnimationTime;
	float m_fRockSpawnTime;

public:
	void CreatePunch(void);

private:
	D3DXVECTOR3 m_vPlayerPos;

private:
	CPlayer* m_pPlayer;

public:
	void Set_Player(CPlayer * pPlayer) { m_pPlayer = pPlayer; }

private:
	bool m_bStartStage;
	bool m_bStartPattern;

private:
	bool m_bIsWave;


public:
	

public:
	void CheckPlayerPosition();

public:
	void UpdatePattern();


private:
	int m_iOriginScrollX;
	int m_iOriginScrollY;

private:
	bool m_bScroll;
	bool m_bStageStart;

private:
	float m_fAttackTime;
	float m_fPatternTime;
private:
	float m_fPatternInterver;
private:
	bool m_bPatternChange;

public:
	int m_iHp;
	int m_iMaxHp;

private:
	bool m_bIsLanch = false;
	bool m_bIsRock = false;

};

