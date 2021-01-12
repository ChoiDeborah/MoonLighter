#include "stdafx.h"
#include "Stage_2.h"

#include "BackGround.h"
#include "ObjMgr.h"
#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "ObjFactory.h"

#include "Guardian.h"
#include "Player.h"

#include "LineMgr.h"
#include "CollisionMgr.h"
#include "SoundMgr.h"




CStage_2::CStage_2()
{
}


CStage_2::~CStage_2()
{
	Release();
}

HRESULT CStage_2::Initialize(void)
{
	CSoundMgr::Get_Instance()->PlayBGM(L"desert_boss_track.wav");
	CLineMgr::Get_Instance()->Initialize();
	CObj* pBackGround = new CBackGround;
	dynamic_cast<CBackGround*>(pBackGround)->Set_Stage(2);
	pBackGround->Initialize();
	CObjMgr::Get_Instance()->AddObject(OBJ_BACK, pBackGround);

	CScrollMgr::Set_ScrollX(-435);
	CScrollMgr::Set_ScrollY(-600);

	CObjMgr::Get_Instance()->AddObject(OBJ_PLAYER, CObjFactory<CPlayer>::CreateObj());
	
	CGuardian* pGuardian = new CGuardian;
	pGuardian->Set_Target(CObjMgr::Get_Instance()->Get_Object(OBJID::OBJ_PLAYER)->front());
	pGuardian->Initialize();
	CObjMgr::Get_Instance()->AddObject(OBJ_BOSS_B, pGuardian);

	return E_NOTIMPL;
}

int CStage_2::Progress(void)
{
	int iResult = 0;
	CLineMgr::Get_Instance()->Progress();
	iResult = CObjMgr::Get_Instance()->Progress();

	vector<CTile*>* pVecTile = dynamic_cast<CBackGround*>(CObjMgr::Get_Instance()->Get_Object(OBJID::OBJ_BACK)->front())->GetTile();

	
	CCollisionMgr::CollisionTileRectEX(*pVecTile, *CObjMgr::Get_Instance()->Get_Object(OBJID::OBJ_BOSS_B), OBJ_BOSS_B);
	
	CCollisionMgr::CollisionTileRectEX(*pVecTile, *CObjMgr::Get_Instance()->Get_Object(OBJID::OBJ_BODY_A), OBJ_BODY_A);

	
	return iResult;
}

void CStage_2::Render(void)
{
	CObjMgr::Get_Instance()->Render();
	CLineMgr::Get_Instance()->Render();
}

void CStage_2::Release(void)
{
	CLineMgr::Get_Instance()->Destroy_Instance();
}
