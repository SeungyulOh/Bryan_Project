#ifndef Snow_h__
#define Snow_h__

#include "Defines.h"
#include "ParticleSystem.h"

class CSnow : public Engine::CParticleSystem
{
public:
	CSnow(LPDIRECT3DDEVICE9 pGraphicDev,const _tchar* pFilePath,_vec3 vMin,_vec3 vMax,int iNumParticles);
	virtual ~CSnow();

	void ResetParticle(Engine::PARICLEATT* pAttribute);
	void Update(float fTimeDelta);
};

#endif // Snow_h__
