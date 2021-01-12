#pragma once
#include "Obj.h"
class CWeapon :
	public CObj
{
public:
	CWeapon();
	virtual ~CWeapon();

	// CObj을(를) 통해 상속됨
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;
	
public:
	void Render_Rect();
	void Update_Rect();
private:
	WEAPON_STATE	m_eCurWeapon;
	WEAPON_STATE	m_eNextWeapon;

public:
	void ChangeType(void);
	
public:
	void Frame(void);
	void Matrix(void);

public:
	void Reset_Rect();

public:
	void Set_WeaponTpye(WEAPON_STATE eWeapon) { m_eNextWeapon = eWeapon; }
	void Set_NextDirection(DIRECTION eDir) { m_eNextDir = eDir; }
	void Change_Direction();
	void Frame_Reset() { m_tFrame.fFrame = 0; }
private:
	bool m_bStateChange;
	bool m_bPlayFrame;

public:
	void Set_PlayWeaponFrame(bool value) { m_bPlayFrame = value; }
	
	void Load_Rect();


};

