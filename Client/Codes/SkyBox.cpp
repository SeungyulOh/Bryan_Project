#include "stdafx.h"
#include "SkyBox.h"
#include "Export_Engine.h"

CSkyBox::CSkyBox(LPDIRECT3DDEVICE9 pGraphicDev)
:Engine::CGameObject(pGraphicDev)
{

}

CSkyBox::~CSkyBox(void)
{

}

HRESULT CSkyBox::Ready_GameObject(void)
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);

	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);

	m_pCubeBufCom = (Engine::CCube_Tex*)Engine::Clone(COM_STATIC,L"Component_Buffer_CubeTex");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Buffer_CubeTex",m_pCubeBufCom);

	m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_SkyBox");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_SkyBox",m_pTextureCom);

	m_pShaderCom = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Skybox");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Skybox",m_pShaderCom);

	m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,_vec3(0.f,0.f,0.f));
	m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,_vec3(30.f,30.f,30.f));

	return S_OK;
}

_int CSkyBox::Update_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);
	m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_PRIORITY);

	return 0;
}

_int CSkyBox::LastUpdate_GameObject(const _float& fTimeDelta)
{
	m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,Engine::MyGetCamPosition(m_pGraphicDev, Engine::VEC_POSITION));
	m_pTransformCom->Update_Component(fTimeDelta);
	return 0;
}


void CSkyBox::Render_GameObject(void)
{
	


	m_pShaderCom->SetMatrixOnShader(*m_pTransformCom->GetWorldMatrix());
	m_pShaderCom->GetEffect()->SetTexture("g_BaseTexture",m_pTextureCom->GetTexture());
	
	m_pShaderCom->GetEffect()->Begin(0,0);
	m_pShaderCom->GetEffect()->BeginPass(0);
	
	m_pCubeBufCom->Render_Buffer();

	m_pShaderCom->GetEffect()->EndPass();
	m_pShaderCom->GetEffect()->End();

}

CSkyBox* CSkyBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSkyBox* pInst = new CSkyBox(pGraphicDev);

	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;
}

void CSkyBox::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pCubeBufCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShaderCom);

	Engine::CGameObject::Free();
}
