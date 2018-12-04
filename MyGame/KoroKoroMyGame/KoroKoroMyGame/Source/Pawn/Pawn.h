//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Pawn.h
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef PAWN_H
#define PAWN_H

// ===== インクルード部 =====
#include "d3dx9.h"
#include "../GameObjectBase/GameObjectBase.h"
#include "../Mesh/Mesh.h"
#include "../MyHierarchy/MyHierarchy.h"
#include "../ResoueceManager/ResourceManager.h"
#include "../Transform/Transform.h"
#include <vector>
#include <memory>

enum class MeshObjType
{
	NormalModel,
	HierarchyModel,	
};

// ===== クラスの前方宣言 =====
class Collider;

// ===== クラス定義 =====
class Pawn : public GameObjectBase
{
public:
	Pawn();
	Pawn(UINT);
	virtual ~Pawn();

	// 識別用列挙体定義
	enum class TagType
	{
		Player = 0,
		Enemy,
	};

	virtual void initialize();
	virtual void initialize(std::string fileName);
	virtual void update();
	virtual void updateAnimation();
	virtual void draw();
	virtual void draw(D3DXMATRIX,D3DXMATRIX);
	virtual void draw(LPD3DXMESH pMesh, LPDIRECT3DTEXTURE9 pTex, LPD3DXBUFFER pBuff, DWORD numMat);
	virtual void finalize();

	virtual void initializeStatus();	// 各種ステータスを初期値にする

	void drawObjectLocal();

	// ゲッター
	D3DXMATRIX*			getWorldMtx();			// ワールド行列取得
	D3DXVECTOR3			getUpVec();				// 上方向のベクトル取得
	D3DXVECTOR3			getForwardVec();		// 前方向のベクトル取得
	D3DXVECTOR3			getRightVec();			// 横方向のベクトル取得
	D3DXVECTOR3			getPosition();			// 位置取得
	D3DXVECTOR3			getRotation();			// 回転取得
	D3DXVECTOR3			getvelocity();				// 移動スピード取得
	D3DXVECTOR3			getOffset();			// ワールド行列からオフセット値を取得
	D3DXVECTOR3			getCenterPos();			// 中心座標取得
	D3DXVECTOR3			getCollisionBox();		// あたり判定用サイズ取得
	D3DXVECTOR3			getColliderPos();		// あたり判定用位置取得		
	FLOAT				getCollisionRadius();	// あたり判定用半径取得
	TagType				getTag();				// タグ取得
	DWORD				getVertexNum();			// 頂点の数取得
	DWORD				getIndxNum();			// インデックスバッファの数取得
	DWORD				getTriangleNum();		// 三角形の数取得
	MESH_VTX*			getVtxAcess();			// 頂点情報アクセス用ポインタ取得		
	WORD*				getIndxAcess();			// インデックスバッファアクセス用ポインタ取得
	bool				getIsGround();			// 地面上状態取得
	bool				isUsedShader();			// シェーダー使用

	LPD3DXMESH	 getMesh();
	LPD3DXBUFFER getMat();
	DWORD		 getMatNum();

	LPDIRECT3DVERTEXBUFFER9 getVtxBuff();		// 頂点情報取得
	LPDIRECT3DTEXTURE9		getTexture();		// テクスチャ取得
	BOOL					getUsedFlg();		// 使用フラグ取得

	HRESULT AllocBoneMatrix(LPD3DXMESHCONTAINER);
	HRESULT AllocAllBoneMatrix(LPD3DXFRAME);

	void setTime(DOUBLE);
	void drawFrame(LPD3DXFRAME);
	void RenderMeshContainer(LPD3DXMESHCONTAINER, LPD3DXFRAME );

	// セッター
	void setMesh(LPD3DXMESH, LPD3DXBUFFER, DWORD);
	void setMaterialcolorEmissive(float, float, float, float);	// マテリアルの自己発光値セット
	void setUsedFlg(bool);										// 使用フラグセット
	void setvelocitySpeed(D3DXVECTOR3);								// 移動量セット
	void setOffset(D3DXVECTOR3 setOffset);						// 行列上の位置セット
	void setNumber(UINT);										// 識別番号セット
	void setPosition(D3DXVECTOR3);								// 位置セット
	void setTranslate(D3DXVECTOR3);								// 位置セット
	void setRotation(D3DXVECTOR3);								// 回転セット
	void setHitFlg(bool );										// 衝突判定セット
	void setIsGround(bool );									// 地面上判定セット
	void setRayToTriangleInfo(D3DXVECTOR3 , D3DXVECTOR3 );		// レイと三角形のあたり判定用値セット
	void setDefaultValue();										// 初期値セット
	void setDestLanding(D3DXVECTOR3 setLanding);				// 着地位置セット
	void setHitIndex(INT);										// 当たったオブジェクトのインデックスをセット

	void setAnimChange(UINT, UINT);				// アニメーション切り替え

	void updateTransformData();
	void setTransformData(TransformData);
	void setHitData(D3DXVECTOR3 cross, D3DXVECTOR3 normal, D3DXVECTOR3 length, D3DXVECTOR3 destVec);
	const TransformData& getTransformData();
protected:
	std::unique_ptr <Collider> colliderPtr = nullptr;

	MeshData				meshDataObj;
	HIERARCHY_MESH_DATA		hierarchyMeshData;
	TEXTURE_DATA			textureData;

	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuffPawn;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9  pD3DIdxBuffPawn;	// インデックスバッファインターフェースへのポインタ

	LPDIRECT3DTEXTURE9	pD3DTexture;		// テクスチャへのポインタ
	LPD3DXMESH			meshPtr;			// メッシュ情報へのポインタ
	LPD3DXBUFFER		materialBufferPtr;		// マテリアル情報へのポインタ
	DWORD				numMat;				// マテリアル情報の数
	
	D3DXMATRIX			worldMtx;			// ワールドマトリックス
	D3DXMATRIX			translateMtx;		// 平行移動用ワールドマトリックス
	D3DXMATRIX			rotMtx;				// 回転マトリックス

	CHAR				texFileName[256];	// 読み込むファイル名

	D3DXVECTOR3			pos;					// 現在の位置
	D3DXVECTOR3			velocity;				// 移動量
	D3DXVECTOR3			accele;					// 加速度
	D3DXVECTOR3			rot;					// 現在の向き
	D3DXVECTOR3			rotDest;				// 目的の向き
	D3DXVECTOR3			scale;					// 拡大率
	D3DXVECTOR3			destLanding;			// 目的着地位置
	D3DXVECTOR3			maxVtx;					// 最大頂点位置
	D3DXVECTOR3			minVtx;					// 最小頂点位置
	D3DXVECTOR3			centerPos;				// モデルの中心座標
	D3DXVECTOR3			collitionBox;			// あたり判定用サイズ
	D3DXVECTOR3			collisionSize;			// あたり判定用の大きさ

	D3DXVECTOR3			defPos;					// 初期値の位置
	D3DXVECTOR3			defvelocity;				// 初期値の移動量
	D3DXVECTOR3			defrot;					// 初期値の向き
	D3DXVECTOR3			defScale;				// 初期値の拡大率

	D3DXQUATERNION		quatanion;				// 任意軸の回転用クォータニオン
	D3DXQUATERNION		destQua;				// 目的クオータニオン



	CHAR				fileName[256];			// モデルのファイル名
	FLOAT				speed;					// 速度
	FLOAT				radRot;					// 回転角度
	FLOAT				oldRadRot;				// 前の回転角度
	FLOAT				collisionRadus;			// あたり判定用サイズ
	bool				isShader;				// シェーダー使用
	bool				isHit;					// 当たっているかどうか
	bool				isGround;				// 地面の上か
	bool				isUsed;					// 使用中か

	TagType				tagName;				// 識別用タグ
	UINT				idNumber;				// 複数体識別用添え字番号

	D3DXVECTOR3			cross;					// Rayと三角形の判定用ヒット位置
	D3DXVECTOR3			normal;					// Rayと三角形の判定用法線ベクトル


	D3DXVECTOR3			startPos;				// 開始位置 (補間用)
	D3DXVECTOR3			endPos;					// 終了位置 (補間用)
	INT					hitIndex;				// 衝突判定用

	bool				endAnim;				// アニメーションが終了したか
	DOUBLE				maxTrackTime;			// アニメーションのトラック最大時間

	UINT				curSelectAnim;			// 現在選択中のアニメーション番号

	// 階層構造用
	LPD3DXFRAME					frameRoot;		// ルート フレーム オブジェクト
	LPD3DXANIMATIONCONTROLLER	animCtrlPtr;	// アニメーション コントローラ オブジェクト
	UINT						numAnimset;		// アニメーション セット数
	LPD3DXANIMATIONSET*			ppAnimSet;		// アニメーション セット
	MyHierarchy					Hierarchy;		// 階層メモリ確保/解放クラス
	DWORD						dwPrev;			// 直前の時刻
	
	TransformData				myTransformData;		// 書き出す際の保存用データ
	GameObjType					objType;
	MeshObjType					meshType;				// メッシュの種類
	UINT						currentAnim;			// アニメーション
private:

#define FVF_TVERTEX	(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

	void updateFrameMatrices(LPD3DXFRAME, LPD3DXMATRIX);

	void calcCollision(LPD3DXFRAME);			// 階層構造用、境界球/境界ボックス取得
	void calcCollisionFrame(LPD3DXFRAME);
	void calcCollisionMeshContainer(LPD3DXMESHCONTAINER , LPD3DXFRAME );

	D3DXMATRIX			mtxInverse;				// 打消し用逆行列
	D3DXMATRIX			mtxOldTranslate;		// 1フレーム前の平行移動

	// 衝突判定用
	DWORD				dwNumVtx;				// 頂点数
	DWORD				dwNumTriangles;			// 三角形の数 (頂点 * 3)
	DWORD				dwNumIndx;				// インデックスバッファの数
	DWORD				dwAttrNum;				// 属性値

	FLOAT				colorAlpha;				// アルファ値変更用変数
};

#endif