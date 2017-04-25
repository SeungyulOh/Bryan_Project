#ifndef Export_Engine_h__
#define Export_Engine_h__

#include "Export_System.h"
#include "Export_Utility.h"

namespace Engine
{
	enum VECTYPE {VEC_RIGHT, VEC_UP, VEC_LOOK, VEC_POSITION};
	inline _vec3 MyGetCamPosition(LPDIRECT3DDEVICE9 pGraphicDev, VECTYPE eType);
	inline _vec3 MyGetVec3FromMat(VECTYPE eType, _matrix InputMat);
	inline _matrix MyGetNewMatExcScale(_matrix matIn);
	inline _matrix MyGetRotMatFromWorldMat(_matrix matWorld);
	inline _matrix MyGetTransMatFromWorldMat(_matrix matWorld);
	inline _matrix MyGetNewMatExcRot(_matrix matIn);

#include "Export_MyFunction.inl"
}

#endif // Export_Engine_h__
