//����������������������������������������������������������������
// Skydome.cpp
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "Skydome.h"
#include "../Camera/camera.h"
#include "../ResoueceManager/ResourceManager.h"
#include "../DirectX3D/DirectX3D.h"
#include <string>

//������������������������������
// �R���X�g���N�^
//������������������������������
Skydome::Skydome()
{
	strcpy_s(fileName, ModelFilePass.c_str());
	strcpy_s(texFileName, TextureFilePass.c_str());

	myTransform.scale = (D3DXVECTOR3(Scale,Scale,Scale));
	textureSize  = D3DXVECTOR3(10.0f, 10.0f, 10.0f);

	myTransform.isUsed = true;
}

//������������������������������
// �f�X�g���N�^
//������������������������������
Skydome::~Skydome()
{

}

//������������������������������
// �X�J�C�h�[��������
//������������������������������
void Skydome::initialize()
{
	ResourceManager::makeModel(meshDataObj, fileName, meshType);
	ResourceManager::createTexture(textureData, texFileName);
}

//������������������������������
// �X�J�C�h�[���㏈��
//������������������������������
void Skydome::finalize()
{
	ResourceManager::destroyAllMesh();
}

//������������������������������
// �X�J�C�h�[���X�V
//������������������������������
void Skydome::update()
{

}

//������������������������������
// �X�J�C�h�[���`��
//������������������������������
void Skydome::draw()
{
	Pawn::draw();
}

//������������������������������
// �X�J�C�h�[���`��
//������������������������������
void Skydome::draw(D3DXMATRIX mtxView, D3DXMATRIX mtxProj)
{
	Pawn::draw();
}

//��������������������������������������
// �e�N�X�`������
//��������������������������������������
HRESULT Skydome::createTexture()
{
	// ��O����
	if (!texFileName)
	{
		return E_FAIL;
	}

	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(devicePtr, texFileName, &pD3DTexture);

	return S_OK;
}