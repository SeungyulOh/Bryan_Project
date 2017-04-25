#include "ParticleSystem.h"

Engine::CParticleSystem::CParticleSystem(LPDIRECT3DDEVICE9 pGraphicDev)
:Engine::CGameObject(pGraphicDev)
{
	m_iMaxTrail = 20;

}

Engine::CParticleSystem::~CParticleSystem()
{

}

bool Engine::CParticleSystem::Init(_uint iMaxNumParticle)
{

	m_iMaxParticles =iMaxNumParticle;
	if(FAILED(m_pGraphicDev->CreateVertexBuffer(m_dwVTXSize * sizeof(PARTICLE),D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY, D3DFVF_PARTICLE, D3DPOOL_DEFAULT, &m_pVB,NULL)))
		return false;

	m_pTrailList = new list<PARICLEATT>*[iMaxNumParticle];
	for(_uint i =0; i<iMaxNumParticle; ++i)
		m_pTrailList[i] = new list<PARICLEATT>;

	return true;
}

void Engine::CParticleSystem::Reset()
{
	list<PARICLEATT>::iterator iter = m_AttributeList.begin();
	list<PARICLEATT>::iterator iter_end = m_AttributeList.end();

	_uint iIndex = 0;
	for( ; iter != iter_end ; ++iter)
	{
		ResetParticle(&(*iter),iIndex);
		++iIndex;
	}
}

void Engine::CParticleSystem::AddParticle()
{
	PARICLEATT attribute;

	ResetParticle(&attribute,0);

	m_AttributeList.push_back(attribute);
}


void Engine::CParticleSystem::AddParticle(_uint iIndex)
{
	PARICLEATT attribute;

	ResetParticle(&attribute,iIndex);

	m_AttributeList.push_back(attribute);
}

void Engine::CParticleSystem::AddParticle(PARICLEATT Attribute, _uint iIndex)
{

	if(m_iMaxTrail > (*m_pTrailList[iIndex]).size())
		(*m_pTrailList[iIndex]).push_back(Attribute);
	else
	{
		(*m_pTrailList[iIndex]).pop_front();
		(*m_pTrailList[iIndex]).push_back(Attribute);
	}
	

	
}

void Engine::CParticleSystem::preRender()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING,false);
	//m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE,true);
	//m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE,true);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE,fToDW(m_fSize));
	//m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE_MIN,fToDW(10.f));
	//m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE_MAX,fToDW(1000.f));

	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_A,0);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_B,0);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_C,1);

	/*m_pGraphicDev->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_SELECTARG1);*/

	//m_pGraphicDev->SetRenderState(D3DRS_ZENABLE,false);

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	
}

void Engine::CParticleSystem::Render_GameObject()
{

	if(m_AttributeList.size() != 0)
	{
		preRender();

		m_pGraphicDev->SetFVF(D3DFVF_PARTICLE);
		m_pGraphicDev->SetStreamSource(0,m_pVB,0,sizeof(PARTICLE));
		//m_pGraphicDev->DrawPrimitive(D3DPT_POINTLIST,m_dwVTXOffSet,m_dwBatchSize);

		if(m_dwVTXOffSet >= m_dwVTXSize)
			m_dwVTXOffSet = 0;

		PARTICLE* vParticle = 0;
		DWORD numParticlesInBatch = 0;

		m_pVB->Lock(m_dwVTXOffSet * sizeof(PARTICLE), m_dwBatchSize * sizeof(PARTICLE), (void**)&vParticle, m_dwVTXOffSet ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

		list<PARICLEATT>::iterator iter = m_AttributeList.begin();
		list<PARICLEATT>::iterator iter_end = m_AttributeList.end();
		
		_uint iUpperIndex = 0;
		for (; iter!=iter_end ; ++iter)
		{
			list<PARICLEATT>::iterator iter_Trail = (*m_pTrailList[iUpperIndex]).begin();
			list<PARICLEATT>::iterator iter_Trail_end = (*m_pTrailList[iUpperIndex]).end();

			_uint iIndex = 0;
			for(; iter_Trail != iter_Trail_end ; ++iter_Trail)
			{
				if(iter_Trail->isAlive)
				{
					++iIndex;
					_float fRatioAlpha = _float(iIndex) / (*m_pTrailList[iUpperIndex]).size();

					vParticle->vPos = iter_Trail->vPos;
					D3DXCOLOR NewColor = iter_Trail->vColor;
					NewColor.a *= fRatioAlpha;
					vParticle->dwColor = (D3DCOLOR)NewColor;
					++vParticle;
					numParticlesInBatch++;

					if(numParticlesInBatch == m_dwBatchSize)
					{
						m_pVB->Unlock();

						m_pGraphicDev->DrawPrimitive(D3DPT_POINTLIST,m_dwVTXOffSet,m_dwBatchSize);

						m_dwVTXOffSet += m_dwBatchSize;

						if(m_dwVTXOffSet >= m_dwVTXSize)
							m_dwVTXOffSet = 0;

						m_pVB->Lock(m_dwVTXOffSet * sizeof(PARTICLE),m_dwBatchSize * sizeof(PARTICLE),(void**)&vParticle,m_dwVTXOffSet ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

						numParticlesInBatch = 0;
					}
				}
			}
			++iUpperIndex;

			

			if(iter->isAlive)
			{

				vParticle->vPos = iter->vPos;
				vParticle->dwColor = (D3DCOLOR)iter->vColor;
				++vParticle;
				numParticlesInBatch++;

				if(numParticlesInBatch == m_dwBatchSize)
				{
					m_pVB->Unlock();


					m_pGraphicDev->DrawPrimitive(D3DPT_POINTLIST,m_dwVTXOffSet,m_dwBatchSize);

					m_dwVTXOffSet += m_dwBatchSize;

					if(m_dwVTXOffSet >= m_dwVTXSize)
						m_dwVTXOffSet = 0;

					m_pVB->Lock(m_dwVTXOffSet * sizeof(PARTICLE),m_dwBatchSize * sizeof(PARTICLE),(void**)&vParticle,m_dwVTXOffSet ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);
					numParticlesInBatch = 0;
				}
			}
		}

		m_pVB->Unlock();

		//m_AttributeList.sort(Compare);


		if(numParticlesInBatch)
		{
			m_pGraphicDev->DrawPrimitive(D3DPT_POINTLIST,m_dwVTXOffSet,numParticlesInBatch);
		}

		m_dwVTXOffSet += m_dwBatchSize;


		postRender();
	}
}

void Engine::CParticleSystem::Render_GameObject(LPD3DXEFFECT pEffect, LPDIRECT3DTEXTURE9 pTexture)
{
	if(m_AttributeList.size() != 0)
	{
		preRender();

		m_pGraphicDev->SetFVF(D3DFVF_PARTICLE);
		m_pGraphicDev->SetStreamSource(0,m_pVB,0,sizeof(PARTICLE));
		//m_pGraphicDev->DrawPrimitive(D3DPT_POINTLIST,m_dwVTXOffSet,m_dwBatchSize);

		if(m_dwVTXOffSet >= m_dwVTXSize)
			m_dwVTXOffSet = 0;

		PARTICLE* vParticle = 0;
		DWORD numParticlesInBatch = 0;

		m_pVB->Lock(m_dwVTXOffSet * sizeof(PARTICLE), m_dwBatchSize * sizeof(PARTICLE), (void**)&vParticle, m_dwVTXOffSet ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

		list<PARICLEATT>::iterator iter = m_AttributeList.begin();
		list<PARICLEATT>::iterator iter_end = m_AttributeList.end();

		_uint iUpperIndex = 0;
		for (; iter!=iter_end ; ++iter)
		{
			list<PARICLEATT>::iterator iter_Trail = (*m_pTrailList[iUpperIndex]).begin();
			list<PARICLEATT>::iterator iter_Trail_end = (*m_pTrailList[iUpperIndex]).end();

			_uint iIndex = 0;
			for(; iter_Trail != iter_Trail_end ; ++iter_Trail)
			{
				if(iter_Trail->isAlive)
				{
					++iIndex;
					_float fRatioAlpha = _float(iIndex) / (*m_pTrailList[iUpperIndex]).size();

					vParticle->vPos = iter_Trail->vPos;
					D3DXCOLOR NewColor = iter_Trail->vColor;
					NewColor.a *= fRatioAlpha;
					vParticle->dwColor = (D3DCOLOR)NewColor;
					++vParticle;
					numParticlesInBatch++;

					if(numParticlesInBatch == m_dwBatchSize)
					{
						m_pVB->Unlock();

						m_pGraphicDev->DrawPrimitive(D3DPT_POINTLIST,m_dwVTXOffSet,m_dwBatchSize);

						m_dwVTXOffSet += m_dwBatchSize;

						if(m_dwVTXOffSet >= m_dwVTXSize)
							m_dwVTXOffSet = 0;

						m_pVB->Lock(m_dwVTXOffSet * sizeof(PARTICLE),m_dwBatchSize * sizeof(PARTICLE),(void**)&vParticle,m_dwVTXOffSet ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

						numParticlesInBatch = 0;
					}
				}
			}
			++iUpperIndex;



			if(iter->isAlive)
			{

				vParticle->vPos = iter->vPos;
				vParticle->dwColor = (D3DCOLOR)iter->vColor;
				++vParticle;
				numParticlesInBatch++;

				if(numParticlesInBatch == m_dwBatchSize)
				{
					m_pVB->Unlock();


					pEffect->SetTexture("g_baseTexture",pTexture);
					pEffect->CommitChanges();
					m_pGraphicDev->DrawPrimitive(D3DPT_POINTLIST,m_dwVTXOffSet,m_dwBatchSize);

					m_dwVTXOffSet += m_dwBatchSize;

					if(m_dwVTXOffSet >= m_dwVTXSize)
						m_dwVTXOffSet = 0;

					m_pVB->Lock(m_dwVTXOffSet * sizeof(PARTICLE),m_dwBatchSize * sizeof(PARTICLE),(void**)&vParticle,m_dwVTXOffSet ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

					numParticlesInBatch = 0;
				}
			}
		}

		m_pVB->Unlock();

		//m_AttributeList.sort(Compare);


		if(numParticlesInBatch)
		{
			pEffect->SetTexture("g_baseTexture",pTexture);
			pEffect->CommitChanges();
			m_pGraphicDev->DrawPrimitive(D3DPT_POINTLIST,m_dwVTXOffSet,numParticlesInBatch);
		}

		m_dwVTXOffSet += m_dwBatchSize;


		postRender();
	}
}

void Engine::CParticleSystem::postRender()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, true);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE,false);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE,false);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE,false);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	//m_pGraphicDev->SetRenderState(D3DRS_ZENABLE,true);
}

bool Engine::CParticleSystem::isEmpty()
{
	return true;
}

bool Engine::CParticleSystem::isDead()
{
	return true;
}

void Engine::CParticleSystem::RemoveDeadParticles()
{
	list<PARICLEATT>::iterator iter = m_AttributeList.begin();

	while( iter != m_AttributeList.end())
	{
		if(iter->isAlive == false)
		{
			iter = m_AttributeList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void Engine::CParticleSystem::Free(void)
{
	Engine::Safe_Release(m_pVB);

	for(_int i=0; i<m_iMaxParticles; ++i)
	{
		m_pTrailList[i]->clear();
		Engine::Safe_Delete(m_pTrailList[i]);
	}
	Engine::Safe_Delete_Array(m_pTrailList);

	Engine::CGameObject::Free();
}

void Engine::CParticleSystem::MakeInvisible(PARICLEATT* pAttribute, _float fTimeDelta)
{
	if(pAttribute->vColor.a > 0.f)
		pAttribute->vColor.a -= fTimeDelta;
}

