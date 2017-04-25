#include "stdafx.h"
#include "Terrain.h"
#include "Export_Engine.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
:Engine::CGameObject(pGraphicDev)
{

}

CTerrain::~CTerrain()
{

}

HRESULT CTerrain::Ready_GameObject(void)
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);

	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);

	m_pBufferCom = (Engine::CTerrain_Texture*)Engine::Clone(COM_STAGE,L"Component_Buffer_Terrain");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Buffer_Terrain", m_pBufferCom);

	m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_Terrain");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_Terrain", m_pTextureCom);

	m_pMaterialCom = (Engine::CMaterial*)Engine::Clone(COM_STATIC,L"Component_Material");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Material", m_pMaterialCom);


	return S_OK;
}

_int CTerrain::Update_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);
	
	//m_pMaterialCom->Change_Material(Engine::CMaterial::DIFFUSE,&D3DXCOLOR(1.f,0.f,0.f,1.f));
	m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_NONALPHA);
	return 0;
}

void CTerrain::Render_GameObject(void)
{

	m_pTransformCom->SetUp_OnGraphicDev();
	m_pTextureCom->SetUp_OnGraphicDev();
	m_pMaterialCom->SetUp_OnGraphicDev();
	m_pBufferCom->Render_Buffer();

}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrain* pInst = new CTerrain(pGraphicDev);
	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;
}

void CTerrain::Free(void)
{
	Engine::CGameObject::Free();

	Engine::Safe_Release(m_pMaterialCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTextureCom);

}

