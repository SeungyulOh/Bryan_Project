#include "stdafx.h"
#include "BloodAni.h"
#include "Export_Engine.h"

CBloodAni::CBloodAni(LPDIRECT3DDEVICE9 pGraphicDev,eType MyType,_float fDeadTime)
:Engine::CGameObject(pGraphicDev)
,m_pRendererCom(NULL)
,m_pTransformCom(NULL)
,m_pShader(NULL)
,m_fUV(0.f)
,m_isOnce(false)
,m_iImgIdx(0)
,m_eMyType(MyType)
,m_fRandX(0.f)
,m_fRandZ(0.f)
,m_fChangePosTime(0.f)
,m_fChangeImgTime(0.f)
{
	m_fTimeAcc = 0.f;
	m_fAlpha = 1.f;
	m_fSpeed = (_float)(rand()% 3) + 1.f;
	m_fDir = ((_float)(rand()%2) - 0.5f) * 2.f;
	m_iImgIdx = (rand()%16);
	m_fDeadTime = fDeadTime;
}

CBloodAni::~CBloodAni()
{

}

HRESULT CBloodAni::Ready_GameObject(void)
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);
		
	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);
	
	m_pBufferCom = (Engine::CRect_Texture*)Engine::Clone(COM_STATIC,L"Component_Buffer_RectTexture");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Buffer_RectTexture",m_pBufferCom);

	m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_Ep10");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_Ep10",m_pTextureCom);

	m_pShader = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Default");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Default",m_pShader);
	
	_float fRandX = _float(rand()%3) + 2.f;
	_float fRandY = _float(rand()%3) + 2.f;
	_float fRandZ = _float(rand()%3) + 2.f;
	m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,Engine::_vec3(fRandX,fRandY,fRandZ));
	
	//_float fAngleX,fAngleY,fAngleZ;
	//if(m_eMyType != ATTACK)
	//{
	//	fAngleX = (_float)(rand()%360);
	//	fAngleY = (_float)(rand()%360);
	//	fAngleZ = (_float)(rand()%360);
	//}
	//else
	//{
	//	/*fAngleX = 0.f;
	//	fAngleY = 0.f;
	//	fAngleZ = 0.f;*/
	//	fAngleX =D3DXToRadian((_float)(rand()%20) - 10.f);
	//	fAngleY =D3DXToRadian((_float)(rand()%20) - 10.f);
	//	fAngleZ =D3DXToRadian((_float)(rand()%20) - 10.f);

	//	fRandX = _float(rand()%6) + 10.f;
	//	fRandY = _float(rand()%6) + 10.f;
	//	fRandZ = 1.f;
	//	m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,Engine::_vec3(fRandX,fRandY,fRandZ));
	//}

	Engine::CTransform* pBossTransform = (Engine::CTransform*)Engine::GetList(L"Layer_Boss")->front()->FindComponent(Engine::CGameObject::UPDATE,L"Component_Transform");
	m_pBossWorldMat = pBossTransform->GetWorldMatrix();

	Engine::CDynamicMesh* pPlayerDynamicMesh = (Engine::CDynamicMesh*)Engine::GetList(L"Layer_Boss")->front()->FindComponent(Engine::CGameObject::NONUPDATE,L"Component_Mesh_Kadan");

	Engine::D3DXFRAME_DERIVED* pFrameFound;
	if(m_eMyType == RIGHTHAND)
		pFrameFound = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(pPlayerDynamicMesh->GetRootFrame(),"ValveBiped_Bip01_R_Hand");
	else if(m_eMyType == LEFTHAND)
		pFrameFound = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(pPlayerDynamicMesh->GetRootFrame(),"ValveBiped_Bip01_L_Hand");
	
	if(m_eMyType != ATTACK)
		m_pBossFrame = &pFrameFound->CombinedMatrix;

	_float fAngleX = 0.f;
	_float fAngleY = D3DXToRadian(rand()%361);
	_float fAngleZ = 0.f;
	m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,_vec3(D3DXToRadian(fAngleX),D3DXToRadian(fAngleY),D3DXToRadian(fAngleZ)));

	_matrix matScale,matRotX,matRotY,matRotZ;
	D3DXMatrixScaling(&matScale,fRandX,fRandY,fRandZ);
	D3DXMatrixRotationX(&matRotX,fAngleX);
	D3DXMatrixRotationX(&matRotY,fAngleY);
	D3DXMatrixRotationX(&matRotZ,fAngleZ);
	m_matMyLocal = matScale * matRotX * matRotY * matRotZ;

	/*_vec3 vNewPos = *(_vec3*)&((Engine::MyGetTransMatFromWorldMat(*m_pPlayerFrame) * Engine::MyGetNewMatExcRot(*m_pPlayerWorldMat)).m[3][0]);
	m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,vNewPos);*/

	m_fRandX = (_float)(rand()%17) - 8.f;
	m_fRandZ = (_float)(rand()%17) - 8.f;

	return S_OK;
}

_int CBloodAni::Update_GameObject(const _float& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	m_fChangePosTime += fTimeDelta;
	m_fChangeImgTime += fTimeDelta;
	if(m_fTimeAcc > m_fDeadTime)
		m_fAlpha -= fTimeDelta;
	if(m_fAlpha < 0.f)
		m_isDestroy = true;

	if(m_fChangeImgTime > 0.01f)
	{
		m_iImgIdx++;
		m_fChangeImgTime = 0.f;
	}
	
	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW,&matView);
	D3DXMatrixInverse(&matView,0,&matView);
	
	_vec3 vNewPos = _vec3(0.f,0.f,0.f);
	vNewPos = *(_vec3*)&((Engine::MyGetTransMatFromWorldMat(*m_pBossFrame)* *m_pBossWorldMat).m[3][0]);
	m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,_vec3(m_pTransformCom->GetInfomation(Engine::CTransform::SCALE)->x + fTimeDelta*2.f,m_pTransformCom->GetInfomation(Engine::CTransform::SCALE)->y + fTimeDelta*2.f,m_pTransformCom->GetInfomation(Engine::CTransform::SCALE)->z + fTimeDelta*2.f));
	m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,vNewPos);
	m_pTransformCom->SetBillBoardMat(Engine::MyGetRotMatFromWorldMat(matView));
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	//ComputeViewZ(m_pTransformCom->GetWorldMatrix());
	m_pRendererCom->Add_RenderObject(this, Engine::CRenderer::RT_ALPHA);
	return 0;
}

void CBloodAni::Render_GameObject(void)
{
	m_pShader->SetMatrixOnShader(*m_pTransformCom->GetWorldMatrix());
	m_pShader->GetEffect()->SetTexture("g_baseTexture",m_pTextureCom->GetTexture(0));
	m_pShader->GetEffect()->SetFloat("g_fAlpha",m_fAlpha);
	m_pShader->GetEffect()->SetInt("g_iImgIdx",m_iImgIdx);

	_uint iNumPass = 0;

	m_pShader->GetEffect()->Begin(&iNumPass,0);
	m_pShader->GetEffect()->BeginPass(1);
	
	m_pBufferCom->Render_Buffer();

	m_pShader->GetEffect()->EndPass();
	m_pShader->GetEffect()->End();
}

CBloodAni* CBloodAni::Create(LPDIRECT3DDEVICE9 pGraphicDev,eType MyType,_float fDeadTime)
{
	CBloodAni* pInst = new CBloodAni(pGraphicDev,MyType,fDeadTime);
	
	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;
}

void CBloodAni::Free(void)
{
	Engine::CGameObject::Free();

	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShader);
	
}

