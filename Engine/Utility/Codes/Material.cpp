#include "Material.h"

using namespace Engine;

Engine::CMaterial::CMaterial(LPDIRECT3DDEVICE9 pGraphicDev)
:m_pGraphicDev(pGraphicDev)
{
	ZeroMemory(&m_tMtrl, sizeof(D3DMATERIAL9));
	m_pGraphicDev->AddRef();
}
Engine::CMaterial::CMaterial(const CMaterial& rhs)
:m_pGraphicDev(rhs.m_pGraphicDev)
,m_tMtrl(rhs.m_tMtrl)
{
	m_pGraphicDev->AddRef();
}
Engine::CMaterial::~CMaterial()
{

}

HRESULT Engine::CMaterial::Ready_Material(D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, D3DXCOLOR Emissive, _float Power)
{
	m_tMtrl.Diffuse = Diffuse;
	m_tMtrl.Ambient = Ambient;
	m_tMtrl.Specular = Specular;
	m_tMtrl.Emissive = Emissive;
	m_tMtrl.Power = Power;


	return S_OK;
}

void Engine::CMaterial::Change_Material(MATERIAL eType, void* pInfo)
{
	if(eType == POWER)
		*((_float*)(((D3DXCOLOR*)&m_tMtrl) + eType)) = *((_float*)pInfo);
	else
		*(((D3DXCOLOR*)&m_tMtrl) + eType) = *((D3DXCOLOR*)pInfo);
}

CMaterial* Engine::CMaterial::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, D3DXCOLOR Emissive, _float Power)
{
	CMaterial* pInst = new CMaterial(pGraphicDev);
	if(FAILED(pInst->Ready_Material(Diffuse,Ambient,Specular,Emissive,Power)))
		return NULL;

	return pInst;
}

CComponent* Engine::CMaterial::Clone(void)
{
	return new CMaterial(*this);
}

void Engine::CMaterial::Free(void)
{
	Engine::CComponent::Free();

	Engine::Safe_Release(m_pGraphicDev);
}

