#include "stdafx.h"
#include "ChargeParticle_LightRay.h"
#include "Export_Engine.h"

CChargeParticle_LightRay::CChargeParticle_LightRay(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pOriginPos, _int iNumParticles,TARGET eTarget)
:Engine::CParticleSystem(pGraphicDev)
,m_pRendererCom(NULL)
,m_pTextureCom(NULL)
,m_pShaderCom(NULL)
,m_pParentTransform(NULL)
,m_isOnce(false)
,m_matPlayerRH(NULL)
,m_pPlayerState(NULL)
{
	m_vOrigin = *pOriginPos;

	m_fSize = 0.8f;
	m_dwVTXSize = 2048;
	m_dwVTXOffSet = 0;
	m_dwBatchSize = 512;

	CParticleSystem::Init(iNumParticles);

	for(int i =0; i<iNumParticles ; ++i)
		AddParticle();

	m_eTarget = eTarget;
	m_fAlpha = 1.f;
	m_fTimeAcc = 0.f;


}

CChargeParticle_LightRay::~CChargeParticle_LightRay()
{

}

HRESULT CChargeParticle_LightRay::Ready_GameObject()
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);

	m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_flare");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_flare",m_pTextureCom);

	m_pShaderCom = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Particle");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Particle",m_pShaderCom);


	

	return S_OK; 
}

void CChargeParticle_LightRay::ResetParticle(Engine::PARICLEATT* pAttribute,_uint iIndex)
{
	pAttribute->isAlive = true;

	if(m_matPlayerRH != NULL && m_pParentTransform != NULL)
	{
		_matrix matLocal;
		D3DXMatrixTranslation(&matLocal,50.f,50.f,0.f);
		_matrix matWorld;
		if(m_eTarget == SPIRIT)
			matWorld = matLocal* *m_matSpiritWorld;
		else if(m_eTarget == FOOTL)
			matWorld = *m_PlayerFootL* *m_pParentTransform->GetWorldMatrix();
		else if(m_eTarget == FOOTR)
			matWorld = *m_PlayerFootR* *m_pParentTransform->GetWorldMatrix();
		
		pAttribute->vPos = *(_vec3*)&matWorld.m[3][0];
	}
	else
		pAttribute->vPos = m_vOrigin;
	
	m_vBoundingMinMax[0] = _vec3(-1.f,-1.f,-1.f);
	m_vBoundingMinMax[1] = _vec3(1.f,1.f,1.f);

	Engine::CParticleSystem::GetRandomVector(&pAttribute->vVelocity,&m_vBoundingMinMax[0],&m_vBoundingMinMax[1]);

	D3DXVec3Normalize(&pAttribute->vVelocity,&pAttribute->vVelocity);

	pAttribute->vVelocity *= 1.f;

	/*pAttribute->vColor = D3DXCOLOR(Engine::CParticleSystem::GetRandomFloat(0.f,1.f),
									Engine::CParticleSystem::GetRandomFloat(0.f,1.f),
									Engine::CParticleSystem::GetRandomFloat(0.f,1.f),
									1.f);*/

	float fBlue =(rand()%255) / 255.f;
	float fGreen = (rand()%255) / 255.f;
	float fRed = (rand() % 255) / 255.f;
	float fAlpha = (rand() % 256) / 255.f;



	if(m_pPlayerState == NULL)
		pAttribute->vColor = D3DXCOLOR(fRed,fGreen,fBlue,1.f);
	else if(*m_pPlayerState == CPlayer::ICE)
		pAttribute->vColor = D3DXCOLOR(fRed/2.f,fGreen,1.f,1.f);
	else if(*m_pPlayerState == CPlayer::FIRE)
		pAttribute->vColor = D3DXCOLOR(1.f,fGreen,fBlue/2.f,1.f);
	else if(*m_pPlayerState == CPlayer::LIGHTNING)
	{
		pAttribute->vColor = D3DXCOLOR(fRed,fGreen,fBlue,1.f);
		pAttribute->vColor.r = max(pAttribute->vColor.r,0.7f);
		pAttribute->vColor.g = max(pAttribute->vColor.g,0.7f);
		pAttribute->vColor.b = max(pAttribute->vColor.b,0.7f);
	}


	
	pAttribute->fAge = 0.0f;
	if(m_eTarget == SPIRIT)
		pAttribute->fLifeTime = (_float)(rand()%3) * 0.1f + 0.3f;
	else
		pAttribute->fLifeTime = (_float)(rand()%3) * 0.1f + 0.3f;

	pAttribute->fViewZ = (_float)(rand()%6);


}


_int CChargeParticle_LightRay::Update_GameObject(const _float& fTimeDelta)
{

	if(m_eTarget != SPIRIT)
	{
		m_fTimeAcc += fTimeDelta;
		if(m_fTimeAcc > 1.f)
			m_fAlpha -= fTimeDelta;

		if(m_fAlpha < 0.f)
		{
			m_isDestroy = true;
			return 0;
		}
	}
	

	if(m_isOnce == false)
	{
		m_isOnce = true;

		m_pParentTransform = (Engine::CTransform*)Engine::GetList(L"Layer_Player")->front()->FindComponent(Engine::CGameObject::UPDATE,L"Component_Transform");
		m_pParentTransform->AddRefCnt();

		Engine::CDynamicMesh* pPlayerMesh = (Engine::CDynamicMesh*)Engine::GetList(L"Layer_Player")->front()->FindComponent(Engine::CGameObject::NONUPDATE,L"Component_Mesh_Hagie");
		Engine::D3DXFRAME_DERIVED* pFrameFound = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(pPlayerMesh->GetRootFrame(),"ValveBiped_Anim_Attachment_RH");
		m_matPlayerRH = &pFrameFound->CombinedMatrix;

		Engine::CTransform* pSpritTransform = (Engine::CTransform*)Engine::GetList(L"Layer_Effect")->front()->FindComponent(Engine::CGameObject::UPDATE,L"Component_Transform");
		m_matSpiritWorld = pSpritTransform->GetWorldMatrix();

		m_pPlayerState = ((CPlayer*)Engine::GetList(L"Layer_Player")->front())->GetAtkState();

		pFrameFound = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(pPlayerMesh->GetRootFrame(),"ValveBiped_Bip01_L_Foot");
		m_PlayerFootL = &pFrameFound->CombinedMatrix;

		pFrameFound = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(pPlayerMesh->GetRootFrame(),"ValveBiped_Bip01_R_Foot");
		m_PlayerFootR = &pFrameFound->CombinedMatrix;
		 
	}

	list<Engine::PARICLEATT>::iterator iter = m_AttributeList.begin();
	list<Engine::PARICLEATT>::iterator iter_end = m_AttributeList.end();

	_uint iIndex = 0;
	for ( ; iter!=iter_end; ++iter)
	{
		if(iter->isAlive)
		{
			iter->vPos += iter->vVelocity * fTimeDelta;

			iter->fAge += fTimeDelta;

			if(iter->fAge > iter->fLifeTime)
				iter->isAlive = false;
		}
		else
			ResetParticle(&(*iter),iIndex);

		++iIndex;
	}

	m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_ALPHA);

	return 0;
}


void CChargeParticle_LightRay::Render_GameObject()
{

	//m_pShaderCom->SetMatrixOnShader(*m_matPlayerRH *  *m_pParentTransform->GetWorldMatrix());
	_matrix matLocal;
	D3DXMatrixTranslation(&matLocal,50.f,50.f,0.f);
	_matrix matWorld;
	if(m_eTarget == SPIRIT)
		matWorld= *m_matSpiritWorld;
	else if(m_eTarget == FOOTL)
		matWorld= *m_pParentTransform->GetWorldMatrix();
	else if(m_eTarget == FOOTR)
		matWorld= *m_pParentTransform->GetWorldMatrix();
	
	m_pShaderCom->SetMatrixOnShader(matWorld);
	m_pShaderCom->GetEffect()->SetTexture("g_baseTexture",m_pTextureCom->GetTexture(4));
	m_pShaderCom->GetEffect()->SetBool("g_isZbufferEnable",true);
	
	float fRand;
	if(m_eTarget == SPIRIT)
		fRand= rand()%10+20.f;
	else
		fRand = rand()%20 + 25.f;
	m_pShaderCom->GetEffect()->SetFloat("g_fMaxSize",fRand);
	m_pShaderCom->GetEffect()->SetFloat("g_fAlpha", 1.f);
	m_pShaderCom->GetEffect()->SetFloat("g_fRandValue",0.f);

	m_pShaderCom->GetEffect()->Begin(NULL,0);
	m_pShaderCom->GetEffect()->BeginPass(2);

	Engine::CParticleSystem::Render_GameObject();

	m_pShaderCom->GetEffect()->EndPass();
	m_pShaderCom->GetEffect()->End();
}



CChargeParticle_LightRay* CChargeParticle_LightRay::Create(LPDIRECT3DDEVICE9 pGraphicDev,_vec3* pOriginPos, _int iNumParticles,TARGET eTarget)
{
	CChargeParticle_LightRay* pInst = new CChargeParticle_LightRay(pGraphicDev,pOriginPos,iNumParticles,eTarget);
	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;

}

void CChargeParticle_LightRay::Free()
{
	Engine::Safe_Release(m_pParentTransform);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShaderCom);

	Engine::CParticleSystem::Free();
}

