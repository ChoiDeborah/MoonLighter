#pragma once
#include "Obj.h"
class CGuardianIntro :
	public CObj
{
public:
	CGuardianIntro();
	virtual ~CGuardianIntro();

	// CObj을(를) 통해 상속됨
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

public:
	void Frame(void);
	void Matrix(void);
};

