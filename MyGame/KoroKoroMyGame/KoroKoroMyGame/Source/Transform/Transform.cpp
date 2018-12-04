//
// Transform.h
// Author : Masaya Hayashi
//

// ===== CN[h =====
#include "Transform.h"
#include "../DirectX3D/DirectX3D.h"
#include <memory>

//
// RXgN^
//
Transform::Transform()
{

}

//
// fXgN^
//
Transform::~Transform()
{

}

//
// ¶¬
//
void Transform::create()
{

}

//
// f[^æ¾
//
TransformData* Transform::getData(INT index) const
{
	if (transformList.empty())
	{
		throw std::runtime_error("transform is empty");
	}

	for (auto transform : transformList)
	{
		if (transform->idNumber == index)
		{
			return transform;
		}
	}

	throw std::runtime_error("index do not match");
}

//
// f[^Zbg
//
void Transform::setData(TransformData *setData)
{
	transformList.push_back(setData);

	setData->idNumber = transformList.size() - 1;	// IDip\Í0X^[gÅê
}

//
//  ½è»è
//
void Transform::setRayHit(INT index, D3DXVECTOR3 setCross, D3DXVECTOR3 setNormal, D3DXVECTOR3 setLength, D3DXVECTOR3 setDestvec)
{
	TransformData *hitDataPtr = getData(index);

	hitDataPtr->cross	= setCross;
	hitDataPtr->normal  = setNormal;
	hitDataPtr->length  = setLength;
	hitDataPtr->destvec = setDestvec;
}

//
// tOZbg
//
void Transform::setHit(INT index,bool setFlg)
{
	for (auto transform : transformList)
	{
		if (transform->idNumber == index)
		{
			transform->isHit = setFlg;
		}
	}
}

//
// NA
//
void Transform::clear()
{
	transformList.clear();
}