#pragma once
#include "Scene.h"
class CStage_2 :
	public CScene
{
public:
	CStage_2();
	virtual ~CStage_2();

	// CScene을(를) 통해 상속됨
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;
};

