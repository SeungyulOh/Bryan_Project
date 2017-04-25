#include "stdafx.h"
#include "BossTeleBegin.h"
#include "Export_Engine.h"

CBossTeleBegin::CBossTeleBegin(LPDIRECT3DDEVICE9 pGraphicDev,_int iNumParticles)
:Engine::CParticleSystem(pGraphicDev)
,m_pRendererCom(NULL)
,m_pTextureCom(NULL)
,m_pShaderCom(NULL)
,m_pParentTransform(NULL)
,m_isOnce(false)
,m_pBossWorldMat(NULL)
{

	m_fSize = 0.8f;
	m_dwVTXSize = 2048;
	m_dwVTXOffSet = 0;
	m_dwBatchSize = 512;

	Engine::CTransform* pBossTransform = (Engine::CTransform*)Engine::GetList(L"Layer_Boss")->front()->FindComponent(Engine::CGameObject::UPDATE,L"Component_Transform");
	m_pBossWorldMat = pBossTransform->GetWorldMatrix();

	CParticleSystem::Init(iNumParticles);

	for(int i =0; i<iNumParticles ; ++i)
		AddParticle();

	m_fAlpha = 1.f;
	m_fTimeAcc = 0.f;


}

CBossTeleBegin::~CBossTeleBegin()
{

}

HRESULT CBossTeleBegin::Ready_GameObject()
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);

	m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_KadanTeleport");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_KadanTeleport",m_pTextureCom);

	m_pShaderCom = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Particle");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Particle",m_pShaderCom);


	

	return S_OK; 
}

void CBossTeleBegin::ResetParticle(Engine::PARICLEATT* pAttribute,_uint iIndex)
{
	pAttribute->isAlive = true;

	//pAttribute->vPos = _vec3(0.f,5.f,0.f);
	
	m_vBoundingMinMax[0] = _vec3(-1.f,-1.f,-1.f);
	m_vBoundingMinMax[1] = _vec3(1.f,1.f,1.f);

	Engine::CParticleSystem::GetRandomVector(&pAttribute->vPos,&m_vBoundingMinMax[0],&m_vBoundingMinMax[1]);
	Engine::CParticleSystem::GetRandomVector(&pAttribute->vVelocity,&m_vBoundingMinMax[0],&m_vBoundingMinMax[1]);
	pAttribute->vPos += *(_vec3*)&m_pBossWorldMat->m[3][0] + _vec3(0.f,5.f,0.f);

	D3DXVec3Normalize(&pAttribute->vVelocity,&pAttribute->vVelocity);

	pAttribute->vVelocity *= 0.f;

	/*pAttribute->vColor = D3DXCOLOR(Engine::CParticleSystem::GetRandomFloat(0.f,1.f),
									Engine::CParticleSystem::GetRandomFloat(0.f,1.f),
									Engine::CParticleSystem::GetRandomFloat(0.f,1.f),
									1.f);*/

	float fBlue =(rand()%255) / 255.f;
	float fGreen = (rand()%255) / 255.f;
	float fRed = (rand() % 255) / 255.f;
	float fAlpha = (rand() % 256) / 255.f;

	pAttribute->vColor = D3DXCOLOR(0.f,0.f,0.f,1.f);
	
	
	pAttribute->fAge = 0.0f;
	pAttribute->fLifeTime = (_float)(rand()%3) * 0.1f + 0.3f;

}


_int CBossTeleBegin::Update_GameObject(const _float& fTimeDelta)
{

	m_fTimeAcc += fTimeDelta;
	if(m_fTimeAcc > 0.1f)
		m_fAlpha -= fTimeDelta;

	if(m_fAlpha < 0.f)
	{
		m_isDestroy = true;
		return 0;
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


void CBossTeleBegin::Render_GameObject()
{

	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);
	m_pShaderCom->SetMatrixOnShader(matWorld);
	m_pShaderCom->GetEffect()->SetTexture("g_baseTexture",m_pTextureCom->GetTexture(0));
	m_pShaderCom->GetEffect()->SetTexture("g_secondTexture",m_pTextureCom->GetTexture(1));
	m_pShaderCom->GetEffect()->SetTexture("g_thirdTexture",m_pTextureCom->GetTexture(2));
	m_pShaderCom->GetEffect()->SetBool("g_isZbufferEnable",true);
	
	_float fRand = rand()%20 + 35.f;
	m_pShaderCom->GetEffect()->SetFloat("g_fMaxSize",fRand);
	m_pShaderCom->GetEffect()->SetFloat("g_fAlpha", 1.f);
	m_pShaderCom->GetEffect()->SetFloat("g_fRandValue",0.f);

	m_pShaderCom->GetEffect()->Begin(NULL,0);
	m_pShaderCom->GetEffect()->BeginPass(4);

	Engine::CParticleSystem::Render_GameObject();

	m_pShaderCom->GetEffect()->EndPass();
	m_pShaderCom->GetEffect()->End();
}



CBossTeleBegin* CBossTeleBegin::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iNumParticles)
{
	CBossTeleBegin* pInst = new CBossTeleBegin(pGraphicDev,iNumParticles);
	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;

}

void CBossTeleBegin::Free()
{
	Engine::Safe_Release(m_pParentTransform);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShaderCom);

	Engine::CParticleSystem::Free();
}

