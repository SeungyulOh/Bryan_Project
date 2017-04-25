#include "stdafx.h"
#include "RedEye.h"
#include "Export_Engine.h"
#include "EyeFollowParticle.h"

CREdEye::CREdEye(LPDIRECT3DDEVICE9 pGraphicDev)
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
,m_pBossVisible(NULL)
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
	m_iImgIdx = 11;
}

CREdEye::~CREdEye()
{

}

HRESULT CREdEye::Ready_GameObject()
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
	

	CBossDevil* pBoss = dynamic_cast<CBossDevil*>(Engine::GetList(L"Layer_Boss")->front());
	if(pBoss != NULL)
		m_pBossVisible = pBoss->GetVisible();

	return S_OK; 
}

void CREdEye::ResetParticle(Engine::PARICLEATT* pAttribute, _uint iIndex)
{
	pAttribute->isAlive = true;
	pAttribute->vPos = m_vOrigin;	
	pAttribute->vColor = D3DXCOLOR(1.f,1.f,1.f,1.f);
	pAttribute->fLifeTime = 9999.f;
}


_int CREdEye::Update_GameObject(const _float& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	if(Engine::GetList(L"Layer_Boss")->front()->GetDestroy() == true)
		m_isDestroy = true;

	if(m_isOnce == false)
	{
		m_isOnce = true;

		m_pParentTransform = (Engine::CTransform*)Engine::GetList(L"Layer_Boss")->front()->FindComponent(Engine::CGameObject::UPDATE,L"Component_Transform");
		m_pParentTransform->AddRefCnt();

		Engine::CDynamicMesh* pParentMesh = (Engine::CDynamicMesh*)Engine::GetList(L"Layer_Boss")->front()->FindComponent(Engine::CGameObject::NONUPDATE,L"Component_Mesh_Kadan");
		Engine::D3DXFRAME_DERIVED* pFrameFound = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(pParentMesh->GetRootFrame(),"effect_Bone01");
		m_matEyeFrame = &pFrameFound->CombinedMatrix;

		/*m_pEffectLayer = Engine::GetLayer(L"Layer_EffectNotInEvent");
		Engine::CGameObject* pTrail = CParticle_Trail::Create(m_pGraphicDev,m_pTransformCom->GetWorldMatrix(),&m_AttributeList.front().vPos,m_AttributeList.front().fLifeTime);
		m_pEffectLayer->Add_Object(pTrail);*/

		_matrix matParent = *m_matEyeFrame * *m_pParentTransform->GetWorldMatrix();
		Engine::CGameObject* pGameObject = CEyeFollowParticle::Create(m_pGraphicDev,(_vec3*)&matParent.m[3][0],50,m_pParentTransform,m_matEyeFrame,&m_iImgIdx);
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
	}

	if(m_fTimeAcc > 0.3f)
	{
		m_fRand = (rand() % 2) * 0.1f;
		m_fTimeAcc = 0.f;
	}
	
	_matrix matTrans,matScale,matRot;
	D3DXMatrixTranslation(&matTrans,50.f,50.f,0.f);
	D3DXMatrixScaling(&matScale,1.f,1.f,1.f);
	D3DXMatrixRotationY(&matRot,D3DX_PI/2.f);
	m_pTransformCom->SetWorldmat(matScale* matRot * /*matTrans **/ *m_matEyeFrame * *m_pParentTransform->GetWorldMatrix());


	if(m_isDestroy == false && Engine::GetList(L"Layer_Boss")->size() == 2 && *m_pBossVisible == true)
		m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_ALPHA);


	return 0;
}


void CREdEye::Render_GameObject()
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD,&(*m_matPlayerRH * *m_pParentTransform->GetWorldMatrix()));
	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW,&matView);
	D3DXMatrixInverse(&matView,0,&matView);
	m_pShaderCom->GetEffect()->SetVector("g_vCamWorldPos", (_vec4*)&matView.m[3][0]);


	m_pShaderCom->SetMatrixOnShader(*m_pTransformCom->GetWorldMatrix());
	m_pShaderCom->GetEffect()->SetTexture("g_baseTexture",m_pTextureCom->GetTexture(m_iImgIdx));
	m_pShaderCom->GetEffect()->SetFloat("g_fRandValue",m_fRand);
	m_pShaderCom->GetEffect()->SetFloat("g_fMaxSize",70.f);

	m_pShaderCom->GetEffect()->Begin(NULL,0);
	m_pShaderCom->GetEffect()->BeginPass(0);

	Engine::CParticleSystem::Render_GameObject();

	m_pShaderCom->GetEffect()->EndPass();
	m_pShaderCom->GetEffect()->End();
}


CREdEye* CREdEye::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CREdEye* pInst = new CREdEye(pGraphicDev);
	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;

}

void CREdEye::Free()
{
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pParentTransform);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShaderCom);

	Engine::CParticleSystem::Free();
}

