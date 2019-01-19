//����������������������������������������������������������������
// MainObject.cpp
// �u���b�N�N���X
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "MainObject.h"
#include "../ResoueceManager/ResourceManager.h"
#include "../Collision/Collision.h"
#include "../SceneManager/SceneManager.h"

//������������������������������������������������
// �R���X�g���N�^
//������������������������������������������������
MainObject::MainObject()
{

}

//������������������������������������������������
// �R���X�g���N�^
// modelPass:	 ���f���p�X����
// texturePass : �e�N�X�`���p�X����
//������������������������������������������������
MainObject::MainObject(std::string modelPass,std::string texturePass, size_t setNumber,bool setFieldModel)
{

	tagName						= modelPass;
	idNumber					= setNumber;
	modelPasses[tagName]		= passName + modelPass;
	texturePasses[tagName]		= passName + texturePass;
	myGameObjType		 = GameObjectType::NormalBlockObj;
	isFieldObject = setFieldModel;

	
	isUsed = false;
}

//����������������������������������������������
// �f�X�g���N�^
//����������������������������������������������
MainObject::~MainObject()
{

}

//����������������������������������������������
// �u���b�N������
//����������������������������������������������
void MainObject::initialize()
{
	// �t�@�C���p�X�ݒ�
	strcpy_s(fileName, modelPasses[tagName].c_str());
	strcpy_s(texFileName, texturePasses[tagName].c_str());

	// ���f������
	ResourceManager::makeModel(meshDataObj, fileName, meshType);
	ResourceManager::createTexture(textureData, texFileName);

	myTransform.collisionBox = meshDataObj.collitionBox;

	
	Collision::registerList(&myTransform, tagName);


	// �ʒu�A�ړ��ʁA�g�嗦������
	myTransform.scale	= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	myTransform.rotDeg	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.pos		= D3DXVECTOR3(0.0f, -meshDataObj.collitionBox.y * 2, 0.0f);
}

//��������������������������������������������
// �u���b�N�㏈��
//��������������������������������������������
void MainObject::finalize()
{
	ResourceManager::destroyAllMesh();

	modelPasses.clear();
	texturePasses.clear();
	ResourceManager::destroyAllTexture();

	Collision::release(tagName);
}

//����������������������������������������������
// �u���b�N�X�V
//����������������������������������������������
void MainObject::update()
{
	// �R���C�_�[�X�V
//	pCollider->UpdateCollider(mtxWorldPawn, FIELD_BOX_COLOR );

	updateExportData();
}

//����������������������������������
// �u���b�N�`��
//����������������������������������
void MainObject::draw()
{
	if (!isUsed)
	{
		return;
	}

	Pawn::draw();
}

//������������������������������������������
// �`��
//������������������������������������������
void MainObject::draw(D3DXMATRIX mtxView, D3DXMATRIX mtxProj)
{
	// ��O����
	if (!isUsed)
	{
		return;
	}

	// �`��
	Pawn::draw(mtxView,mtxProj);
}

//����������������������������������
// �L�[�l�擾
//����������������������������������
const std::string MainObject::getKeyName()
{
	return tagName;
}