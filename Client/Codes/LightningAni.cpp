#include "stdafx.h"
#include "LightningAni.h"
#include "Export_Engine.h"

CLightningAni::CLightningAni(LPDIRECT3DDEVICE9 pGraphicDev,eType MyType,_float fDeadTime)
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

CLightningAni::~CLightningAni()
{

}

HRESULT CLightningAni::Ready_GameObject(void)
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);
		
	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);
	
	m_pBufferCom = (Engine::CRect_Texture*)Engine::Clone(COM_STATIC,L"Component_Buffer_RectTexture");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Buffer_RectTexture",m_pBufferCom);

	m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_Particle_lightning_ani");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_Particle_lightning_ani",m_pTextureCom);

	m_pShader = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Default");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Default",m_pShader);
	
	_float fRandX = _float(rand()%3) + 1.f;
	_float fRandY = _float(rand()%3) + 1.f;
	_float fRandZ = _float(rand()%3) + 1.f;
	m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,Engine::_vec3(fRandX,fRandY,fRandZ));
	
	_float fAngleX,fAngleY,fAngleZ;
	if(m_eMyType != ATTACK)
	{
		fAngleX = (_float)(rand()%360);
		fAngleY = (_float)(rand()%360);
		fAngleZ = (_float)(rand()%360);
	}
	else
	{
		/*fAngleX = 0.f;
		fAngleY = 0.f;
		fAngleZ = 0.f;*/
		fAngleX =D3DXToRadian((_float)(rand()%20) - 10.f);
		fAngleY =D3DXToRadian((_float)(rand()%20) - 10.f);
		fAngleZ =D3DXToRadian((_float)(rand()%20) - 10.f);

		fRandX = _float(rand()%6) + 10.f;
		fRandY = _float(rand()%6) + 10.f;
		fRandZ = 1.f;
		m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,Engine::_vec3(fRandX,fRandY,fRandZ));
	}
	m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,_vec3(D3DXToRadian(fAngleX),D3DXToRadian(fAngleY),D3DXToRadian(fAngleZ)));

	Engine::CTransform* pPlayerTransform = (Engine::CTransform*)Engine::GetList(L"Layer_Player")->front()->FindComponent(Engine::CGameObject::UPDATE,L"Component_Transform");
	m_pPlayerWorldMat = pPlayerTransform->GetWorldMatrix();

	Engine::CDynamicMesh* pPlayerDynamicMesh = (Engine::CDynamicMesh*)Engine::GetList(L"Layer_Player")->front()->FindComponent(Engine::CGameObject::NONUPDATE,L"Component_Mesh_Hagie");

	Engine::D3DXFRAME_DERIVED* pFrameFound;
	if(m_eMyType == RIGHTHAND)
		pFrameFound = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(pPlayerDynamicMesh->GetRootFrame(),"ValveBiped_Bip01_R_Hand");
	else if(m_eMyType == LEFTHAND)
		pFrameFound = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(pPlayerDynamicMesh->GetRootFrame(),"ValveBiped_Bip01_L_Hand");
	
	if(m_eMyType != ATTACK)
		m_pPlayerFrame = &pFrameFound->CombinedMatrix;


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

_int CLightningAni::Update_GameObject(const _float& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	m_fChangePosTime += fTimeDelta;
	m_fChangeImgTime += fTimeDelta;
	if(m_fTimeAcc > m_fDeadTime)
		m_fAlpha -= fTimeDelta;
	if(m_fAlpha < 0.f)
		m_isDestroy = true;

	if(m_fChangeImgTime > 0.1f)
	{
		m_iImgIdx++;
		m_fChangeImgTime = 0.f;
	}
	

	_vec3 vNewPos = _vec3(0.f,0.f,0.f);
	if(m_eMyType !=ATTACK)
	{
		vNewPos = *(_vec3*)&((Engine::MyGetTransMatFromWorldMat(*m_pPlayerFrame)* *m_pPlayerWorldMat).m[3][0]);
		m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,_vec3(m_pTransformCom->GetInfomation(Engine::CTransform::SCALE)->x + fTimeDelta*2.f,m_pTransformCom->GetInfomation(Engine::CTransform::SCALE)->y + fTimeDelta*2.f,m_pTransformCom->GetInfomation(Engine::CTransform::SCALE)->z + fTimeDelta*2.f));
		m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,vNewPos);
		Engine::CGameObject::Update_GameObject(fTimeDelta);
	}
	else
	{
		_matrix matWorld = m_matMyLocal * Engine::MyGetNewMatExcScale(*m_pPlayerWorldMat);
	
		
		if(m_fChangePosTime > 0.4f)
		{
			m_fChangePosTime = 0.f;
			m_fRandX = (_float)(rand()%17) - 8.f;
			m_fRandZ = (_float)(rand()%17) - 8.f;
		}
		

		_vec3 vNewPos = *(_vec3*)&matWorld.m[3][0] + _vec3(m_fRandX,3.f,m_fRandZ);
		memcpy(&matWorld.m[3][0],&vNewPos,sizeof(_float)*3);
		/*_matrix matWorld = Engine::MyGetNewMatExcScale(*m_pPlayerWorldMat);
		_vec3 vOffSet = *(_vec3*)&matWorld.m[2][0] * 10.f;
		matWorld._41 += vOffSet.x;
		matWorld._42 += vOffSet.y + 3.f;
		matWorld._43 += vOffSet.z;*/
		m_pTransformCom->SetWorldmat(matWorld);
	}
	
	//ComputeViewZ(m_pTransformCom->GetWorldMatrix());
	m_pRendererCom->Add_RenderObject(this, Engine::CRenderer::RT_ALPHA);
	return 0;
}

void CLightningAni::Render_GameObject(void)
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

CLightningAni* CLightningAni::Create(LPDIRECT3DDEVICE9 pGraphicDev,eType MyType,_float fDeadTime)
{
	CLightningAni* pInst = new CLightningAni(pGraphicDev,MyType,fDeadTime);
	
	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;
}

void CLightningAni::Free(void)
{
	Engine::CGameObject::Free();

	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShader);
	
}

