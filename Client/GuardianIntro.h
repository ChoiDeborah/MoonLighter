#pragma once
#include "Obj.h"
class CGuardianIntro :
	public CObj
{
public:
	CGuardianIntro();
	virtual ~CGuardianIntro();

	// CObj��(��) ���� ��ӵ�
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

public:
	void Frame(void);
	void Matrix(void);
};

