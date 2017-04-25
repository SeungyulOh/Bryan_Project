#include "stdafx.h"
#include "ChargeParticle_LightGlow.h"
#include "Export_Engine.h"

CChargeParticle_LightGlow::CChargeParticle_LightGlow(LPDIRECT3DDEVICE9 pGraphicDev,eType MyType, _vec3* pOriginPos, _int iNumParticles, _float fDeadTime)
:Engine::CParticleSystem(pGraphicDev)
,m_pRendererCom(NULL)
,m_pTextureCom(NULL)
,m_pShaderCom(NULL)
,m_pParentTransform(NULL)
,m_isOnce(false)
,m_pEffectLayer(NULL)
,m_pTransformCom(NULL)
,m_pPlayerState(NULL)
{
	//m_vOrigin = *pOriginPos;
	m_vOrigin = _vec3(0.f,0.f,0.f);

	m_fSize = 1.f;
	m_dwVTXSize = 2048;
	m_dwVTXOffSet = 0;
	m_dwBatchSize = 512;
	m_fGlowSize = 2.f;
	m_fAlpha = 1.f;
	m_eMyType = MyType;
	m_fDeadTime = fDeadTime;

	CParticleSystem::Init(iNumParticles);

	for(int i =0; i<iNumParticles ; ++i)
		AddParticle();
}

CChargeParticle_LightGlow::~CChargeParticle_LightGlow()
{

}

HRESULT CChargeParticle_LightGlow::Ready_GameObject()
{

	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);

	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);

	m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_Particle_evy_icelight_small");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_Particle_evy_icelight_small",m_pTextureCom);

	m_pShaderCom = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Particle");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Particle",m_pShaderCom);


	m_pShaderCom->GetEffect()->SetFloat("g_fInputSize",m_fSize);

	D3DVIEWPORT9 tViewPort;
	m_pGraphicDev->GetViewport(&tViewPort);

	m_pShaderCom->GetEffect()->SetFloat("g_fViewPortY",(float)tViewPort.Height);
	

	return S_OK; 
}

void CChargeParticle_LightGlow::ResetParticle(Engine::PARICLEATT* pAttribute,_uint iIndex)
{
	pAttribute->isAlive = true;
	pAttribute->vPos = m_vOrigin + _vec3(_float(rand()%3 - 1.f),_float(rand()%3 - 1.f),_float(rand()%3 - 1.f));
	m_vBoundingMinMax[0] = _vec3(-1.f,-1.f,-1.f);
	m_vBoundingMinMax[1] = _vec3(1.f,1.f,1.f);

	Engine::CParticleSystem::GetRandomVector(&pAttribute->vVelocity,&m_vBoundingMinMax[0],&m_vBoundingMinMax[1]);
	D3DXVec3Normalize(&pAttribute->vVelocity,&pAttribute->vVelocity);
	pAttribute->vVelocity *= _float(rand() % 15 + 5);

	//pAttribute->vVelocity = _vec3(1.f,1.f,1.f);

	/*pAttribute->vColor = D3DXCOLOR(Engine::CParticleSystem::GetRandomFloat(0.f,1.f),
									Engine::CParticleSystem::GetRandomFloat(0.f,1.f),
									Engine::CParticleSystem::GetRandomFloat(0.f,1.f),
									1.f);*/

	float fBlue = (rand()%50 +185) / 255.f;
	float fGreen = (rand()%40 + 140) / 255.f;
	float fRed = (rand() % 90 + 100) / 255.f;
	float fAlpha = (rand() % 256) / 255.f;
	pAttribute->vColor = D3DXCOLOR(1.f,1.f,1.f,1.f);
	
	pAttribute->fAge = 0.0f;
	pAttribute->fLifeTime = (_float)(rand()%3) * 0.1f + 0.5f;

}


_int CChargeParticle_LightGlow::Update_GameObject(const _float& fTimeDelta)
{
	m_fGlowSize += fTimeDelta;
	if(m_fGlowSize > 2.f + m_fDeadTime)
		m_fAlpha -= fTimeDelta;
	if(m_fAlpha < 0.f)
		m_isDestroy = true;


	if(m_isOnce == false)
	{
		m_isOnce = true;

		m_pParentTransform = (Engine::CTransform*)Engine::GetList(L"Layer_Player")->front()->FindComponent(Engine::CGameObject::UPDATE,L"Component_Transform");
		m_pParentTransform->AddRefCnt();

		Engine::CDynamicMesh* pPlayerMesh = (Engine::CDynamicMesh*)Engine::GetList(L"Layer_Player")->front()->FindComponent(Engine::CGameObject::NONUPDATE,L"Component_Mesh_Hagie");

		Engine::D3DXFRAME_DERIVED* pFrameFound;
		if(m_eMyType == RIGHTHAND)
			pFrameFound = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(pPlayerMesh->GetRootFrame(),"ValveBiped_Bip01_R_Hand");
		else if(m_eMyType == LEFTHAND)
			pFrameFound = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(pPlayerMesh->GetRootFrame(),"ValveBiped_Bip01_L_Hand");
		
		m_matPlayerRH = &pFrameFound->CombinedMatrix;
		m_pEffectLayer = Engine::GetLayer(L"Layer_Effect");

		m_pPlayerState = ((CPlayer*)Engine::GetList(L"Layer_Player")->front())->GetAtkState();
	}

	list<Engine::PARICLEATT>::iterator iter = m_AttributeList.begin();
	list<Engine::PARICLEATT>::iterator iter_end = m_AttributeList.end();

	_uint iIndex = 0;
	for ( ; iter!=iter_end; ++iter)
	{
		if(iter->isAlive)
		{
			//AddParticle(*iter,iIndex);

			iter->fAge += fTimeDelta;
			iter->vPos += iter->vVelocity * fTimeDelta;//iter->fAge;

			if(iter->fAge > iter->fLifeTime)
				iter->isAlive = false;
		}
		else
			ResetParticle(&(*iter),iIndex);

		++iIndex;
	}

	m_pTransformCom->SetWorldmat(*m_matPlayerRH * *m_pParentTransform->GetWorldMatrix());
	m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_ALPHA);

	return 0;
}


void CChargeParticle_LightGlow::Render_GameObject()
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD,&(*m_matPlayerRH * *m_pParentTransform->GetWorldMatrix()));
	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW,&matView);
	D3DXMatrixInverse(&matView,0,&matView);
	m_pShaderCom->GetEffect()->SetVector("g_vCamWorldPos", (_vec4*)&matView.m[3][0]);

	m_pShaderCom->SetMatrixOnShader(*m_pTransformCom->GetWorldMatrix());

	if(*m_pPlayerState == CPlayer::LIGHTNING)
	{
		m_pShaderCom->GetEffect()->SetTexture("g_baseTexture",m_pTextureCom->GetTexture(0));
		m_pShaderCom->GetEffect()->SetFloat("g_fMaxSize",m_fGlowSize*20.f);
	}
	if(*m_pPlayerState == CPlayer::FIRE)
	{	
		m_pShaderCom->GetEffect()->SetTexture("g_baseTexture",m_pTextureCom->GetTexture(1));
		m_pShaderCom->GetEffect()->SetFloat("g_fMaxSize",m_fGlowSize*30.f);
	}
	

	m_pShaderCom->GetEffect()->SetFloat("g_fAlpha",m_fAlpha);
	
	m_pShaderCom->GetEffect()->Begin(NULL,0);
	m_pShaderCom->GetEffect()->BeginPass(0);

	Engine::CParticleSystem::Render_GameObject();

	m_pShaderCom->GetEffect()->EndPass();
	m_pShaderCom->GetEffect()->End();

}


CChargeParticle_LightGlow* CChargeParticle_LightGlow::Create(LPDIRECT3DDEVICE9 pGraphicDev,eType MyType,_vec3* pOriginPos, _int iNumParticles, _float fDeadTime)
{
	CChargeParticle_LightGlow* pInst = new CChargeParticle_LightGlow(pGraphicDev,MyType,pOriginPos,iNumParticles,fDeadTime);
	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;

}

void CChargeParticle_LightGlow::Free()
{
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pParentTransform);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShaderCom);

	Engine::CParticleSystem::Free();
}

