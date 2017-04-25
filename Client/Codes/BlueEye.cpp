#include "stdafx.h"
#include "BlueEye.h"
#include "Export_Engine.h"
#include "EyeFollowParticle.h"

CBlueEye::CBlueEye(LPDIRECT3DDEVICE9 pGraphicDev, EYETYPE eType)
:Engine::CParticleSystem(pGraphicDev)
,m_pRendererCom(NULL)
,m_pTextureCom(NULL)
,m_pShaderCom(NULL)
,m_pParentTransform(NULL)
,m_isOnce(false)
,m_pEffectLayer(NULL)
,m_pTransformCom(NULL)
,m_fRand(0.f)
,m_fTimeAcc(0.f)
,m_pParentMesh(NULL)
,m_eType(eType)
,m_fAlpha(1.f)
,m_pRightParticle(NULL)
,m_pLeftParticle(NULL)
{
	m_vOrigin = _vec3(0.f,0.f,0.f);
	int iNumParticles = 1;

	m_fSize = 20.f;
	m_dwVTXSize = 2048;
	m_dwVTXOffSet = 0;
	m_dwBatchSize = 512;

	CParticleSystem::Init(iNumParticles);

	for(int i =0; i<iNumParticles ; ++i)
		AddParticle();

	m_fViewZ = 0.1f;
	m_iImgIdx = 0;
}

CBlueEye::~CBlueEye()
{

}

HRESULT CBlueEye::Ready_GameObject()
{

	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);

	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);

	m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_Particle_Glow");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_Particle_Glow",m_pTextureCom);

	m_pShaderCom = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Particle");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Particle",m_pShaderCom);

	m_pShaderCom->GetEffect()->SetFloat("g_fInputSize",m_fSize);

	D3DVIEWPORT9 tViewPort;
	m_pGraphicDev->GetViewport(&tViewPort);

	m_pShaderCom->GetEffect()->SetFloat("g_fViewPortY",(float)tViewPort.Height);
	

	return S_OK; 
}

void CBlueEye::ResetParticle(Engine::PARICLEATT* pAttribute, _uint iIndex)
{
	pAttribute->isAlive = true;
	pAttribute->vPos = m_vOrigin;	
	pAttribute->vColor = D3DXCOLOR(1.f,1.f,1.f,1.f);
	pAttribute->fLifeTime = 9999.f;
}


_int CBlueEye::Update_GameObject(const _float& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if(m_isDestroy == true)
		return 0;

	if(m_isOnce == false)
	{
		m_isOnce = true;
		Engine::D3DXFRAME_DERIVED* pFrameFound = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(m_pParentMesh->GetRootFrame(),"jiggle_head_01");
		m_matHeadFrame = &pFrameFound->CombinedMatrix;

		//_matrix matParent = *m_matEyeFrame * *m_pParentTransform->GetWorldMatrix();
		//Engine::CGameObject* pGameObject = CEyeFollowParticle::Create(m_pGraphicDev,(_vec3*)&matParent.m[3][0],50);
		//Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);

		_matrix matParent = *m_matHeadFrame * *m_pParentTransform->GetWorldMatrix();
		Engine::CGameObject* pGameObject = m_pRightParticle = CEyeFollowParticle::Create(m_pGraphicDev,(_vec3*)&matParent.m[3][0],50,m_pParentTransform,m_matHeadFrame,&m_iImgIdx,CEyeFollowParticle::RIGHT);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
		pGameObject = m_pLeftParticle = CEyeFollowParticle::Create(m_pGraphicDev,(_vec3*)&matParent.m[3][0],50,m_pParentTransform,m_matHeadFrame,&m_iImgIdx,CEyeFollowParticle::LEFT);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
	}

	m_pRightParticle->SetAlpha(m_fAlpha);
	m_pLeftParticle->SetAlpha(m_fAlpha);

	if(m_fTimeAcc > 0.3f)
	{
		m_fRand = (rand() % 2) * 0.1f;
		m_fTimeAcc = 0.f;
	}
	
	_vec3 vOffSet;
	if(m_eType == R_EYE)
		vOffSet = *D3DXVec3Normalize(&_vec3(),(_vec3*)&m_matHeadFrame->m[2][0]) * 10.f + *D3DXVec3Normalize(&_vec3(),(_vec3*)&m_matHeadFrame->m[0][0]) * 2.5f + *D3DXVec3Normalize(&_vec3(),(_vec3*)&m_matHeadFrame->m[1][0]) * -5.f;
	else
		vOffSet = *D3DXVec3Normalize(&_vec3(),(_vec3*)&m_matHeadFrame->m[2][0]) * 10.f + *D3DXVec3Normalize(&_vec3(),(_vec3*)&m_matHeadFrame->m[0][0]) * -2.5f + *D3DXVec3Normalize(&_vec3(),(_vec3*)&m_matHeadFrame->m[1][0]) * -5.f;


	_matrix matTrans,matScale,matRot;
	D3DXMatrixTranslation(&matTrans,vOffSet.x,vOffSet.y,vOffSet.z);
	D3DXMatrixScaling(&matScale,1.f,1.f,1.f);
	D3DXMatrixRotationY(&matRot,D3DX_PI/2.f);
	m_pTransformCom->SetWorldmat(matScale* matRot * matTrans * *m_matHeadFrame * *m_pParentTransform->GetWorldMatrix());
	m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_ALPHA);


	return 0;
}


void CBlueEye::Render_GameObject()
{
	if(m_isDestroy == true)
		return;

	//m_pGraphicDev->SetTransform(D3DTS_WORLD,&(*m_matPlayerRH * *m_pParentTransform->GetWorldMatrix()));
	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW,&matView);
	D3DXMatrixInverse(&matView,0,&matView);
	m_pShaderCom->GetEffect()->SetVector("g_vCamWorldPos", (_vec4*)&matView.m[3][0]);


	m_pShaderCom->SetMatrixOnShader(*m_pTransformCom->GetWorldMatrix());
	m_pShaderCom->GetEffect()->SetTexture("g_baseTexture",m_pTextureCom->GetTexture(m_iImgIdx));
	m_pShaderCom->GetEffect()->SetFloat("g_fRandValue",m_fRand);
	m_pShaderCom->GetEffect()->SetFloat("g_fMaxSize",70.f);
	m_pShaderCom->GetEffect()->SetFloat("g_fAlpha",m_fAlpha);

	m_pShaderCom->GetEffect()->Begin(NULL,0);
	m_pShaderCom->GetEffect()->BeginPass(0);

	Engine::CParticleSystem::Render_GameObject();

	m_pShaderCom->GetEffect()->EndPass();
	m_pShaderCom->GetEffect()->End();
}


CBlueEye* CBlueEye::Create(LPDIRECT3DDEVICE9 pGraphicDev, EYETYPE eType)
{
	CBlueEye* pInst = new CBlueEye(pGraphicDev,eType);
	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;

}

void CBlueEye::Free()
{
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pParentTransform);
	Engine::Safe_Release(m_pParentMesh);

	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShaderCom);

	m_pRightParticle->SetDestroy(true);
	m_pLeftParticle->SetDestroy(true);

	//Engine::Safe_Release(m_pRightParticle);
	//Engine::Safe_Release(m_pLeftParticle);

	Engine::CParticleSystem::Free();
}

void CBlueEye::SetParentTransform(Engine::CTransform* pParent)
{
	m_pParentTransform = pParent;
	m_pParentTransform->AddRefCnt();
}

void CBlueEye::SetParentMesh(Engine::CDynamicMesh* pParentMesh)
{
	m_pParentMesh = pParentMesh;
	m_pParentMesh->AddRefCnt();
}

