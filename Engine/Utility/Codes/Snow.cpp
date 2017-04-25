#include "stdafx.h"
#include "Snow.h"

CSnow::CSnow(LPDIRECT3DDEVICE9 pGraphicDev,const _tchar* pFilePath,_vec3 vMin,_vec3 vMax, int iNumParticles)
{

	m_vBoundingMinMax[0] = vMin;
	m_vBoundingMinMax[1] = vMax;

	m_fSize = 1.5f;
	m_dwVTXSize = 2048;
	m_dwVTXOffSet = 0;
	m_dwBatchSize = 512;

	CParticleSystem::Init(pGraphicDev,pFilePath);

	for(int i =0; i<iNumParticles ; ++i)
		AddParticle();
}


CSnow::~CSnow()
{

}

void CSnow::ResetParticle(Engine::PARICLEATT* pAttribute)
{
	pAttribute->isAlive = true;

	CParticleSystem::GetRandomVector(&pAttribute->vPos,&m_vBoundingMinMax[0],&m_vBoundingMinMax[1]);

	pAttribute->vPos.y = m_vBoundingMinMax[1].y;

	pAttribute->vVelocity.x = CParticleSystem::GetRandomFloat(0.f,1.f) * -3.f;
	pAttribute->vVelocity.y = CParticleSystem::GetRandomFloat(0.f,1.f) * -10.f;
	pAttribute->vVelocity.z = 0.f;

	//pAttribute->vColor = D3DXCOLOR(1.f,1.f,1.f,1.f);
}

void CSnow::Update(float fTimeDelta)
{
	list<Engine::PARICLEATT>::iterator iter = m_AttributeList.begin();
	list<Engine::PARICLEATT>::iterator iter_end = m_AttributeList.end();

	for( ;iter!=iter_end ; ++iter)
	{
		iter->vPos += iter->vVelocity * fTimeDelta;

		if(iter->vPos.y < m_vBoundingMinMax[0].y)
			ResetParticle(&(*iter));
	}
}

