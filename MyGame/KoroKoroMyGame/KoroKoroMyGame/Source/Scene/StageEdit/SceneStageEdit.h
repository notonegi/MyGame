//������������������������������������������������
// SceneStageEdit.h
// Author : Masaya Hayashi
//������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef SCENE_STAGE_EDIT_H
#define SCENE_STAGE_EDIT_H

// ===== �C���N���[�h�� =====
#include "../../SceneInterface/sceneBase.h"
#include "../../Edit/Editor/StageEditor.h"
#include <memory>

// ===== �萔�E�}�N����` =====

// ===== �񋓑̒�` =====

// ===== �N���X�̑O���錾 =====
class Camera;
class Skydome;

// ===== �N���X��` =====
class SceneStageEdit : public SceneBase
{
public:
	SceneStageEdit();	// �R���X�g���N�^
	~SceneStageEdit();	// �f�X�g���N�^

	void initialize();
	void finalize();
	void update();
	void draw();

	void initializeStatus();
	
	// �Q�b�^�[�֘A
	Camera* getCamera();

private:

	std::unique_ptr<Skydome>		skydomePtr;
	std::unique_ptr<StageEditor>	stageEditorPtr;
};

#endif