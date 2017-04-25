#include "stdafx.h"
#include "Spirit.h"
#include "Export_Engine.h"

CSpirit::CSpirit(LPDIRECT3DDEVICE9 pGraphicDev)
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
{
	m_vOrigin = _vec3(50.f,50.f,0.f);
	int iNumParticles = 1;

	m_fSize = 20.f;
	m_dwVTXSize = 2048;
	m_dwVTXOffSet = 0;
	m_dwBatchSize = 512;

	CParticleSystem::Init(iNumParticles);

	for(int i =0; i<iNumParticles ; ++i)
		AddParticle();

	m_fViewZ = 0.1f;
}

CSpirit::~CSpirit()
{

}

HRESULT CSpirit::Ready_GameObject()
{

	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);

	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);

	m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_Particle_yellowflare_white_glow");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_Particle_yellowflare_white_glow",m_pTextureCom);

	m_pShaderCom = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Particle");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Particle",m_pShaderCom);

	m_pShaderCom->GetEffect()->SetFloat("g_fInputSize",m_fSize);

	D3DVIEWPORT9 tViewPort;
	m_pGraphicDev->GetViewport(&tViewPort);

	m_pShaderCom->GetEffect()->SetFloat("g_fViewPortY",(float)tViewPort.Height);
	

	return S_OK; 
}

void CSpirit::ResetParticle(Engine::PARICLEATT* pAttribute, _uint iIndex)
{
	pAttribute->isAlive = true;
	pAttribute->vPos = m_vOrigin;	
	pAttribute->vColor = D3DXCOLOR(1.f,1.f,1.f,1.f);
	pAttribute->fLifeTime = 9999.f;
}


_int CSpirit::Update_GameObject(const _float& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if(m_isOnce == false)
	{
		m_isOnce = true;

		m_pParentTransform = (Engine::CTransform*)Engine::GetList(L"Layer_Player")->front()->FindComponent(Engine::CGameObject::UPDATE,L"Component_Transform");
		m_pParentTransform->AddRefCnt();

		Engine::CDynamicMesh* pPlayerMesh = (Engine::CDynamicMesh*)Engine::GetList(L"Layer_Player")->front()->FindComponent(Engine::CGameObject::NONUPDATE,L"Component_Mesh_Hagie");
		Engine::D3DXFRAME_DERIVED* pFrameFound = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(pPlayerMesh->GetRootFrame(),"ValveBiped_Bip01");
		m_matPlayerFrame = &pFrameFound->CombinedMatrix;

		m_pEffectLayer = Engine::GetLayer(L"Layer_Effect");
		Engine::CGameObject* pTrail = CParticle_Trail::Create(m_pGraphicDev,m_pTransformCom->GetWorldMatrix(),&m_AttributeList.front().vPos,m_AttributeList.front().fLifeTime);
		m_pEffectLayer->Add_Object(pTrail);
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
	m_pTransformCom->SetWorldmat(matScale* matRot * /*matTrans **/ *m_matPlayerFrame * *m_pParentTransform->GetWorldMatrix());
	m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_ALPHA);

	//-------------------Point Light------------------
	/*D3DLIGHT9 tLightInfo = Engine::GetLightInfo(1);
	_matrix matSpirit = matTrans *  matScale* matRot* *m_matPlayerFrame * *m_pParentTransform->GetWorldMatrix();
	tLightInfo.Position = *(_vec3*)&matSpirit.m[3][0];
	Engine::Change_LightInfo(tLightInfo,1);*/
	/////////////////////////////////////////////
	return 0;
}


void CSpirit::Render_GameObject()
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD,&(*m_matPlayerRH * *m_pParentTransform->GetWorldMatrix()));
	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW,&matView);
	D3DXMatrixInverse(&matView,0,&matView);
	m_pShaderCom->GetEffect()->SetVector("g_vCamWorldPos", (_vec4*)&matView.m[3][0]);


	m_pShaderCom->SetMatrixOnShader(*m_pTransformCom->GetWorldMatrix());
	m_pShaderCom->GetEffect()->SetTexture("g_baseTexture",m_pTextureCom->GetTexture());
	m_pShaderCom->GetEffect()->SetFloat("g_fRandValue",m_fRand);
	m_pShaderCom->GetEffect()->SetFloat("g_fMaxSize",70.f);

	m_pShaderCom->GetEffect()->Begin(NULL,0);
	m_pShaderCom->GetEffect()->BeginPass(0);

	Engine::CParticleSystem::Render_GameObject();

	m_pShaderCom->GetEffect()->EndPass();
	m_pShaderCom->GetEffect()->End();

	////////////////////////////////////////////////////////////////

}


CSpirit* CSpirit::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSpirit* pInst = new CSpirit(pGraphicDev);
	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;

}

void CSpirit::Free()
{
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pParentTransform);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShaderCom);

	Engine::CParticleSystem::Free();
}

