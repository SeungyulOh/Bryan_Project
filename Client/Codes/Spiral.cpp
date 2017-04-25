#include "stdafx.h"
#include "Spiral.h"
#include "Export_Engine.h"
#include "Default_Camera.h"

CSpiral::CSpiral(LPDIRECT3DDEVICE9 pGraphicDev)
:Engine::CGameObject(pGraphicDev)
,m_pRendererCom(NULL)
,m_pTransformCom(NULL)
,m_pShader(NULL)
,m_fUV(0.f)
,m_isOnce(false)
{
	m_fTimeAcc = 0.f;
	m_fAlpha = 1.f;
	m_fSpeed = (_float)(rand()% 5) + 15.f;
	m_fDir = ((_float)(rand()%2) - 0.5f) * 2.f;
}

CSpiral::~CSpiral()
{

}

HRESULT CSpiral::Ready_GameObject(void)
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);
		
	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);
	
	m_pBufferCom = (Engine::CRect_Texture*)Engine::Clone(COM_STATIC,L"Component_Buffer_RectTexture");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Buffer_RectTexture",m_pBufferCom);

	m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_simpleaura");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_simpleaura",m_pTextureCom);

	m_pTextureDistortion = (Engine::CTexture*)Engine::Clone(COM_STATIC,L"Component_Texture_efx_ripple1_normal");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_efx_ripple1_normal",m_pTextureCom);

	m_pShader = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Default");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Default",m_pShader);

	m_pShaderForRenderTarget = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_RenderTarget");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_RenderTarget",m_pShaderForRenderTarget);

	m_iImgIdx = 2;
	m_fScale = 3.5f;
	m_fOffSetAngle = D3DXToRadian(rand()%360);


	return S_OK;
}

_int CSpiral::Update_GameObject(const _float& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	if(m_fTimeAcc > 0.2f)
		m_fAlpha -= fTimeDelta;
	if(m_fAlpha < 0.f)
	{
		m_isDestroy = true;
		m_pRendererCom->SetisBlur(false);
	}

	
	//ComputeViewZ(m_pTransformCom->GetWorldMatrix());
	m_pRendererCom->Add_RenderObject(this, Engine::CRenderer::RT_ALPHA);
	return 0;
}

void CSpiral::Render_GameObject(void)
{
	m_fMat[0] = cosf(m_fOffSetAngle+m_fTimeAcc);
	m_fMat[1] = -sinf(m_fOffSetAngle+m_fTimeAcc);
	m_fMat[2] = sinf(m_fOffSetAngle+m_fTimeAcc);
	m_fMat[3] = cosf(m_fOffSetAngle+m_fTimeAcc);	
	m_pShader->GetEffect()->SetFloatArray("g_RotMat",m_fMat,4);

	m_pShader->SetMatrixOnShader(*m_pTransformCom->GetWorldMatrix());
	m_pShader->GetEffect()->SetTexture("g_baseTexture",m_pTextureCom->GetTexture(m_iImgIdx));
	m_pShader->GetEffect()->SetFloat("g_fAlpha",m_fAlpha);
	m_pShader->GetEffect()->SetFloat("g_fScale",m_fScale + m_fTimeAcc);

	_uint iNumPass = 0;
	//m_pGraphicDev->Clear(0,0,D3DCLEAR_TARGET,D3DXCOLOR(0.5f,0.5f,0.f,1.f),1.f,0);

	m_pShader->GetEffect()->Begin(&iNumPass,0);
	m_pShader->GetEffect()->BeginPass(3);
	
	m_pBufferCom->Render_Buffer();

	m_pShader->GetEffect()->EndPass();
	m_pShader->GetEffect()->End();

	//m_pShaderForRenderTarget->GetEffect()->SetFloat("g_fProceed",m_fTimeAcc*10.f);
	//Engine::GetRenderer()->Render_WithDistortion(m_pShaderForRenderTarget->GetEffect(),0,m_pTextureDistortion->GetTexture(),m_fMat);
	//_float fFocusPoint[2] = {0.5f,0.4f};
	////m_pShaderForRenderTarget->GetEffect()->SetFloat("g_fBlurPower",0.05f);
	//m_pShaderForRenderTarget->GetEffect()->SetFloatArray("g_fFocusPoint",fFocusPoint,2);
	//Engine::GetRenderer()->Render_WithRadialBlur(m_pShaderForRenderTarget->GetEffect(),3,max(_int(6 - m_fTimeAcc*5.f),1));

	
}

CSpiral* CSpiral::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSpiral* pInst = new CSpiral(pGraphicDev);
	
	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;
}

void CSpiral::Free(void)
{
	Engine::CGameObject::Free();

	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShader);
	
	Engine::Safe_Release(m_pShaderForRenderTarget);
	Engine::Safe_Release(m_pTextureDistortion);
}

