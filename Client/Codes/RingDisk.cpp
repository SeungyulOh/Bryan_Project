#include "stdafx.h"
#include "RingDisk.h"
#include "Export_Engine.h"

CRingDisk::CRingDisk(LPDIRECT3DDEVICE9 pGraphicDev)
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
	m_fDir = -1.f;
}

CRingDisk::~CRingDisk()
{
	
}

HRESULT CRingDisk::Ready_GameObject(void)
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);
		
	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);
	
	m_pBufferCom = (Engine::CRect_Texture*)Engine::Clone(COM_STATIC,L"Component_Buffer_RectTexture");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Buffer_RectTexture",m_pBufferCom);

	m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_ring_disk");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_ring_disk",m_pTextureCom);

	m_pShader = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Default");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Default",m_pShader);
	
	m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,Engine::_vec3(20.f,20.f,20.f));
	_float fAngleY = D3DX_PI/2.f;
	m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,_vec3(D3DX_PI/2.f,fAngleY,0.f));

	_matrix matScale,matRotX,matRotY,matTrans;
	D3DXMatrixScaling(&matScale,4.f,4.f,4.f);
	D3DXMatrixRotationX(&matRotX,D3DX_PI/2.f);
	D3DXMatrixRotationY(&matRotY,D3DX_PI/2.f);
	D3DXMatrixTranslation(&matTrans,0.f,0.f,0.f);

	m_LocalMat = matScale;// * matTrans;// * matRotY;

	m_iImgIdx = 1;//rand()%3;
	return S_OK;
}

_int CRingDisk::Update_GameObject(const _float& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	if(m_fTimeAcc > 0.05f)
		m_fAlpha -= fTimeDelta;
	if(m_fAlpha < 0.f)
		m_isDestroy = true;

	if(m_isOnce == false)
	{
		m_isOnce = true;
		Engine::CTransform* pPlayerTransform = (Engine::CTransform*)Engine::GetList(L"Layer_Player")->front()->FindComponent(Engine::CGameObject::UPDATE,L"Component_Transform");
		m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,_vec3(Engine::MyGetTransMatFromWorldMat(*pPlayerTransform->GetWorldMatrix())._41,Engine::MyGetTransMatFromWorldMat(*pPlayerTransform->GetWorldMatrix())._42,Engine::MyGetTransMatFromWorldMat(*pPlayerTransform->GetWorldMatrix())._43));
		m_pPlayerMat = pPlayerTransform->GetWorldMatrix();

		_vec3 vNewPos = _vec3(0.f,4.f,0.f) + *(_vec3*)&m_pPlayerMat->m[2][0] * 18.f + *(_vec3*)&m_pPlayerMat->m[0][0] * 15.f;
		_matrix matTrans;
		D3DXMatrixTranslation(&matTrans,vNewPos.x,vNewPos.y,vNewPos.z);
		m_pTransformCom->SetWorldmat(m_LocalMat* Engine::MyGetNewMatExcScale(*m_pPlayerMat) * matTrans);
	}

	//m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,_vec3(m_pTransformCom->GetInfomation(Engine::CTransform::SCALE)->x+ fTimeDelta*m_fSpeed,m_pTransformCom->GetInfomation(Engine::CTransform::SCALE)->y + fTimeDelta*m_fSpeed,m_pTransformCom->GetInfomation(Engine::CTransform::SCALE)->z+ fTimeDelta*m_fSpeed));
	//m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,_vec3(m_pTransformCom->GetInfomation(Engine::CTransform::ANGLE)->x,m_pTransformCom->GetInfomation(Engine::CTransform::ANGLE)->y + fTimeDelta*m_fDir*0.1f,m_pTransformCom->GetInfomation(Engine::CTransform::ANGLE)->z));

	/*_vec3 vNewPos = *(_vec3*)&m_pPlayerMat->m[3][0];
	m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,vNewPos);
	Engine::CGameObject::Update_GameObject(fTimeDelta);*/
		
	//ComputeViewZ(m_pTransformCom->GetWorldMatrix());
	m_pRendererCom->Add_RenderObject(this, Engine::CRenderer::RT_ALPHA);
	return 0;
}

void CRingDisk::Render_GameObject(void)
{

	_matrix matScale;
	if(m_fTimeAcc < 0.5f)
		D3DXMatrixScaling(&matScale,m_fTimeAcc*2.f,m_fTimeAcc*2.f,m_fTimeAcc*2.f);
	else
		D3DXMatrixIdentity(&matScale);

	m_pShader->SetMatrixOnShader(matScale * *m_pTransformCom->GetWorldMatrix());
	m_pShader->GetEffect()->SetTexture("g_baseTexture",m_pTextureCom->GetTexture(0));
	m_pShader->GetEffect()->SetFloat("g_fAlpha",m_fAlpha);
	m_pShader->GetEffect()->SetFloat("g_fProcess",m_fTimeAcc);

	_uint iNumPass = 0;

	m_pShader->GetEffect()->Begin(&iNumPass,0);
	m_pShader->GetEffect()->BeginPass(2);
	
	m_pBufferCom->Render_Buffer();

	m_pShader->GetEffect()->EndPass();
	m_pShader->GetEffect()->End();
}

CRingDisk* CRingDisk::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRingDisk* pInst = new CRingDisk(pGraphicDev);
	
	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;
}

void CRingDisk::Free(void)
{
	Engine::CGameObject::Free();

	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShader);
	
}

