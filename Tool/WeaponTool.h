#pragma once
#include "afxwin.h"
#include "afxcmn.h"

class CWeaponView;
// CWeaponTool 대화 상자입니다.

class CWeaponTool : public CDialog
{
	DECLARE_DYNAMIC(CWeaponTool)

public:
	CWeaponTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CWeaponTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WEAPONTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

private:
	CListBox m_ListBox_Player;
	CListBox m_ListBox_Weapon;

private:
	wstring m_wstrWeaponStateKey;
	wstring m_wstrPlayerStateKey;
private:
	CWeaponView* m_pWeaponView;
	CCreateContext pContext;
	// 프리뷰 추가 하고 디바이스 얻어오기



public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangePlayerList();
	afx_msg void OnLbnSelchangeWeaponList();
	
private:
	// Control
	CComboBox m_ComboBox_Position;

private:
	// Control
	CSliderCtrl m_SliderCtrl_PlayerScale;
	CSliderCtrl m_SliderCtrl_WeaponScale;


public:
	afx_msg void OnBnClickedButtonReplay();
	afx_msg void OnCbnSelchangeComboPosition();

private:
	int m_iComboPosIndex;


public:
	afx_msg void OnNMReleasedcaptureSliderPlayerScale(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderWeaponScale(NMHDR *pNMHDR, LRESULT *pResult);
};
