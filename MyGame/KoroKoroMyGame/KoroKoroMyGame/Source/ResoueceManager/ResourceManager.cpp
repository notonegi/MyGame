//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_ResourceManager.cpp
// リソース管理(メッシュ、テクスチャ管理)
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
#define _CRT_SECURE_NO_WARNINGS

// ===== インクルード部 =====
#include "ResourceManager.h"
#include <string>
#include <tchar.h>
#include <utility>
#include "../CrtDebug/CrtDebug.h"
#include "../Pawn/Pawn.h"
#include "../Board/Board.h"
#include "../DirectX3D/DirectX3D.h"
#include "../Pawn/Pawn.h"

// ===== 静的メンバ変数 =====
std::unique_ptr<ResourceManager>    ResourceManager::instancePtr = nullptr;
std::vector <MeshData*>				ResourceManager::meshes;
std::vector<TEXTURE_DATA*>			ResourceManager::texture;				// テクスチャ情報
std::vector<TEXTURE_DATA*>			ResourceManager::fadeTexture;			// テクスチャ情報
std::vector<VERTEX_BOARD_DATA*>		ResourceManager::vtxBoard;				// ボード頂点情報
std::vector<VERTEX_BOARD_DATA*>		ResourceManager::vtxFadeBoard;			// ボード頂点情報
std::vector<HIERARCHY_MESH_DATA*>	ResourceManager::hierarchyMesh;

#define SAFE_RELEASE(p)		 { if (p) { (p)->Release(); (p)=NULL; }}
#define SAFE_DELETE(p)	     { if( p != nullptr ){ delete p;  p = nullptr; }}
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=nullptr; }}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
ResourceManager::ResourceManager()
{
	
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
ResourceManager::~ResourceManager()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// モデル生成(階層構造無し)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT ResourceManager::makeModel(MeshData &meshData, CHAR *pszFilename, MeshObjType &umeshType)
{
	// 例外処理
	if (!pszFilename)
	{
		return E_FAIL;
	}

	// メッシュの種類
	umeshType = MeshObjType::NormalModel;

	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr =  DirectX3D::getDevice();

	// 既に生成されているか
	if (checkExisting(pszFilename, meshData))
	{
		return S_OK;
	}

	// ファイル名セット
	strcpy_s(meshData.meshFileName, pszFilename);

	// メッシュ情報セット

	meshes.push_back(&meshData);
	
	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(pszFilename, D3DXMESH_SYSTEMMEM, devicePtr, nullptr, &meshes.back()->materialBufferPtr, nullptr, &meshes.back()->numMat, &meshes.back()->meshPtr)))
	{
		MessageBox(nullptr, TEXT("Error"), TEXT("モデル読み込みエラー"), MB_ICONERROR);
		return E_FAIL;
	}

	// 属性取得のための下準備
	LPD3DXMESH pmeshWk;
	HRESULT hr = meshes.back()->meshPtr->Optimize(D3DXMESHOPT_ATTRSORT, nullptr, nullptr, nullptr, nullptr, &pmeshWk);

	if (SUCCEEDED(hr))
	{
		//meshes.back()->meshPtr->Release();
		meshes.back()->meshPtr = pmeshWk;
	}
	else
	{
		//SAFE_RELEASE(meshes.back()->materialBufferPtr);	// マテリアル解放
		return false;
	}

	// 属性テーブル取得
	meshes.back()->dwAttrNum = 0;
	meshes.back()->meshPtr->GetAttributeTable(nullptr, &meshes.back()->dwAttrNum);
	meshes.back()->attrPtr = NEW D3DXATTRIBUTERANGE[meshes.back()->dwAttrNum];
	meshes.back()->meshPtr->GetAttributeTable(meshes.back()->attrPtr, &meshes.back()->dwAttrNum);

	// 指定の頂点フォーマットに変換
	DWORD dwFVF = meshes.back()->meshPtr->GetFVF();
	if (dwFVF != FVF_TVERTEX)
	{
		LPD3DXMESH pmeshTmp = meshes.back()->meshPtr.Get();
		pmeshTmp->CloneMeshFVF(pmeshTmp->GetOptions(), FVF_TVERTEX, devicePtr, &meshes.back()->meshPtr);
		delete pmeshTmp;
		pmeshTmp = nullptr;
	//	SAFE_RELEASE(pmeshTmp);
		// 法線が無い場合は強制的に追加
		if ((dwFVF & D3DFVF_NORMAL) == 0)
		{
			D3DXComputeNormals(meshes.back()->meshPtr.Get(), nullptr);
		}
	}

	// 頂点情報サイズ数取得
	meshes.back()->dwNumVtx = meshes.back()->meshPtr->GetNumVertices();	// 頂点数取得
	meshes.back()->vertexPtr = NEW MESH_VTX[meshes.back()->dwNumVtx];
	LPVOID pWork;

	// 頂点バッファアンロック
	meshes.back()->meshPtr->LockVertexBuffer(D3DLOCK_READONLY, &pWork);
	CopyMemory(meshes.back()->vertexPtr, pWork, sizeof(MESH_VTX) * meshes.back()->dwNumVtx);

	// 頂点バッファロック
	meshes.back()->meshPtr->UnlockVertexBuffer();

	// インデックスバッファから読み込み
	meshes.back()->dwNumIndx = meshes.back()->meshPtr->GetNumFaces() * 3;
	meshes.back()->indexPtr = NEW WORD[meshes.back()->dwNumIndx];

	// インデックスバッファロック
	meshes.back()->meshPtr->LockIndexBuffer(D3DLOCK_READONLY, &pWork);
	CopyMemory(meshes.back()->indexPtr, pWork, sizeof(WORD) * meshes.back()->dwNumIndx);

	// インデックスバッファアンロック
	meshes.back()->meshPtr->UnlockIndexBuffer();

	// 境界ボックスと中心座標を求める
	meshes.back()->maxVtx = meshes.back()->vertexPtr[0].VtxPos;
	meshes.back()->minVtx = meshes.back()->vertexPtr[0].VtxPos;
	for (WORD i = 1; i < meshes.back()->dwNumVtx; i++)
	{
		// 最大値取得
		if (meshes.back()->maxVtx.x < meshes.back()->vertexPtr[i].VtxPos.x)
		{
			meshes.back()->maxVtx.x = meshes.back()->vertexPtr[i].VtxPos.x;
		}

		if (meshes.back()->maxVtx.y < meshes.back()->vertexPtr[i].VtxPos.y)
		{
			meshes.back()->maxVtx.y = meshes.back()->vertexPtr[i].VtxPos.y;
		}

		if (meshes.back()->maxVtx.z < meshes.back()->vertexPtr[i].VtxPos.z)
		{
			meshes.back()->maxVtx.z = meshes.back()->vertexPtr[i].VtxPos.z;
		}

		// 最小値取得
		if (meshes.back()->minVtx.x > meshes.back()->vertexPtr[i].VtxPos.x)
		{
			meshes.back()->minVtx.x = meshes.back()->vertexPtr[i].VtxPos.x;
		}

		if (meshes.back()->minVtx.y > meshes.back()->vertexPtr[i].VtxPos.y)
		{
			meshes.back()->minVtx.y = meshes.back()->vertexPtr[i].VtxPos.y;
		}
		if (meshes.back()->minVtx.z > meshes.back()->vertexPtr[i].VtxPos.z)
		{
			meshes.back()->minVtx.z = meshes.back()->vertexPtr[i].VtxPos.z;
		}
	}

	// 中心座標、あたり判定取得
	meshes.back()->centerPos = (meshes.back()->maxVtx + meshes.back()->minVtx) * 0.5f;
	meshes.back()->collitionBox = (meshes.back()->maxVtx - meshes.back()->minVtx) * 0.5f;

	// 境界球取得
	FLOAT dx = meshes.back()->vertexPtr->VtxPos.x - meshes.back()->centerPos.x;
	FLOAT dy = meshes.back()->vertexPtr->VtxPos.y - meshes.back()->centerPos.y;
	FLOAT dz = meshes.back()->vertexPtr->VtxPos.z - meshes.back()->centerPos.z;
	meshes.back()->collisionRadus = sqrtf(dx * dx + dy * dy + dz * dz);

	for (DWORD i = 1; i < meshes.back()->dwNumVtx; i++)
	{
		dx = meshes.back()->vertexPtr[i].VtxPos.x - meshes.back()->centerPos.x;
		dy = meshes.back()->vertexPtr[i].VtxPos.y - meshes.back()->centerPos.y;
		dz = meshes.back()->vertexPtr[i].VtxPos.z - meshes.back()->centerPos.z;
		float fRadius = sqrtf(dx * dx + dy * dy + dz * dz);
		if (meshes.back()->collisionRadus < fRadius)
		{
			meshes.back()->collisionRadus = fRadius;
		}
	}

	return S_OK;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 頂点情報生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT ResourceManager::makevertexBoard(VERTEX_BOARD_DATA &VtxBordData, CHAR *pszName)
{
	// 識別用ファイル名セット
	strcpy_s(VtxBordData.name, pszName);

	// フェード用判定
	if (VtxBordData.fade)
	{
		makeVtxFade(VtxBordData);
	}
	else
	{
		makeVtx(VtxBordData);
	}

	return S_OK;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 頂点情報生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool ResourceManager::makeVtx(VERTEX_BOARD_DATA &vtxBoardData)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// 頂点情報情報セット
	vtxBoard.push_back(&vtxBoardData);

	// 頂点情報生成
	switch (vtxBoardData.boardType)
	{
	case  boardType::Polygon2d:

		// オブジェクトの頂点バッファを生成
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_2D) * (DirectX3D::VertexSize),	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
			FVF_VERTEX_2D,								// 使用する頂点フォーマット
			D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
			&vtxBoard.back()->pD3DVtxBuff,			// 頂点バッファインターフェースへのポインタ
			nullptr)))										// nullptrに設定
		{
			return false;
		}

		{//頂点バッファの中身を埋める
			VERTEX_2D *pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			vtxBoard.back()->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			{
				// 頂点座標の設定
				pVtx[0].vtx = D3DXVECTOR3(vtxBoardData.pos.x - vtxBoard.back()->size.x * 0.5f, vtxBoard.back()->pos.y + vtxBoard.back()->size.y * 0.5f, 0.0f);
				pVtx[1].vtx = D3DXVECTOR3(vtxBoardData.pos.x + vtxBoard.back()->size.x * 0.5f, vtxBoard.back()->pos.y + vtxBoard.back()->size.y * 0.5f, 0.0f);
				pVtx[2].vtx = D3DXVECTOR3(vtxBoardData.pos.x - vtxBoard.back()->size.x * 0.5f, vtxBoard.back()->pos.y - vtxBoard.back()->size.y * 0.5f, 0.0f);
				pVtx[3].vtx = D3DXVECTOR3(vtxBoardData.pos.x + vtxBoard.back()->size.x * 0.5f, vtxBoard.back()->pos.y - vtxBoard.back()->size.y * 0.5f, 0.0f);

				// rhwの設定
				pVtx[0].rhw =
					pVtx[1].rhw =
					pVtx[2].rhw =
					pVtx[3].rhw = 1.0f;

				// 反射光の設定
				pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}

			// 頂点データをアンロックする
			vtxBoard.back()->pD3DVtxBuff->Unlock();
		}
		return true;
	case boardType::Billboard:
	{
		// オブジェクトの頂点バッファを生成
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * DirectX3D::VertexSize,	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
			FVF_VERTEX_3D,								// 使用する頂点フォーマット
			D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
			&vtxBoard.back()->pD3DVtxBuff,			// 頂点バッファインターフェースへのポインタ
			nullptr)))										// nullptrに設定
		{
			return false;
		}

		//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		vtxBoard.back()->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].vtx = D3DXVECTOR3(-vtxBoardData.size.x, vtxBoardData.size.y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(vtxBoardData.size.x, vtxBoardData.size.y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-vtxBoardData.size.x, -vtxBoardData.size.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(vtxBoardData.size.x, -vtxBoardData.size.y, 0.0f);

		// 法線の設定
		if (vtxBoardData.size.z == 0.0f)
		{
			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		}
		else
		{
			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		}

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		vtxBoard.back()->pD3DVtxBuff->Unlock();
	}
		return true;
	case boardType::Polygon3d:
		// オブジェクトの頂点バッファを生成
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * DirectX3D::VertexSize,	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
			FVF_VERTEX_3D,				// 使用する頂点フォーマット
			D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
			&vtxBoardData.pD3DVtxBuff,			// 頂点バッファインターフェースへのポインタ
			nullptr)))						// nullptrに設定
		{
			return false;
		}

		//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		vtxBoard.back()->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


		pVtx[0].vtx = D3DXVECTOR3(-vtxBoardData.size.x, vtxBoardData.size.y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(vtxBoardData.size.x, vtxBoardData.size.y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-vtxBoardData.size.x, -vtxBoardData.size.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(vtxBoardData.size.x, -vtxBoardData.size.y, 0.0f);

		// 法線の設定
		if (vtxBoardData.size.z == 0.0f)
		{
			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		}
		else
		{
			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		}

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


		/*
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		*/
		// 頂点データをアンロックする
		vtxBoard.back()->pD3DVtxBuff->Unlock();

		return true;
	default:
		break;
	}

	return false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フェード用頂点情報生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool ResourceManager::makeVtxFade(VERTEX_BOARD_DATA &vtxBoardData)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// 頂点情報情報セット
	vtxFadeBoard.push_back(&vtxBoardData);

	// 頂点情報生成
	switch (vtxBoardData.boardType)
	{
	case boardType::Polygon2d:

		// オブジェクトの頂点バッファを生成
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_2D) * DirectX3D::VertexSize,	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
			FVF_VERTEX_2D,								// 使用する頂点フォーマット
			D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
			&vtxFadeBoard.back()->pD3DVtxBuff,			// 頂点バッファインターフェースへのポインタ
			nullptr)))										// nullptrに設定
		{
			return false;
		}

		{//頂点バッファの中身を埋める
			VERTEX_2D *pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			vtxFadeBoard.back()->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			{
				// 頂点座標の設定
				pVtx[0].vtx = D3DXVECTOR3(vtxBoardData.pos.x - vtxFadeBoard.back()->size.x * 0.5f, vtxFadeBoard.back()->pos.y + vtxFadeBoard.back()->size.y * 0.5f, 0.0f);
				pVtx[1].vtx = D3DXVECTOR3(vtxBoardData.pos.x + vtxFadeBoard.back()->size.x * 0.5f, vtxFadeBoard.back()->pos.y + vtxFadeBoard.back()->size.y * 0.5f, 0.0f);
				pVtx[2].vtx = D3DXVECTOR3(vtxBoardData.pos.x - vtxFadeBoard.back()->size.x * 0.5f, vtxFadeBoard.back()->pos.y - vtxFadeBoard.back()->size.y * 0.5f, 0.0f);
				pVtx[3].vtx = D3DXVECTOR3(vtxBoardData.pos.x + vtxFadeBoard.back()->size.x * 0.5f, vtxFadeBoard.back()->pos.y - vtxFadeBoard.back()->size.y * 0.5f, 0.0f);

				// rhwの設定
				pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

				// 反射光の設定
				pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}

			// 頂点データをアンロックする
			vtxFadeBoard.back()->pD3DVtxBuff->Unlock();
		}
		return true;
	case boardType::Billboard:
	{
		// オブジェクトの頂点バッファを生成
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * DirectX3D::VertexSize,	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
			FVF_VERTEX_3D,								// 使用する頂点フォーマット
			D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
			&vtxFadeBoard.back()->pD3DVtxBuff,			// 頂点バッファインターフェースへのポインタ
			nullptr)))										// nullptrに設定
		{
			return false;
		}

		//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		vtxFadeBoard.back()->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].vtx = D3DXVECTOR3(-vtxBoardData.size.x, vtxBoardData.size.y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(vtxBoardData.size.x, vtxBoardData.size.y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-vtxBoardData.size.x, -vtxBoardData.size.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(vtxBoardData.size.x, -vtxBoardData.size.y, 0.0f);
		
		// 法線の設定
		if (vtxBoardData.size.z == 0.0f)
		{
			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		}
		else
		{
			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		}

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		vtxFadeBoard.back()->pD3DVtxBuff->Unlock();
	}
		return true;

	case boardType::Polygon3d:
		// オブジェクトの頂点バッファを生成
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * DirectX3D::VertexSize,	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
			FVF_VERTEX_3D,				// 使用する頂点フォーマット
			D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
			&vtxFadeBoard.back()->pD3DVtxBuff,			// 頂点バッファインターフェースへのポインタ
			nullptr)))						// nullptrに設定
		{
			return false;
		}

		//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		vtxFadeBoard.back()->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].vtx = D3DXVECTOR3(-vtxBoardData.size.x, vtxBoardData.size.y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(vtxBoardData.size.x, vtxBoardData.size.y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-vtxBoardData.size.x, -vtxBoardData.size.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(vtxBoardData.size.x, -vtxBoardData.size.y, 0.0f);

		// 法線の設定
		if (vtxBoardData.size.z == 0.0f)
		{
			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		}
		else
		{
			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		}

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		vtxFadeBoard.back()->pD3DVtxBuff->Unlock();

		return true;
	default:
		break;
	}

	return false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// テクスチャ生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT ResourceManager::createTexture(TEXTURE_DATA &TextureData,  CHAR *pszFilename)
{
	// 例外処理
	if (!pszFilename)
	{
		return E_FAIL;
	}

	// 既に生成されているか
	if (checkExisting(pszFilename, TextureData))
	{
		return S_OK;
	}

	// ファイル名セット
	strcpy_s(TextureData.texFileName, pszFilename);

	texture.push_back(&TextureData);

	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// テクスチャの読み込み
	if (D3DXCreateTextureFromFile(devicePtr, texture.back()->texFileName, &texture.back()->pD3DTexture))
	{
		return S_OK;
	}
	else
	{
	//	MessageBox(nullptr, TEXT("テクスチャ読み込みエラー"), TEXT("Error"), MB_ICONERROR);
		return E_FAIL;
	}
}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 階層構造用モデル読み込み
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT ResourceManager::makeModelHierarchy(HIERARCHY_MESH_DATA &setHierarchyMeshData,CHAR *pszFilename,std::string keyName, MeshObjType &meshType)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = DirectX3D::getDevice();

	// メッシュの種類初期化
	meshType = MeshObjType::HierarchyModel;

	// メッシュ情報セット
	strcpy_s(setHierarchyMeshData.szMeshFileName, pszFilename);	// ファイル名セット

	hierarchyMesh.push_back(&setHierarchyMeshData);

	// ディレクトリ抽出
	TCHAR szDir[_MAX_PATH];
	TCHAR szDirWk[_MAX_DIR];
	_tsplitpath(hierarchyMesh.back()->szMeshFileName, szDir, szDirWk, NULL, NULL);
	lstrcat(szDir, szDirWk);
	hierarchyMesh.back()->Hierarchy.setDirectory(szDir);

	// 階層構造メッシュの読み込み
	HRESULT hr = D3DXLoadMeshHierarchyFromX(hierarchyMesh.back()->szMeshFileName, D3DXMESH_MANAGED, pDevice, &hierarchyMesh.back()->Hierarchy, NULL, &hierarchyMesh.back()->pFrameRoot, &hierarchyMesh.back()->pAnimCtrl);
	if (FAILED(hr))
		return false;

	// ボーンとフレームの関連付け
	hr = allocAllBoneMatrix(hierarchyMesh.back()->pFrameRoot, pszFilename);
	if (FAILED(hr)) return false;

	// アニメーションセット取得
	hierarchyMesh.back()->uNumAnimSet = 0;
	if (hierarchyMesh.back()->pAnimCtrl)
	{
		hierarchyMesh.back()->uNumAnimSet = hierarchyMesh.back()->pAnimCtrl->GetNumAnimationSets();
		if (hierarchyMesh.back()->uNumAnimSet > 0)
		{
			hierarchyMesh.back()->ppAnimSet = NEW LPD3DXANIMATIONSET[hierarchyMesh.back()->uNumAnimSet];
			for (DWORD u = 0; u < hierarchyMesh.back()->uNumAnimSet; u++)
			{
				hierarchyMesh.back()->pAnimCtrl->GetAnimationSet(u, &hierarchyMesh.back()->ppAnimSet[u]);
			}
		}
	}

	if (hierarchyMesh.back()->pFrameRoot)
	{
		// マトリックス更新
		setTime(0.0, pszFilename);
		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);
		updateFrameMatrices(hierarchyMesh.back()->pFrameRoot, &world);

		// 境界球/境界ボックス取得
		calcCollision(hierarchyMesh.back()->pFrameRoot, pszFilename);
	}

	// 経過時間計測用時刻設定
	hierarchyMesh.back()->dwPrev = timeGetTime();

	return SUCCEEDED(hr);
}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フレームのマトリックスを更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void ResourceManager::updateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
	MYFRAME* pFrame = (MYFRAME*)pFrameBase;
	// 親のマトリックスを掛け合わせる
	if (pParentMatrix)
	{
		pFrame->combinedTransformationMatrix = pFrame->TransformationMatrix * *pParentMatrix;
	}
	else
	{
		pFrame->combinedTransformationMatrix = pFrame->TransformationMatrix;
	}

	// 兄弟フレームがあればマトリックスを更新
	if (pFrame->pFrameSibling)
	{
		updateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
	}
	// 子フレームがあればマトリックスを更新
	if (pFrame->pFrameFirstChild)
	{
		// 自分が親となるので、自分のマトリックスを渡す
		updateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->combinedTransformationMatrix);
	}
}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// アニメーション開始時間設定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void ResourceManager::setTime(DOUBLE dTime,CHAR *pszFilename)
{
	if (hierarchyMesh.back()->pAnimCtrl == nullptr)
	{
		return;
	}

	for (DWORD i = 0; i < hierarchyMesh.back()->pAnimCtrl->GetMaxNumTracks(); ++i)
	{
		hierarchyMesh.back()->pAnimCtrl->SetTrackPosition(i, 0);
	}

	hierarchyMesh.back()->pAnimCtrl->ResetTime();
	hierarchyMesh.back()->pAnimCtrl->AdvanceTime(dTime, nullptr);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ボーン用ワールド・マトリックス領域確保
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT ResourceManager::allocBoneMatrix(LPD3DXMESHCONTAINER meshContainerPtrBase, CHAR *pszFilename)
{
	MYMESHCONTAINER* meshContainerPtr = (MYMESHCONTAINER*)meshContainerPtrBase;

	if (meshContainerPtr->pSkinInfo == nullptr)
	{
		return S_OK;	// スキン情報が無ければ何もしない
	}

	DWORD dwBoneNum = meshContainerPtr->pSkinInfo->GetNumBones();
	meshContainerPtr->ppBoneMatrix = NEW LPD3DXMATRIX[dwBoneNum];

	for (DWORD i = 0; i < dwBoneNum; i++)
	{
		MYFRAME* pFrame = (MYFRAME*)D3DXFrameFind(hierarchyMesh.back()->pFrameRoot, meshContainerPtr->pSkinInfo->GetBoneName(i));

		if (pFrame == nullptr)
		{
			return E_FAIL;
		}

		meshContainerPtr->ppBoneMatrix[i] = &pFrame->combinedTransformationMatrix;
	}
	return S_OK;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ボーン用ワールド・マトリックス領域確保
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT ResourceManager::allocAllBoneMatrix(LPD3DXFRAME pFrameBase, CHAR *pszFilename)
{
	MYFRAME* pFrame = (MYFRAME*)pFrameBase;
	HRESULT hr = S_OK;
	if (pFrame->pMeshContainer)
	{
		hr = allocBoneMatrix(pFrame->pMeshContainer,pszFilename);
		if (FAILED(hr))
		{
			return hr;
		}
	}
	if (pFrame->pFrameSibling)
	{
		hr = allocAllBoneMatrix(pFrame->pFrameSibling,pszFilename);
		if (FAILED(hr))
		{
			return hr;
		}
	}
	if (pFrame->pFrameFirstChild)
	{
		hr = allocAllBoneMatrix(pFrame->pFrameFirstChild, pszFilename);
	}

	return hr;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
//  階層構造用 境界球/境界ボックス取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void ResourceManager::calcCollision(LPD3DXFRAME pFrame,CHAR *pszFilename)
{
	D3DXVECTOR3& vMax = hierarchyMesh.back()->CollitionBox;
	D3DXVECTOR3& vMin = hierarchyMesh.back()->CenterPos;

	vMax.x = vMax.y = vMax.z = -FLT_MAX;
	vMin.x = vMin.y = vMin.z = FLT_MAX;
	
	hierarchyMesh.back()->fCollisionRadus = -1.0f;
	calcCollisionFrame(pFrame,pszFilename);
	
	D3DXVECTOR3 vBBox, vCenter;
	vBBox = (vMax - vMin) / 2.0f;
	vCenter = (vMax + vMin) / 2.0f;
	
	hierarchyMesh.back()->CollitionBox = vBBox;
	hierarchyMesh.back()->CenterPos = vCenter;
	hierarchyMesh.back()->fCollisionRadus = 0.0f;
	calcCollisionFrame(pFrame,pszFilename);
}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フレーム毎の頂点座標の抽出
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void ResourceManager::calcCollisionFrame(LPD3DXFRAME pFrame, CHAR *pszFilename)
{
	LPD3DXMESHCONTAINER meshContainerPtr = pFrame->pMeshContainer;
	while (meshContainerPtr)
	{
		// メッシュコンテナ毎の頂点座標の抽出
		calcCollisionMeshContainer(meshContainerPtr, pFrame,pszFilename);
		// 次のメッシュコンテナ
		meshContainerPtr = meshContainerPtr->pNextMeshContainer;
	}
	// 兄弟フレームがあれば兄弟フレームを描画
	if (pFrame->pFrameSibling)
	{
		calcCollisionFrame(pFrame->pFrameSibling, pszFilename);
	}
	// 子フレームがあれば子フレームを描画
	if (pFrame->pFrameFirstChild)
	{
		calcCollisionFrame(pFrame->pFrameFirstChild, pszFilename);
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// メッシュコンテナ毎の頂点座標の抽出
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void ResourceManager::calcCollisionMeshContainer(LPD3DXMESHCONTAINER meshContainerPtr, LPD3DXFRAME pFrame, CHAR *pszFilename)
{
	D3DXMATRIX& matrix = ((MYFRAME*)pFrame)->combinedTransformationMatrix;
	// 頂点座標の抽出
	LPD3DXMESH pmesh = meshContainerPtr->MeshData.pMesh;
	DWORD dwStride = pmesh->GetNumBytesPerVertex();
	DWORD dwVtx = pmesh->GetNumVertices();
	LPBYTE pVtx;
	D3DXVECTOR3 vtx;
	pmesh->LockVertexBuffer(0, (LPVOID*)&pVtx);
	for (DWORD i = 0; i < dwVtx; ++i, pVtx += dwStride)
	{
		D3DXVec3TransformCoord(&vtx, (LPD3DXVECTOR3)pVtx, &matrix);
		if (hierarchyMesh.back()->fCollisionRadus < 0.0f)
		{
			D3DXVECTOR3& vMax = hierarchyMesh.back()->CollitionBox;
			D3DXVECTOR3& vMin = hierarchyMesh.back()->CenterPos;
			if (vMax.x < vtx.x) { vMax.x = vtx.x; }
			if (vMax.y < vtx.y) { vMax.y = vtx.y; }
			if (vMax.z < vtx.z) { vMax.z = vtx.z; }
			if (vMin.x > vtx.x) { vMin.x = vtx.x; }
			if (vMin.y > vtx.y) { vMin.y = vtx.y; }
			if (vMin.z > vtx.z) { vMin.z = vtx.z; }
		}
		else
		{
			float fRadius = D3DXVec3Length(&(vtx - hierarchyMesh.back()->CenterPos));
			if (hierarchyMesh.back()->fCollisionRadus < fRadius)
			{
				hierarchyMesh.back()->fCollisionRadus = fRadius;
			}
		}
	}
	pmesh->UnlockVertexBuffer();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// メッシュ情報がすでにあるか検索
// 生成済みなら元のデータを使用
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool ResourceManager::checkExisting(CHAR *pszChakNeme,MeshData &meshDataPtr)
{
	// 全メッシュを検索
	for (UINT i = 0; i < meshes.size(); i++)
	{
		if (strcmp(meshes[i]->meshFileName, pszChakNeme) == 0)
		{
			meshDataPtr.attrPtr = (meshes[i]->attrPtr);
			meshDataPtr.centerPos = meshes[i]->centerPos;
			meshDataPtr.collisionRadus = meshes[i]->collisionRadus;
			meshDataPtr.collitionBox = meshes[i]->collitionBox;
			meshDataPtr.dwAttrNum = meshes[i]->dwAttrNum;
			meshDataPtr.dwNumIndx = meshes[i]->dwNumIndx;
			meshDataPtr.dwNumTriangles = meshes[i]->dwNumTriangles;
			meshDataPtr.dwNumVtx = meshes[i]->dwNumVtx;
			meshDataPtr.indexPtr = (meshes[i]->indexPtr);
			meshDataPtr.materialBufferPtr = meshes[i]->materialBufferPtr;
			meshDataPtr.maxVtx = meshes[i]->maxVtx;
			strcpy_s(meshDataPtr.meshFileName, meshes[i]->meshFileName);
			meshDataPtr.meshPtr = meshes[i]->meshPtr.Get();
			meshDataPtr.minVtx = meshes[i]->minVtx;
			meshDataPtr.numMat = meshes[i]->numMat;
			meshDataPtr.vertexPtr = (meshes[i]->vertexPtr);


			return true;
		}
	}
	return false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// テクスチャ情報がすでにあるか検索
// 生成済みなら元のデータを使用
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool ResourceManager::checkExisting(CHAR *pszChakNeme, TEXTURE_DATA &TextureData)
{
	// 全テクスチャを検索
	for (UINT i = 0; i < texture.size(); i++)
	{
		if (strcmp(texture[i]->texFileName, pszChakNeme) == 0)
		{
			TextureData = *texture[i];	// 生成済みなら元のデータを使用
			return true;
		}
	}
	return false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// テクスチャ情報がすでにあるか検索
// 生成済みなら元のデータを使用
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool ResourceManager::checkExisting(CHAR *pszChakNeme, VERTEX_BOARD_DATA &TextureData)
{
	// 全テクスチャを検索
	for (UINT i = 0; i < vtxBoard.size(); i++)
	{
		if (strcmp(vtxBoard[i]->name, pszChakNeme) == 0)
		{
			TextureData = *vtxBoard[i];	// 生成済みなら元のデータを使用
			return true;
		}
	}
	return false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// メッシュ解放
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool ResourceManager::destroyMesh(CHAR *pszChakNeme)
{
	
	for (INT i = (INT)meshes.size() - 1; i >= 0; i--)
	{
		if (strcmp(meshes[i]->meshFileName, pszChakNeme) == 0)
		{
			delete meshes[i]->attrPtr;
			delete meshes[i]->indexPtr;
			delete meshes[i]->vertexPtr;
			meshes.erase(meshes.begin() + i);
			return true;
		}
	
	}
	
	return false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 全メッシュ解放
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool ResourceManager::destroyAllMesh()
{
	
	// 例外処理
	if (meshes.size() == 0)
		return true;

	for (INT i = 0; i < (INT)meshes.size(); i++)
	{
		delete meshes[i]->attrPtr;
		delete meshes[i]->indexPtr;
		delete meshes[i]->vertexPtr;

		meshes[i]->attrPtr  = nullptr;
		meshes[i]->indexPtr = nullptr;
		meshes[i]->vertexPtr = nullptr;
	}

	meshes.clear();
	
	return true;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 階層構造用モデル解放
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool ResourceManager::destroyHierarchymesh(CHAR *pszChakNeme,std::string keyName)
{
	
	for (auto mesh : hierarchyMesh)
	{

		// アニメーション解放
		if (hierarchyMesh.back()->ppAnimSet)
		{
			for (UINT j = 0; j < hierarchyMesh.back()->uNumAnimSet; j++)
			{
				hierarchyMesh.back()->ppAnimSet[j]->Release();
				hierarchyMesh.back()->ppAnimSet[j] = nullptr;
			}
		}
		else
		{
			return false;
		}

		hierarchyMesh.back()->pAnimCtrl->Release();
		hierarchyMesh.back()->pAnimCtrl = nullptr;

			// メッシュ解放
		if (hierarchyMesh.back()->pFrameRoot)
		{
			D3DXFrameDestroy(hierarchyMesh.back()->pFrameRoot, &hierarchyMesh.back()->Hierarchy);
			hierarchyMesh.back()->pFrameRoot = nullptr;

			if (hierarchyMesh.empty())
			{
				hierarchyMesh.clear();
			}
			else
			{
				hierarchyMesh.pop_back();
			}

			return true;
		}
		else
		{
			return false;
		}
	}

	
	return false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 全階層構造用メッシュを解放
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool ResourceManager::destroyAllHierarchymesh()
{
	for (INT i = hierarchyMesh.size() - 1; i >= 0; i--)
	{
		// アニメーション解放
		if (hierarchyMesh[i]->ppAnimSet)
		{
			for (UINT j = 0; j < hierarchyMesh[i]->uNumAnimSet; j++)
			{
				SAFE_RELEASE(hierarchyMesh[i]->ppAnimSet[j]);
			}
			SAFE_DELETE_ARRAY(hierarchyMesh[i]->ppAnimSet);
		}
		SAFE_RELEASE(hierarchyMesh[i]->pAnimCtrl);

		// メッシュ解放
		if (hierarchyMesh[i]->pFrameRoot)
		{
			D3DXFrameDestroy(hierarchyMesh[i]->pFrameRoot, &hierarchyMesh[i]->Hierarchy);
			hierarchyMesh[i]->pFrameRoot = nullptr;
		}

	}
	hierarchyMesh.clear();
	return true;
	

	return true;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// テクスチャ解放
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool ResourceManager::destroyAllTexture()
{
	/*
	// 例外処理
	if (texture.size() <= 0)
		return false;

	for (UINT i = 0; i < texture.size(); i++)
	{
		SAFE_RELEASE(texture[i]->pD3DTexture);
		texture.erase(texture.begin() + i);
	}

	texture.clear();

	*/
	return true;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 頂点情報解放(フェード用以外)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool ResourceManager::destroyVtx()
{
	/*
	// 例外処理
	if (vtxBoard.size() <= 0)
		return false;

	// 頂点情報解放
	for (UINT i = 0; i < vtxBoard.size(); i++)
		SAFE_RELEASE(vtxBoard[i]->pD3DVtxBuff);

	vtxBoard.clear();
	
	*/
	return true;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フェード用頂点情報解放
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool ResourceManager::destroyFadeVtx()
{
	/*
	// 例外処理
	if (vtxFadeBoard.size() <= 0)
		return false;

	// 頂点情報解放
	for (UINT i = 0; i < vtxFadeBoard.size(); i++)
		SAFE_RELEASE(vtxFadeBoard[i]->pD3DVtxBuff);
	vtxFadeBoard.clear();

	*/
	return true;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// テクスチャ作成(フェード)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void ResourceManager::createFadeTexture(TEXTURE_DATA& TextureData, CHAR *pszFilename)
{
	// ファイル名セット
	strcpy_s(TextureData.texFileName, pszFilename);

	fadeTexture.push_back(&TextureData);

	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// テクスチャの読み込み
	if (D3DXCreateTextureFromFile(devicePtr, fadeTexture.back()->texFileName, &fadeTexture.back()->pD3DTexture))
		return;
}

void  ResourceManager::createNormalTexture(TEXTURE_DATA& TextureData, CHAR *pszFilename)
{

	// 既に生成されているか
	if (checkExisting(pszFilename, TextureData))
		return;

	// ファイル名セット
	strcpy_s(TextureData.texFileName, pszFilename);

	texture.push_back(&TextureData);

	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// テクスチャの読み込み
	if (D3DXCreateTextureFromFile(devicePtr, texture.back()->texFileName, &texture.back()->pD3DTexture))
		return ;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// インスタンス生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool ResourceManager::createInstance()
{
	if (instancePtr.get() == nullptr)
	{
		instancePtr.reset(NEW ResourceManager());

		return true;
	}
	else
	{
		return false;
	}
}