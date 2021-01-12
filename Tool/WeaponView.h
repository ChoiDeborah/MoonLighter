#pragma once


// CWeaponView 뷰입니다.
#include "Include.h"
class CWeaponView : public CView
{
	DECLARE_DYNCREATE(CWeaponView)

protected:
	CWeaponView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CWeaponView();

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

private:
	FRAME m_tFramePlayer;
	FRAME m_tFrameWeapon;
	int m_iPlayerSpriteNum;
	int m_iWeaponSpriteNum;

public:
	void Set_PlayerSpriteNum(int n) { m_iPlayerSpriteNum = n; }
	void Set_WeaponSpriteNum(int n) { m_iWeaponSpriteNum = n; }

private:
	wstring m_Player_StateKey;
	wstring m_Weapon_StateKey;

private:
	bool m_bPlayPlayerSprite;
	bool m_bPlayWeaponSprite;

public:
	void Set_PlayerFrameMax(int iMax) { m_tFramePlayer.fMax = (float)iMax; }
	void Set_PlayerFrame(int frame) { m_tFramePlayer.fFrame = (float)frame; }

public:
	void Set_WeaponFrameMax(int iMax) { m_tFrameWeapon.fMax = (float)iMax; }
	void Set_WeaponFrame(int frame) { m_tFrameWeapon.fFrame = (float)frame; }

public:
	void Set_PlayPlayerSprite(bool bValue) { m_bPlayPlayerSprite = bValue; }
	void Set_PlayWeaponSprite(bool bValue) { m_bPlayWeaponSprite = bValue; }

public:
	void Set_Player_StateKey(wstring PlayerStateKey) { m_Player_StateKey = PlayerStateKey; }
	void Set_Weapon_StateKey(wstring WeaponStateKey) { m_Weapon_StateKey = WeaponStateKey; }
	afx_msg void OnTimer(UINT_PTR nIDEvent);

public:
	void ChangeFrame();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

public:
	void Set_WeaponScale(int sacle);
	void Set_PlayerScale(int scale);

private:
	int m_iIndexPos;
	
public:
	void Set_IndexPos(int index) { m_iIndexPos = index; }


};