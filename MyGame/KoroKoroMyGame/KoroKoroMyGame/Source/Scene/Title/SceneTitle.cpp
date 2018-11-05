//
// SceneTitle.cpp
// Author	 : MasayaHayashi
//

// ===== CN[h =====
#include "SceneTitle.h"
#include ""
#include "C_MainField.h"
#include "C_Camera.h"
#include "C_Light.h"
#include "input.h"
#include "C_ParticleBase.h"
#include "C_Player.h"
#include "C_TitleObj.h"
#include "C_FADE.h"
#include "C_SceneMainUI.h"
#include "C_StageEditUI.h"
#include "debugproc.h"
#include "C_TitleDesc.h"
#include <stdio.h>	// t@CüoÍp

//
// RXgN^
//
SceneTitle::SceneTitle()
{
	bChangeScene = false;
	nChangeSceneWaitCnt = 0;
	uSelectScene = 0;
}

//
// fXgN^
//
SceneTitle::~SceneTitle()
{

}

//
// ú»
//
void SceneTitle::initialize()
{
	// Cgú»
	pLight = new C_LIGHT;
	pLight->InitLight();

	// vC[ú»
	pPlayer = new C_PLAYER;
	pPlayer->InitObject();

	// Jú»
	pCamera = new C_CAMERA;
	pCamera->InitCamera(pPlayer);

	// XJCh[ú»
	pSkydome = NEW C_SKYDOME;
	pSkydome->InitObject();

	// tB[hú»
	pField = NEW C_MAIN_FIELD;
	pField->InitMeshField();

	// ^CgUIú»
	pTitleUI = NEW C_TITLE_UI;
	pTitleUI->InitObject();

	// ^CgIuWFNgú»
	pTitleObj = NEW C_TITLE_OBJ;
	pTitleObj->InitObject();

	// p[eBNú»
	for (INT i = 0; i < MAX_PARTICLE; i++)
	{
		pParticle[i] = NEW C_PARTICLE_BASE(i);
		pParticle[i]->InitObject();
	}

	// 
	for (INT BoardCnt = 0; BoardCnt < MAX_UI_TYPE; BoardCnt++)
	{
		switch (BoardCnt)
		{
		case UI_MAIN:
			pBoard[BoardCnt] = NEW C_SCENE_MAIN_UI;
			break;
		case UI_EDIT:
			pBoard[BoardCnt] = NEW C_STAGE_EDIT_UI;
			break;
		case UI_DESC:
			pBoard[BoardCnt] = NEW C_TITLE_DESCR_UI;
			break;
		default:
			break;
		}
		pBoard[BoardCnt]->InitObject();
	}

	pBoard[UI_MAIN]->SetUsedFlg(false);

	// BGMÄ¶
//	PlaySound(SOUND_BGM_TITLE);
}

//
// ^CgI¹
//
void SceneTitle::UninitScene()
{
	// BGMâ~


	// Jã
	pCamera->UninitCamera();
	SAFE_DELETE(pCamera);

	// Cgã
	pLight->UninitLight();
	SAFE_DELETE(pLight);

	// vC[ã
	pPlayer->UninitObject();
	SAFE_DELETE(pPlayer);

	// XJCh[ã
	pSkydome->UninitObject();
	SAFE_DELETE(pSkydome);

	// tB[hã
	pField->UninitMeshField();
	SAFE_DELETE(pField);

	// ^CgUIã
	pTitleUI->UninitObject();
	SAFE_DELETE(pTitleUI);

	// ^CgIuWFNgã
	pTitleObj->UninitObject();
	SAFE_DELETE(pTitleObj);

	// p[eBNã
	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		pParticle[i]->UninitObject();
		SAFE_DELETE(pParticle[i]);
	}

	// UIã
	for (INT BoardCnt = 0; BoardCnt < MAX_UI_TYPE; BoardCnt++)
	{
		pBoard[BoardCnt]->UninitObject();
		SAFE_DELETE(pBoard[BoardCnt]);
	}


}

//
// XV
//
void SceneTitle::UpdateScene()
{
	D3DXVECTOR3 CameraFowerd = pCamera->GetCameraFowerd();

	// ^CgUIXV
	pTitleUI->UpdateObject();

	// p[eBNXV
	for (INT i = 0; i < MAX_PARTICLE; i++)
		pParticle[i]->UpdateObject();

	// vC[XV
	pPlayer->UpdateObject(CameraFowerd);

	// ^CgIuWFNgXV
	pTitleObj->UpdateObject();

	// JXV
	pCamera->UpdateCamera_Title(pPlayer);

	// V[JÚ
	if (GetKeyboardTrigger(DIK_F1))
	{
//		PlaySound(SOUND_SE_CHANGE_SCENE);
		GetSceneManager()->SetSceneChange(C_SCENE_MANAGER::SCENE_STAGE_EDIT);

		bChangeScene = true;
	}
	if (GetKeyboardTrigger(DIK_F2))
	{
//		PlaySound(SOUND_SE_CHANGE_SCENE);
		GetSceneManager()->SetSceneChange(C_SCENE_MANAGER::SCENE_MAIN);

		bChangeScene = true;
	}

	if (GetKeyboardTrigger(DIK_SPACE))
	{
		if (uSelectScene == 0)
		{
//			PlaySound(SOUND_SE_CHANGE_SCENE);
			GetSceneManager()->SetSceneChange(C_SCENE_MANAGER::SCENE_STAGE_EDIT);

			bChangeScene = true;
		}
		else if (uSelectScene == 1)
		{
//			PlaySound(SOUND_SE_CHANGE_SCENE);
			GetSceneManager()->SetSceneChange(C_SCENE_MANAGER::SCENE_MAIN);

			bChangeScene = true;
		}
	}

	if (GetKeyboardPress(DIK_A))
	{
		pBoard[UI_MAIN]->SetUsedFlg(false);
		pBoard[UI_EDIT]->SetUsedFlg(true);
		uSelectScene = 0;
	}
	if (GetKeyboardPress(DIK_D))
	{
		pBoard[UI_MAIN]->SetUsedFlg(true);
		pBoard[UI_EDIT]->SetUsedFlg(false);
		uSelectScene = 1;
	}

	// UIXV
	for (INT BoardCnt = 0; BoardCnt < MAX_UI_TYPE; BoardCnt++)
	{
		pBoard[BoardCnt]->UpdateObject();
	}


	/*
	// V[JÚ
	if (bChangeScene)
		nChangeSceneWaitCnt++;
	if(nChangeSceneWaitCnt > 120)
	*/

}

//
// `æ
//
void SceneTitle::DrawScene()
{
	// vC[`æ
	pPlayer->DrawObject();

	// XJCh[`æ
	pSkydome->DrawObject();

	// tB[h`æ
	pField->DrawMeshField();

	// ^CgS`æ
	pTitleUI->DrawObject();

	// ^CgIuWFNg`æ
	pTitleObj->DrawObject();

	// JZbg
	pCamera->SetCamera();

		
	// p[eBN`æ
	for (int i = 0; i < MAX_PARTICLE; i++)
		pParticle[i]->DrawObject();

	// UI`æ
	for (INT BoardCnt = 0; BoardCnt < MAX_UI_TYPE; BoardCnt++)
		pBoard[BoardCnt]->DrawObject();
	
}

//
// Xe[^Xú»
//
void SceneTitle::InitStatus()
{

}


//
// Jæ¾
//
C_CAMERA* SceneTitle::GetCamera()
{
	return pCamera;
}