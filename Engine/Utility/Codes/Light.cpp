#include "Light.h"

Engine::CLight::CLight(LPDIRECT3DDEVICE9 pDevice)
:m_pGraphicDev(pDevice)
,m_pVB(NULL)
,m_pIB(NULL)
,m_isRender(true)
{
	m_pGraphicDev->AddRef();
}

Engine::CLight::~CLight()
{

}

Engine::CLight* Engine::CLight::Create(LPDIRECT3DDEVICE9 pDevice, D3DLIGHT9& rLightInfo)
{
	CLight* pMine = new CLight(pDevice);
	pMine->InitLight(rLightInfo);
	return pMine;
}

void Engine::CLight::InitLight(D3DLIGHT9& rLightInfo)
{
	m_tLightInfo = rLightInfo;


	D3DVIEWPORT9 tViewPort;
	m_pGraphicDev->GetViewport(&tViewPort);

	m_pGraphicDev->CreateVertexBuffer(sizeof(Engine::BRUSH)*4,0,Engine::D3DFVF_BRUSH,D3DPOOL_MANAGED,&m_pVB,NULL);

	Engine::BRUSH* pBrush= NULL;
	m_pVB->Lock(0,0,(void**)&pBrush,0);

	pBrush[0].vPos = _vec4(0.f-0.5f,0.f-0.5f,0.f,1.f);
	pBrush[0].vTexUV = _vec2(0.f,0.f);

	pBrush[1].vPos = _vec4((_float)tViewPort.Width-0.5f,0.f-0.5f,0.f,1.f);
	pBrush[1].vTexUV = _vec2(1.f,0.f);

	pBrush[2].vPos = _vec4((_float)tViewPort.Width-0.5f,(_float)tViewPort.Height-0.5f,0.f,1.f);
	pBrush[2].vTexUV = _vec2(1.f,1.f);

	pBrush[3].vPos = _vec4(0.f-0.5f,(_float)tViewPort.Height-0.5f,0.f,1.f);
	pBrush[3].vTexUV = _vec2(0.f,1.f);

	m_pVB->Unlock();

	m_pGraphicDev->CreateIndexBuffer(sizeof(Engine::INDEX16)*2,0,D3DFMT_INDEX16,D3DPOOL_MANAGED,&m_pIB,NULL);

	Engine::INDEX16* pIndex = NULL;

	m_pIB->Lock(0,0,(void**)&pIndex,0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();

}


void Engine::CLight::DeleteLight(void)
{
	delete this;
}


D3DLIGHT9* Engine::CLight::GetLight()
{
	return &m_tLightInfo;
}

void Engine::CLight::Free(void)
{

	Engine::Safe_Release(m_pGraphicDev);

	Engine::Safe_Release(m_pVB);
	Engine::Safe_Release(m_pIB);
}

void Engine::CLight::Render_Light(LPD3DXEFFECT pEffect)
{
	_uint iPassIdx = 0;
	if(m_tLightInfo.Type == D3DLIGHT_DIRECTIONAL)
	{
		pEffect->BeginPass(0);
		pEffect->SetVector("g_vLightWorldDirection",&_vec4(m_tLightInfo.Direction,0.f));
	}
	else if(m_tLightInfo.Type == D3DLIGHT_POINT)
	{
		pEffect->BeginPass(1);
		pEffect->SetVector("g_vLightWorldPos",&_vec4(m_tLightInfo.Position,1.f));
		pEffect->SetFloat("g_fRange",m_tLightInfo.Range);
	}
	
	pEffect->SetVector("g_vLightDiffuse",(_vec4*)&m_tLightInfo.Diffuse);
	pEffect->SetVector("g_vLightAmbient",(_vec4*)&m_tLightInfo.Ambient);
	pEffect->SetVector("g_vLightSpecular",(_vec4*)&m_tLightInfo.Specular);

	pEffect->CommitChanges();

	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(BRUSH));
	m_pGraphicDev->SetFVF(D3DFVF_BRUSH);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
}

void Engine::CLight::ChangeLightInfo(D3DLIGHT9& tLightInfo)
{
	m_tLightInfo = tLightInfo;
}

