#include "stdafx.h"
#include "Atk2Throw.h"
#include "Export_Engine.h"
#include "WindBlade.h"
#include "Atk2Glow.h"
#include "Critical.h"


CAtk2Throw::CAtk2Throw(LPDIRECT3DDEVICE9 pGraphicDev, _int iLeftLife,_vec3 MyFirstPos,_vec3 vFirstMove,_float fOffSet,_float fLifeTime,_float fOffSetAngleX,_int iImgIdx)
:Engine::CGameObject(pGraphicDev)
,m_pRendererCom(NULL)
,m_pTransformCom(NULL)
,m_pShader(NULL)
,m_fUV(0.f)
,m_isOnce(false)
,m_fOffSet(fOffSet)
,m_iLeftLife(iLeftLife)
,m_fLifeTime(fLifeTime)
,m_iImgIdx(iImgIdx)
,m_fOffSetAngleX(fOffSetAngleX)
,m_MyFirstDir(vFirstMove)
,m_MyFirstPos(MyFirstPos)
{
	m_fTimeAcc = 0.f;
	m_fAlpha = 1.f;
	m_fSpeed = (_float)(rand()% 5) + 15.f;
	m_fDir = -1.f;
}

CAtk2Throw::~CAtk2Throw()
{

	
}

HRESULT CAtk2Throw::Ready_GameObject(void)
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);
		
	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);
	
	m_pBufferCom = (Engine::CRect_Texture*)Engine::Clone(COM_STATIC,L"Component_Buffer_RectTexture");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Buffer_RectTexture",m_pBufferCom);

	m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_ring_outer_wind");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_ring_outer_wind",m_pTextureCom);

	m_pShader = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Default");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Default",m_pShader);
	
	m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,Engine::_vec3(20.f,20.f,20.f));
	_float fAngleY = D3DX_PI/2.f;
	m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,_vec3(D3DX_PI/2.f,fAngleY,0.f));

	_matrix matScale,matRotX,matRotY,matTrans;
	D3DXMatrixScaling(&matScale,4.f,4.f,4.f);
	D3DXMatrixRotationX(&matRotX,D3DX_PI/2.f + m_fOffSetAngleX);
	D3DXMatrixRotationY(&matRotY,D3DXToRadian(rand()%360));
	D3DXMatrixTranslation(&matTrans,0.f,0.f,0.f);

	m_LocalMat = matScale *matRotX *matRotY;

	//m_iImgIdx = 1;//rand()%3;
	return S_OK;
}

_int CAtk2Throw::Update_GameObject(const _float& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if(m_iLeftLife != -1)
	{
		if(m_fTimeAcc > m_fLifeTime)
			m_fAlpha -= fTimeDelta*10.f;
		if(m_fAlpha < 0.f)
			m_isDestroy = true;
	}
	else
	{
		if(m_fTimeAcc > 0.6f)
			m_fAlpha -= fTimeDelta*10.f;
		if(m_fAlpha < 0.f)
			m_isDestroy = true;
	}

	if(m_isOnce == false)
	{
		m_isOnce = true;
		Engine::CTransform* pPlayerTransform = (Engine::CTransform*)Engine::GetList(L"Layer_Player")->front()->FindComponent(Engine::CGameObject::UPDATE,L"Component_Transform");
		m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,_vec3(Engine::MyGetTransMatFromWorldMat(*pPlayerTransform->GetWorldMatrix())._41,Engine::MyGetTransMatFromWorldMat(*pPlayerTransform->GetWorldMatrix())._42,Engine::MyGetTransMatFromWorldMat(*pPlayerTransform->GetWorldMatrix())._43));
		m_PlayerMat = *pPlayerTransform->GetWorldMatrix();

		_vec3 vNewPos = m_MyFirstPos + _vec3(0.f,4.f,0.f) + *(_vec3*)&m_PlayerMat.m[2][0] * 20.f * m_fOffSet + m_MyFirstDir * 15.f;
		_matrix matTrans;
		D3DXMatrixTranslation(&matTrans,vNewPos.x,vNewPos.y,vNewPos.z);
		m_pTransformCom->SetWorldmat(m_LocalMat * matTrans);

	}
	
	//ComputeViewZ(m_pTransformCom->GetWorldMatrix());
	m_pRendererCom->Add_RenderObject(this, Engine::CRenderer::RT_ALPHA);
	return 0;
}

void CAtk2Throw::Render_GameObject(void)
{
	if(m_fAlpha<0)
		return;

	_matrix matScale;
	if(m_fTimeAcc < 0.1f)
		D3DXMatrixScaling(&matScale,m_fTimeAcc*20.f,m_fTimeAcc*20.f,m_fTimeAcc*20.f);
	else
	{
		if(m_fTimeAcc < 0.3f && m_fAlpha > 0)
			D3DXMatrixScaling(&matScale,m_fTimeAcc*15.f,m_fTimeAcc*15.f,m_fTimeAcc*15.f);
		else if(m_fTimeAcc > 1.f && m_fAlpha > 0)
			D3DXMatrixScaling(&matScale,m_fTimeAcc*5.f,m_fTimeAcc*5.f,m_fTimeAcc*5.f);
	}

	m_pShader->SetMatrixOnShader(matScale * *m_pTransformCom->GetWorldMatrix());
	m_pShader->GetEffect()->SetTexture("g_baseTexture",m_pTextureCom->GetTexture(m_iImgIdx));
	m_pShader->GetEffect()->SetFloat("g_fAlpha",m_fAlpha);
	m_pShader->GetEffect()->SetFloat("g_fProcess",m_fTimeAcc);

	_uint iNumPass = 0;

	m_pShader->GetEffect()->Begin(&iNumPass,0);
	m_pShader->GetEffect()->BeginPass(2);
	
	for(_int i=0; i<m_iLeftLife+2; ++i)
		m_pBufferCom->Render_Buffer();
	

	m_pShader->GetEffect()->EndPass();
	m_pShader->GetEffect()->End();
}

CAtk2Throw* CAtk2Throw::Create(LPDIRECT3DDEVICE9 pGraphicDev,_int iLeftLife,_vec3 MyFirstPos,_vec3 vFirstMove, _float fOffSet,_float fLifeTime,_float fOffSetAngleX,_int iImgIdx)
{
	CAtk2Throw* pInst = new CAtk2Throw(pGraphicDev,iLeftLife,MyFirstPos,vFirstMove,fOffSet,fLifeTime,fOffSetAngleX,iImgIdx);
	
	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;
}

void CAtk2Throw::Free(void)
{
	if(m_iLeftLife >= 1)
	{
		Engine::CGameObject* pGameObject = CAtk2Throw::Create(m_pGraphicDev,--m_iLeftLife,m_MyFirstPos,m_MyFirstDir,m_fOffSet * 2.f);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
	}
	else if(m_iLeftLife == 0)
	{
		Engine::CGameObject* pGameObject = CAtk2Throw::Create(m_pGraphicDev,-1,m_MyFirstPos,m_MyFirstDir,m_fOffSet * 2.f,0.01f);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);

		for(_int i=0; i<3; ++i)
		{
			Engine::CGameObject* pGameObject = CAtk2Throw::Create(m_pGraphicDev,-1,m_MyFirstPos,m_MyFirstDir,m_fOffSet * 2.f,2.f,D3DX_PI/20.f,1);
			Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
		}

		pGameObject = CWindBlade::Create(m_pGraphicDev,*(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[3][0] + m_MyFirstDir * 20.f * m_fOffSet,1.f,2);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
		pGameObject = CWindBlade::Create(m_pGraphicDev,*(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[3][0] + m_MyFirstDir * 20.f * m_fOffSet,-1.f,2);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);

		pGameObject = CAtk2Glow::Create(m_pGraphicDev,*(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[3][0] + m_MyFirstDir * 20.f * m_fOffSet,10,(_float)(rand()%2)*0.1f);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);

		pGameObject = CCritical::Create(m_pGraphicDev, 1,*(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[3][0] + m_MyFirstDir * 20.f * m_fOffSet);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
	}

	Engine::CGameObject::Free();

	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShader);
	
}

