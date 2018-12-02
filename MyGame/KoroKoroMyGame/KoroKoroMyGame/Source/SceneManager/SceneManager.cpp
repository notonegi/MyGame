//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_GameSceneManager.cpp
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "SceneManager.h"
#include "../CrtDebug/CrtDebug.h"
#include "../KeyBoard/Keyboard.h"
#include "../Scene/Title/SceneTitle.h"
#include "../Scene/Main/SceneMain.h"
#include "../Scene/Result/C_SceneResult.h"
#include "../Fade/FadeUI.h"

// ===== 静的メンバ =====
std::unique_ptr<SceneManager>	SceneManager::sceneManagerInstancePtr(nullptr);
std::unique_ptr<SceneBase>		SceneManager::currentScenePtr(nullptr);
std::unique_ptr<FadeUI>			SceneManager::fadePtr(nullptr);

SceneManager::SceneState	  SceneManager::nextSceneType	 = SceneManager::SceneState::SceneTitle;
SceneManager::SceneState	  SceneManager::currentSceneType = SceneManager::SceneState::SceneTitle;

bool	SceneManager::debugMode		  = false;
INT		SceneManager::frameAdvanceCnt = DebugMoveOnFream;

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
SceneManager::SceneManager()
{
	debugMode		= false;
	frameAdvanceCnt = DebugMoveOnFream;

	// C_SCENE_STAGE_EDIT
	// SCENE_STAGE_EDIT

	// SceneMain
	// SCENE_MAIN

	// C_SCENE_TITLE
	// SCENE_TITLE

	// シーン設定初期化

	loadSettingFile();

	makeStartScene();

	fadePtr.reset(NEW FadeUI);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
SceneManager::~SceneManager()
{
	fadePtr->finalize();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// インスタンス生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool SceneManager::create()
{
	if (sceneManagerInstancePtr.get() == nullptr)
	{
		sceneManagerInstancePtr.reset(NEW SceneManager());
		return true;
	}
	else
	{
		return false;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// シーン初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneManager::initialize()
{
	fadePtr->initialize();

	// シーン初期化
	currentScenePtr->initialize();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// シーン終了処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneManager::finalize()
{
	// シーン後処理
	currentScenePtr->finalize();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// シーン更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneManager::update()
{
	// デバッグモード切替え
	if (Keyboard::getTrigger(DIK_V))
	{
		debugMode = !debugMode;
	}

	// デバッグモード判定
	if (debugMode)
	{
		if (Keyboard::getPress(DIK_SPACE))
		{
			frameAdvanceCnt--;
		}

		if (frameAdvanceCnt <= 0)
		{
			frameAdvanceCnt = DebugMoveOnFream;
			currentScenePtr->update();
		}
	}
	else
	{
		// シーン更新
		currentScenePtr->update();
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フェード更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneManager::updateFade()
{
	fadePtr->update();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// シーン描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneManager::draw()
{
	// シーン描画
	currentScenePtr->draw();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フェードシーン描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void  SceneManager::drawFade()
{
	if (fadePtr->getFadeState() != FadeUI::FadeType::FadeNone)
	{
		// フェード描画
		fadePtr->draw();
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// シーンセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneManager::changeScene(SceneState Scene)
{
	switch (Scene)
	{
	case SceneState::SceneTitle:
		currentScenePtr.reset(NEW SceneTitle());
		break;
		
	case SceneState::SceneMain:
		currentScenePtr.reset(NEW SceneMain());
		break;
	/*
	case SceneState::SceneResult:
		delete currentScenePtr.get();
		currentScenePtr = NEW SceneResult();
		break;
	case SceneState::SceneStageEdit:
		delete currentScenePtr.get();
		currentScenePtr = NEW SceneStageEdit();
		break;
		*/
	// 例外処理
	default:
		break;
	}
	
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 現在のシーンのインスタンス取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
SceneBase* SceneManager::getInstanse()
{
	return currentScenePtr.get();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 現在のシーンenum取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
const SceneManager::SceneState SceneManager::getCurrentSceneType()
{
	return currentSceneType;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 外部から次のシーンをセット(ロードシーン用)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneManager::setCurrentScene(SceneState setStateType)
{
	currentSceneType = setStateType;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 次のシーンをセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneManager::setNextScene(SceneState setNextSceneType)
{
	nextSceneType = setNextSceneType;
	fadePtr->setFade(FadeUI::FadeType::FadeOut);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 次のシーン取得(ロードシーン用)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
const SceneManager::SceneState SceneManager::getNextScene()
{
	return nextSceneType;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フェード取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
const FadeUI* SceneManager::getFade() const
{
	return fadePtr.get();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// iniファイルロード
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneManager::loadSettingFile()
{
	DWORD  ret;
	ret = GetPrivateProfileString("Scene", "StartScene", "しっぱい", startScene, sizeof(startScene),"Data/debugSetting.ini");

	if (!ret)
	{
		MessageBox(nullptr, TEXT("モデル読み込みエラー"), TEXT("Error"), MB_ICONERROR);
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 初期シーン設定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneManager::makeStartScene()
{
	switch (sceneTypeMap[startScene])
	{
	case SceneState::SceneTitle :
		currentScenePtr.reset(NEW SceneTitle());

		break;
	case SceneState::SceneMain	:
		currentScenePtr.reset(NEW SceneMain());

		break;
	case SceneState::SceneStageEdit: 

		break;
	case SceneState::SceneResult:


		break;
	default:
		break;
	}

	currentSceneType = sceneTypeMap[startScene];
	nextSceneType = sceneTypeMap[startScene];
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 文字列からUINTへの変換
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneManager::changeStringToUint()
{
	
}