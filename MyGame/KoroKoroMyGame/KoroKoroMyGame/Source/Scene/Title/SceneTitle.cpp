//
// SceneTitle.cpp
// Author	 : MasayaHayashi
//

// ===== CN[h =====
#include "SceneTitle.h"
#include "../../Light/Light.h"
#include "../../Player/Player.h"
#include "../../Camera/camera.h"
#include "../../Skydome/Skydome.h"
#include "../../MainField/MainField.h"
#include "../../TitleUI/TitleUI.h"
#include "../../HeartObj/HeartObj.h"

/*
#include ""
#include "C_MainField.h"
#include "C_Camera.h"
#include "C_Light.h"
#include "input.h"
#include "C_ParticleBase.h"
#include "C_Player.h"
#include "C_TitleObj.h"
#include "FadeUI.h"
#include "C_SceneMainUI.h"
#include "C_StageEditUI.h"
#include "debugproc.h"
#include "C_TitleDesc.h"
#include <stdio.h>	// t@CüoÍp
*/

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
	pLight = new Light;
	pLight->initialize();

	// vC[ú»
	pPlayer = new Player;
	pPlayer->initialize();

	// Jú»
	pCamera = new Camera;
	pCamera->initializeTitle(pPlayer);

	// XJCh[ú»
	pSkydome = new Skydome;
	pSkydome->initialize();

	// tB[hú»
	pField = new C_MAIN_FIELD;
	pField->initializeMeshField();

	// ^CgUIú»
	pTitleUI = new TitleUI;
	pTitleUI->initialize();

	// ^CgIuWFNgú»
	pTitleObj = new HeartObj;
	pTitleObj->initialize();

	/*
	// p[eBNú»
	for (INT i = 0; i < MAX_PARTICLE; i++)
	{
		pParticle[i] = NEW C_PARTICLE_BASE(i);
		pParticle[i]->initialize();
	}
	*/
	/*

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
		pBoard[BoardCnt]->initialize();
	}

	pBoard[UI_MAIN]->setUsedFlg(false);
	*/

}

//
// ^CgI¹
//
void SceneTitle::finalize()
{
	/*
	// Jã
	pCamera->finalizeCamera();
	SAFE_DELETE(pCamera);

	// Cgã
	pLight->finalizeLight();
	SAFE_DELETE(pLight);

	// vC[ã
	pPlayer->finalizeObject();
	SAFE_DELETE(pPlayer);

	// XJCh[ã
	pSkydome->finalizeObject();
	SAFE_DELETE(pSkydome);

	// tB[hã
	pField->finalizeMeshField();
	SAFE_DELETE(pField);

	// ^CgUIã
	pTitleUI->finalizeObject();
	SAFE_DELETE(pTitleUI);

	// ^CgIuWFNgã
	pTitleObj->finalizeObject();
	SAFE_DELETE(pTitleObj);

	// p[eBNã
	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		pParticle[i]->finalizeObject();
		SAFE_DELETE(pParticle[i]);
	}

	// UIã
	for (INT BoardCnt = 0; BoardCnt < MAX_UI_TYPE; BoardCnt++)
	{
		pBoard[BoardCnt]->finalizeObject();
		SAFE_DELETE(pBoard[BoardCnt]);
	}

	*/
}

//
// XV
//
void SceneTitle::update()
{
	/*
	D3DXVECTOR3 CameraFowerd = pCamera->getCameraFowerd();

	// ^CgUIXV
	pTitleUI->updateObject();

	// p[eBNXV
	for (INT i = 0; i < MAX_PARTICLE; i++)
		pParticle[i]->updateObject();

	// vC[XV
	pPlayer->updateObject(CameraFowerd);

	// ^CgIuWFNgXV
	pTitleObj->updateObject();

	// JXV
	pCamera->updateCamera_Title(pPlayer);

	// V[JÚ
	if (getKeyboardTrigger(DIK_F1))
	{
//		PlaySound(SOUND_SE_CHANGE_SCENE);
		getSceneManager()->setSceneChange(C_SCENE_MANAGER::SCENE_STAGE_EDIT);

		bChangeScene = true;
	}
	if (getKeyboardTrigger(DIK_F2))
	{
//		PlaySound(SOUND_SE_CHANGE_SCENE);
		getSceneManager()->setSceneChange(C_SCENE_MANAGER::SCENE_MAIN);

		bChangeScene = true;
	}

	if (getKeyboardTrigger(DIK_SPACE))
	{
		if (uSelectScene == 0)
		{
//			PlaySound(SOUND_SE_CHANGE_SCENE);
			getSceneManager()->setSceneChange(C_SCENE_MANAGER::SCENE_STAGE_EDIT);

			bChangeScene = true;
		}
		else if (uSelectScene == 1)
		{
//			PlaySound(SOUND_SE_CHANGE_SCENE);
			getSceneManager()->setSceneChange(C_SCENE_MANAGER::SCENE_MAIN);

			bChangeScene = true;
		}
	}

	if (getKeyboardPress(DIK_A))
	{
		pBoard[UI_MAIN]->setUsedFlg(false);
		pBoard[UI_EDIT]->setUsedFlg(true);
		uSelectScene = 0;
	}
	if (getKeyboardPress(DIK_D))
	{
		pBoard[UI_MAIN]->setUsedFlg(true);
		pBoard[UI_EDIT]->setUsedFlg(false);
		uSelectScene = 1;
	}

	// UIXV
	for (INT BoardCnt = 0; BoardCnt < MAX_UI_TYPE; BoardCnt++)
	{
		pBoard[BoardCnt]->updateObject();
	}
	*/

}

//
// `æ
//
void SceneTitle::draw()
{
	// vC[`æ
	pPlayer->draw();

	// XJCh[`æ
//	pSkydome->draw();

	/*
	// tB[h`æ
	pField->drawMeshField();

	// ^CgS`æ
	pTitleUI->drawObject();

	// ^CgIuWFNg`æ
	pTitleObj->drawObject();

	*/

	// JZbg
	pCamera->setCamera();

	/*
	// p[eBN`æ
	for (int i = 0; i < MAX_PARTICLE; i++)
		pParticle[i]->drawObject();

	// UI`æ
	for (INT BoardCnt = 0; BoardCnt < MAX_UI_TYPE; BoardCnt++)
		pBoard[BoardCnt]->drawObject();
	*/
}

//
// Xe[^Xú»
//
void SceneTitle::initializeStatus()
{

}


//
// Jæ¾
//
Camera* SceneTitle::getCamera()
{
	return pCamera;
}