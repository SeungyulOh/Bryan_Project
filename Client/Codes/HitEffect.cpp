#include "stdafx.h"
#include "HitEffect.h"
#include "Export_Engine.h"

CHitEffect::CHitEffect(LPDIRECT3DDEVICE9 pGraphicDev,_int iCreateCnt,_vec3 vWorldCreatePos, _int iImgCnt,_float fScaleOffSet)
:Engine::CGameObject(pGraphicDev)
,m_pRendererCom(NULL)
,m_pTransformCom(NULL)
,m_pShader(NULL)
,m_fUV(0.f)
,m_isOnce(false)
,m_iCreateCnt(iCreateCnt)
,m_vWorldCreatePos(vWorldCreatePos)
{
	m_fTimeAcc = 0.f;
	m_fAlpha = 1.f;
	m_fSpeed = (_float)(rand()% 5) + 15.f;
	m_fDir = -1.f;
	m_iImgIdx = iImgCnt;
	m_fScaleOffset = fScaleOffSet;
}

CHitEffect::~CHitEffect()
{
	
}

HRESULT CHitEffect::Ready_GameObject(void)
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);
		
	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);
	
	m_pBufferCom = (Engine::CRect_Texture*)Engine::Clone(COM_STATIC,L"Component_Buffer_RectTexture");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Buffer_RectTexture",m_pBufferCom);

	m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_Hit");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_Hit",m_pTextureCom);

	m_pShader = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Default");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Default",m_pShader);
	
	m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,Engine::_vec3(20.f,20.f,20.f));
	_float fAngleY = D3DX_PI/2.f;
	m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,_vec3(D3DX_PI/2.f,0.f,0.f));

	_matrix matScale,matRotZ,matRotY,matTrans;
	_float fRandScale = rand()%4 + m_fScaleOffset;
	D3DXMatrixScaling(&matScale,fRandScale,fRandScale*3.5f,fRandScale);
	D3DXMatrixRotationZ(&matRotZ,D3DX_PI/2.f + D3DXToRadian(rand()%30) * _float(rand()%3 - 1));
	D3DXMatrixRotationY(&matRotY,D3DX_PI/2.f);
	D3DXMatrixTranslation(&matTrans,0.f,0.f,0.f);

	m_LocalMat = matScale * matRotZ * matTrans;

	

	return S_OK;
}

_int CHitEffect::Update_GameObject(const _float& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	m_fAlpha -= fTimeDelta*3.f;
	if(m_fAlpha < 0.f)
		m_isDestroy = true;

	if(m_fTimeAcc > 0.1f)
	{
		if(m_iCreateCnt > 0)
		{
			Engine::CGameObject* pGameObject = CHitEffect::Create(m_pGraphicDev,--m_iCreateCnt,m_vWorldCreatePos,m_iImgIdx);
			Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
		}
	}

	if(m_isOnce == false)
	{
		m_isOnce = true;
		Engine::CTransform* pPlayerTransform = (Engine::CTransform*)Engine::GetList(L"Layer_Player")->front()->FindComponent(Engine::CGameObject::UPDATE,L"Component_Transform");
		m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,_vec3(Engine::MyGetTransMatFromWorldMat(*pPlayerTransform->GetWorldMatrix())._41,Engine::MyGetTransMatFromWorldMat(*pPlayerTransform->GetWorldMatrix())._42,Engine::MyGetTransMatFromWorldMat(*pPlayerTransform->GetWorldMatrix())._43));
		m_pPlayerMat = pPlayerTransform->GetWorldMatrix();

		_vec3 vNewPos = m_vWorldCreatePos;//*(_vec3*)&m_pPlayerMat->m[2][0] * 18.f + *(_vec3*)&m_pPlayerMat->m[0][0] * 15.f;
		_matrix matTrans, matRot;
		D3DXMatrixTranslation(&matTrans,vNewPos.x,vNewPos.y,vNewPos.z);
		matRot = Engine::MyGetRotMatFromWorldMat(*m_pPlayerMat);
		m_pTransformCom->SetWorldmat(m_LocalMat* matRot * matTrans);
	}
		
	//ComputeViewZ(m_pTransformCom->GetWorldMatrix());
	m_pRendererCom->Add_RenderObject(this, Engine::CRenderer::RT_ALPHA);

	return 0;
}

void CHitEffect::Render_GameObject(void)
{

	_matrix matScale;
	if(m_fTimeAcc < 0.1f)
		D3DXMatrixScaling(&matScale,m_fTimeAcc*10.f,m_fTimeAcc*10.f,m_fTimeAcc*10.f);
	else
		D3DXMatrixIdentity(&matScale);

	m_pShader->SetMatrixOnShader(matScale * *m_pTransformCom->GetWorldMatrix());
	
	m_pShader->GetEffect()->SetTexture("g_baseTexture",m_pTextureCom->GetTexture(m_iImgIdx));
	m_pShader->GetEffect()->SetFloat("g_fAlpha",m_fAlpha);
	m_pShader->GetEffect()->SetFloat("g_fProcess",m_fTimeAcc);

	_uint iNumPass = 0;

	m_pShader->GetEffect()->Begin(&iNumPass,0);
	m_pShader->GetEffect()->BeginPass(2);
	
	m_pBufferCom->Render_Buffer();

	m_pShader->GetEffect()->EndPass();
	m_pShader->GetEffect()->End();

	/*TCHAR szTemp[256] = L"%d";
	wsprintf(szTemp,L"%d",m_iImgIdx);
	Engine::Render_Font(L"Font_Batang",szTemp,&_vec2(200.f,90.f),D3DXCOLOR(1.f,1.f,1.f,1.f));*/
}

CHitEffect* CHitEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev,_int iCreateCnt,_vec3 vWorldCreatePos, _int iImgCnt,_float fScaleOffSet)
{
	CHitEffect* pInst = new CHitEffect(pGraphicDev,iCreateCnt,vWorldCreatePos,iImgCnt,fScaleOffSet);
	
	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;
}

void CHitEffect::Free(void)
{



	Engine::CGameObject::Free();

	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShader);
	
}

