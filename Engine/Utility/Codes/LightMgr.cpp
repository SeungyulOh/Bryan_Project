#include "LightMgr.h"

IMPLEMENT_SINGLETON(Engine::CLightMgr)
Engine::CLightMgr::CLightMgr()
{

}

Engine::CLightMgr::~CLightMgr()
{
}

void Engine::CLightMgr::AddLight(LPDIRECT3DDEVICE9 pDevice ,D3DLIGHT9& tLightInfo)
{
	CLight* pLight = Engine::CLight::Create(pDevice, tLightInfo);
	if(pLight != NULL)
		m_vecLight.push_back(pLight);
}

void Engine::CLightMgr::LightEnable(LPDIRECT3DDEVICE9 pDevice, const _ushort& wIdx)
{
	pDevice->SetLight(wIdx,m_vecLight[wIdx]->GetLight());
	pDevice->LightEnable(wIdx,TRUE);
}

void Engine::CLightMgr::LightDisable(LPDIRECT3DDEVICE9 pDevice, const _ushort& wIdx)
{
	pDevice->LightEnable(wIdx,FALSE);
}

void Engine::CLightMgr::DeleteLight(const _ushort& wIdx)
{
	vector<CLight*>::iterator iter = find(m_vecLight.begin(),m_vecLight.end(),m_vecLight[wIdx]);
	
	m_vecLight[wIdx]->DeleteLight();
	m_vecLight.erase(iter);

	//m_vecLight.erase(iter);
}

void Engine::CLightMgr::Free(void)
{
	for_each(m_vecLight.begin(),m_vecLight.end(),CRelease_Single());
	m_vecLight.clear();
}

void Engine::CLightMgr::Render_Light(LPD3DXEFFECT pEffect)
{
	vector<CLight*>::iterator iter = m_vecLight.begin();
	vector<CLight*>::iterator iter_end = m_vecLight.end();

	for(; iter!=iter_end; ++iter)
	{
		if((*iter)->GetisRender() == true)
			(*iter)->Render_Light(pEffect);
	}

}

void Engine::CLightMgr::Change_LightInfo(D3DLIGHT9& tLightInfo, _uint iIndex)
{
	if(m_vecLight.size() != 0 && m_vecLight.size() >= iIndex + 1)
		m_vecLight[iIndex]->ChangeLightInfo(tLightInfo);
}

D3DLIGHT9 Engine::CLightMgr::GetLightInfo(_uint iIndex)
{
	if(m_vecLight.size() != 0 && m_vecLight.size() >= iIndex + 1)
		return m_vecLight[iIndex]->GetLightInfo();
}

void Engine::CLightMgr::SetisRender(_uint iIndex,_bool isRender)
{
	if(m_vecLight.size() <= iIndex || m_vecLight.size() == 0)
		return;

	m_vecLight[iIndex]->SetisRender(isRender);
}

