#include "stdafx.h"
#include "Fog.h"
#include "Export_Engine.h"
#include "WaterFall.h"

CFog::CFog(LPDIRECT3DDEVICE9 pGraphicDev, _uint iImgIdx)
:Engine::CGameObject(pGraphicDev)
,m_pRendererCom(NULL)
,m_pTransformCom(NULL)
,m_pShader(NULL)
,m_fUV(0.f)
,m_isOnce(false)
,m_iImgIdx(iImgIdx)
{
	m_fTimeAcc = 0.f;
	m_fAlpha = 1.f;
	m_fSpeed = (_float)(rand()% 5) + 15.f;
	m_fDir = ((_float)(rand()%2) - 0.5f) * 2.f;
}

CFog::~CFog()
{

}

HRESULT CFog::Ready_GameObject(void)
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);
		
	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);
	
	m_pBufferCom = (Engine::CRect_Texture*)Engine::Clone(COM_STATIC,L"Component_Buffer_RectTexture");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Buffer_RectTexture",m_pBufferCom);

	m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_WhirlpoolCloud");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_WhirlpoolCloud",m_pTextureCom);

	m_pShader = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Default");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Default",m_pShader);
	
	_float fScale = rand()%100 + 500.f;
	m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,Engine::_vec3(fScale,fScale,fScale));
	_float fAngleY = (_float)(rand()%360);
	m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,_vec3(D3DX_PI/2.f,D3DXToRadian(fAngleY),0.f));
	m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,_vec3(650.f,450.f,-30.f));

	m_pTransformCom->Update_Component(0.f);
	//m_iImgIdx = 0;//rand()%3;
	return S_OK;
}

_int CFog::Update_GameObject(const _float& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	/*if(m_fTimeAcc > 1.f)
		m_fAlpha -= fTimeDelta;
	if(m_fAlpha < 0.f)
		m_isDestroy = true;*/



	//m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,_vec3(m_pTransformCom->GetInfomation(Engine::CTransform::SCALE)->x+ fTimeDelta*m_fSpeed,m_pTransformCom->GetInfomation(Engine::CTransform::SCALE)->y + fTimeDelta*m_fSpeed,m_pTransformCom->GetInfomation(Engine::CTransform::SCALE)->z+ fTimeDelta*m_fSpeed));
	m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,_vec3(m_pTransformCom->GetInfomation(Engine::CTransform::ANGLE)->x,m_pTransformCom->GetInfomation(Engine::CTransform::ANGLE)->y + fTimeDelta*m_fDir*0.1f,m_pTransformCom->GetInfomation(Engine::CTransform::ANGLE)->z));
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	//ComputeViewZ(m_pTransformCom->GetWorldMatrix());
	m_pRendererCom->Add_RenderObject(this, Engine::CRenderer::RT_ALPHA);
	return 0;
}

void CFog::Render_GameObject(void)
{
	m_pShader->SetMatrixOnShader(*m_pTransformCom->GetWorldMatrix());
	m_pShader->GetEffect()->SetTexture("g_baseTexture",m_pTextureCom->GetTexture(m_iImgIdx));
	m_pShader->GetEffect()->SetFloat("g_fAlpha",m_fAlpha);

	_uint iNumPass = 0;

	m_pShader->GetEffect()->SetVector("g_vOffSetColor",&_vec4(1.f,1.f,1.f,1.f));
	m_pShader->GetEffect()->Begin(&iNumPass,0);
	m_pShader->GetEffect()->BeginPass(2);
	
	m_pBufferCom->Render_Buffer();

	m_pShader->GetEffect()->EndPass();
	m_pShader->GetEffect()->End();
}

CFog* CFog::Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iImgIdx)
{
	CFog* pInst = new CFog(pGraphicDev,iImgIdx);
	
	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;
}

void CFog::Free(void)
{
	Engine::CGameObject::Free();

	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShader);
	
}

