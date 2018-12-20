//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Collision.cpp
//
// 説明：
// 判定を取るための更新処理などをまとめたクラス
// colliderクラスとは別
//
// Auhotor : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "Collision.h"
#include "../Player/Player.h"
#include "../Pawn/Pawn.h"
#include "../DirectX3D/DirectX3D.h"
#include "../SceneManager/SceneManager.h"
#include "../MyVector3/MyVector3.h"
#include "../MyDelete/MyDelete.h"

// ===== 静的メンバ =====
std::unordered_map<std::string, std::list<Transform*>> Collision::collisionMapes;
std::unordered_map<std::string, std::vector<RayHit*>>  Collision::rayHitMapes;

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Collision::Collision()
{
	
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Collision::Collision(Pawn* setPlayerPtr,Pawn* setFieldPtr)
{
	playersPtr.push_back(setPlayerPtr);
	fieldPtr  = setFieldPtr;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Collision::~Collision()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Collision::update()
{
	D3DXVECTOR3 cross, normal, length;

	UINT playerIndex = 0;

	for (auto player : playersPtr)
	{
		if (checkCollisionField(player, player, fieldPtr, cross, normal, length, D3DXVECTOR3(0.0f, -0.1f, 0.0f)))
		{
			if (MyVector3::getLength(length) < 1.02f)
			{
				DirectX3D::printDebug("あたっています");
			}

			rayHitMapes["Player"][playerIndex]->isHit = true;

		}
		else
		{
			rayHitMapes["Player"][playerIndex]->isHit = false;
		}

		playerIndex ++;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 登録
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Collision::registerList(Transform *setPawn,std::string keyName)
{
	collisionMapes[keyName].push_back(setPawn);
	rayHitMapes[keyName].push_back(NEW RayHit());
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 後処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Collision::finalize(std::string keyName)
{
	/*
	for (UINT playerCnt = 0; playerCnt < rayHitMapes["Player"].size(); playerCnt++)
	{
		Mydelete::safeDelete(rayHitMapes["Player"].back());
		rayHitMapes["Player"].pop_back();
	}

	for (UINT fieldCnt = 0; fieldCnt < rayHitMapes["field"].size(); fieldCnt++)
	{
		delete rayHitMapes["field"].back();
		rayHitMapes["field"].pop_back();
	}
	*/
	
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 地面とプレイヤーの衝突処理更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
UINT Collision::checkCollisionField(Pawn *pPlayer, Pawn *pPawnB, Pawn *pField, D3DXVECTOR3 &Cross, D3DXVECTOR3 &Normal, D3DXVECTOR3 &fLength, D3DXVECTOR3 DestVec)
{
	INT	nIndx;

	// 線分と三角形の判定
	nIndx = isHitRayToMesh(pField, pPlayer, &pPlayer->getPosition(), &(pPlayer->getPosition() + DestVec), true, &Cross, &Normal, &fLength);

	if (nIndx >= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 球と球のあたり判定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Collision::IsHitSphereToSphere(Pawn *pPawnA,Pawn *pPawnB)
{
	D3DXVECTOR3 SphereA = pPawnA->getOffset();
	D3DXVECTOR3 SphereB = pPawnB->getPosition();
	FLOAT fRadA = pPawnA->getCollisionRadius() * 1.5f;
	FLOAT fRadB = pPawnB->getCollisionRadius() * 1.5f;

	if (   (SphereA.x - SphereB.x) * (SphereA.x - SphereB.x)
		+  (SphereA.y - SphereB.y) * (SphereA.y - SphereB.y)
		+  (SphereA.z - SphereB.z) * (SphereA.z - SphereB.z)
		<= (fRadA + fRadB) * (fRadA + fRadB))
	{
		return true;
	}
	else
	{
		return false;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// プレイヤーセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Collision::setPlayer(Pawn* playerPtr)
{
	playersPtr.push_back(playerPtr);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// レイと三角形のあたり判定
// 引数 : 
//		  pPawnA  : フィールドのメッシュ情報
//		  pPawnB  : プレイヤーのメッシュ情報
//		  pCross  : 当たった座標 
//		  pNormal : 法線ベクトル
//		  fLength : 位置からの長さ
// 戻り値:
//		  当たっている		⇒ 当たっている三角形の添え字
//		  当たっていない	⇒ -1
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
INT Collision::isHitRayToMesh(Pawn *pPawnA, Pawn *pPawnB, LPD3DXVECTOR3 pRayPos, LPD3DXVECTOR3 pRayDir, bool bSegment, LPD3DXVECTOR3 pCross, LPD3DXVECTOR3 pNormal, LPD3DXVECTOR3 Length)
{
	// ワールドマトリックスの逆マトリックスを生成
	D3DXMATRIX mInvWorld;

	D3DXMATRIX test = *pPawnA->getWorldMtx();

	D3DXMatrixInverse(&mInvWorld, NULL, pPawnA->getWorldMtx());
	// レイを逆ワールド変換
	D3DXVECTOR3 vRayPos, vRayDir;
	D3DXVec3TransformCoord(&vRayPos, pRayPos, &mInvWorld);

	if (bSegment)
	{
		D3DXVec3TransformCoord(&vRayDir, pRayDir, &mInvWorld);
	}
	else
	{
		D3DXVec3TransformNormal(&vRayDir, pRayDir, &mInvWorld);
	}

	// レイとメッシュの交差判定
	INT nIndex = Intersect(pPawnA, &vRayPos, &vRayDir, bSegment, pCross, pNormal, Length);

	if (nIndex >= 0) // 交差している場合
	{
		// 交点、法線をワールド変換
		if (pCross)
		{
			D3DXVec3TransformCoord(pCross, pCross, pPawnA->getWorldMtx());
		}
		if (pNormal)
		{
			D3DXVec3TransformNormal(pNormal, pNormal, pPawnA->getWorldMtx());
		}
	}
	return nIndex;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 線分の判定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
INT Collision::Intersect(Pawn *pField, LPD3DXVECTOR3 pRayPos, LPD3DXVECTOR3 pRayDir, bool bSegment, LPD3DXVECTOR3 pCross, LPD3DXVECTOR3 pNormal, LPD3DXVECTOR3 pFLength)
{
	if (!pRayPos || !pRayDir)
	{
		return -1;
	}

	// レイ取得
	D3DXVECTOR3& P0 = *pRayPos;
	D3DXVECTOR3 W = *pRayDir;

	if (bSegment)
	{
		W -= P0;
	}

	DWORD dwNumIndx = pField->getIndxNum();			// 三角形の数取得	
	MESH_VTX *pVtx = pField->getVtxAcess();		// 頂点情報取得
	WORD	 *pIndx = pField->getIndxAcess();		// インデックス情報取得

													// 全三角形を探索
	for (DWORD i = 0; i < dwNumIndx; i += 3)
	{
		D3DXVECTOR3 P1(pVtx[pIndx[i + 0]].VtxPos);
		D3DXVECTOR3 P2(pVtx[pIndx[i + 1]].VtxPos);
		D3DXVECTOR3 P3(pVtx[pIndx[i + 2]].VtxPos);


		// 辺を示すベクトルを取得
		D3DXVECTOR3 V1(P2 - P1);
		D3DXVECTOR3 V2(P3 - P2);

		// 例外処理
		if (V1.y >= pRayDir->y &&
			V2.y >= pRayDir->y)
		{
			continue;
		}

		// 法線ベクトルを取得
		D3DXVECTOR3 N;
		D3DXVec3Normalize(&N, D3DXVec3Cross(&N, &V1, &V2));
		// 分母を算出
		FLOAT deno = D3DXVec3Dot(&N, &W);
		if (deno >= 0.0f)
		{
			continue;	// 平行(==0)か裏から表(>0)
		}
						// 内外判定
		D3DXVECTOR3 N1;
		D3DXVec3Cross(&N1, &V1, &W);
		if (D3DXVec3Dot(&N1, &(P0 - P1)) < 0.0f)
		{
			continue;
		}

		D3DXVec3Cross(&N1, &V2, &W);

		if (D3DXVec3Dot(&N1, &(P0 - P2)) < 0.0f)
		{
			continue;
		}

		D3DXVECTOR3 V3(P1 - P3);
		D3DXVec3Cross(&N1, &V3, &W);

		if (D3DXVec3Dot(&N1, &(P0 - P3)) < 0.0f)
		{
			continue;
		}

		// 媒介変数算出
		float T = D3DXVec3Dot(&N, &(P1 - P0)) / deno;

		pFLength->x = T;

		// 交点を算出
		D3DXVECTOR3 X = P0 + T * W;
		if (pCross)
		{
			*pCross = X;
		}

		// 法線を返す
		if (pNormal)
		{
			*pNormal = N;
		}
		// 見つかったので三角形の番号を返す

		return i / 3;
	}
	// 見つからなかったので負の値を返す
	return -1;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 線分の判定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Collision::IntersectA(Pawn* pField,LPD3DXVECTOR3 pRayPos, LPD3DXVECTOR3 pRayDir, LPD3DXVECTOR3 pCross, LPD3DXVECTOR3 pNormal, LPD3DXMATRIX pWorld)
{
	bool ans = false;

	D3DXVECTOR3 W;
	D3DXVECTOR3 H;
	D3DXMATRIX matInv;
	if (pWorld) 
	{
		D3DXMatrixInverse(&matInv, nullptr, pWorld);
		D3DXVec3TransformCoord(&W, pRayPos, &matInv);
		D3DXVec3TransformNormal(&H, pRayDir, &matInv);
	}
	else 
	{
		W = *pRayPos;
		H = *pRayDir;
	}

	LPDIRECT3DVERTEXBUFFER9	lpVertexBuffer;	// 頂点バッファ
	pField->getMesh()->GetVertexBuffer(&lpVertexBuffer);	// 頂点バッファオブジェクトへのポインタをゲット
	LPDIRECT3DINDEXBUFFER9	lpIndexBuffer;	// インデックスバッファ
	pField->getMesh()->GetIndexBuffer(&lpIndexBuffer);		// インデックスバッファオブジェクトへのポインタをゲット
	DWORD dwIdx = pField->getMesh()->GetNumFaces() * 3;		// 面数からインデックス数をゲット

	MESH_VTX* pVtx;
	WORD* pIdx;
	lpVertexBuffer->Lock(0, 0, (void**)&pVtx, 0);	// 頂点バッファをロック
	lpIndexBuffer->Lock(0, 0, (void**)&pIdx, 0);	// インデックスバッファをロック

	D3DXVECTOR3* P[4];
	D3DXVECTOR3 N;
	for (DWORD i = 0; i < dwIdx; ) 
	{
		// 三角形の頂点を抽出
		P[0] = &pVtx[pIdx[i++]].VtxPos;
		P[1] = &pVtx[pIdx[i++]].VtxPos;
		P[2] = &pVtx[pIdx[i++]].VtxPos;
		P[3] = P[0];
		INT j = 0;
		for (; j < 3; j++) {
			// 三角形を囲む平面の法線ベクトルを求める
			D3DXVec3Cross(&N, &H, &(*(P[j + 1]) - *(P[j])));
			// 始点が平面の表なら当たっていない
			FLOAT dot = D3DXVec3Dot(&N, &(W - *(P[j])));

			if (dot > 0.0f) 
			{
				break;
			}
		}
		if (j < 3) 
		{
			continue;
		}

		// 三角形の法線ベクトルを求める
		D3DXVec3Cross(&N, &(*(P[1]) - *(P[0])), &(*(P[2]) - *(P[1])));
		D3DXVec3Normalize(&N, &N);

		// 媒介変数tの分母を求める
		float base = D3DXVec3Dot(&N, &H);
		if (base == 0.0f)
		{
			continue;		// 平面と平行
		}

		// tを求める
		FLOAT t = D3DXVec3Dot(&N, &(*(P[0]) - W)) / base;

	//	PrintDebugProc("aaaaaaa:%f\n", t);

		// 交点がレイの後ろ
		if (t < 0.0f)
		{
			continue;
		}
		// 交点がレイの前
		if (t > 1.0f)
		{
			continue;
		}

		// 交点を求める
		D3DXVECTOR3 X = W + t * H;
		if (pCross) 
		{
			if (pWorld)
			{
				D3DXVec3TransformCoord(pCross, &X, pWorld);
			}
			else 
			{
				*pCross = X;
			}
		}
		if (pNormal)
		{
			if (pWorld) 
			{
				D3DXVec3TransformNormal(pNormal, &N, pWorld);
			}
			else
			{
				*pNormal = N;
			}
		}
		ans = true;
		break;
	}

	lpVertexBuffer->Unlock();		// 頂点バッファをアンロック
	lpIndexBuffer->Unlock();			// インデックスバッファをアンロック
//	SAFE_RELEASE(lpVertexBuffer);	// 頂点バッファオブジェクトを解放
//	SAFE_RELEASE(lpIndexBuffer);		// インデックスバッファオブジェクトを解放

	return ans;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
const Transform* Collision::getTransform(std::string keyName,INT index)
{
	if (index < 0)
	{
		throw std::invalid_argument("不正な引数です");

		return nullptr;
	}

	return *std::next(collisionMapes[keyName].begin(), index);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
const std::list<Transform*> Collision::getTransform(std::string keyName)
{
	if (collisionMapes[keyName].empty())
	{
		throw std::invalid_argument("不正な引数です");

	}
	return collisionMapes[keyName];
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// レイの判定取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
const RayHit* Collision::getRayHitData(std::string keyName,UINT index)
{
	if (index < 0)
	{
		throw std::underflow_error("引数の値が小さすぎます");
		return nullptr;
	}

	return *std::next(rayHitMapes[keyName].begin(), index);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 加速度セット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Collision::setVelocity(std::string keyName, UINT index, D3DXVECTOR3 setVelocity)
{
	UINT indexCnt = 0;

	for (auto collisionMap : collisionMapes[keyName])
	{
		if (indexCnt == index)
		{
			collisionMap->velocity = setVelocity;
		}

		indexCnt++;
	}
}