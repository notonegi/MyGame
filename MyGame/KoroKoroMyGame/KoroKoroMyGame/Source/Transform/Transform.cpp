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
const TransformData* Transform::getData(INT index) const
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