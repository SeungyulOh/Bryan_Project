#include "stdafx.h"
#include "TrailParticle.h"
#include "Export_Engine.h"
#include "Phantom_Trail.h"

CTrailParticle::CTrailParticle(LPDIRECT3DDEVICE9 pGraphicDev, _int iNumParticles,  _int iImgIdx)
:Engine::CParticleSystem(pGraphicDev)
,m_pRendererCom(NULL)
,m_pTextureCom(NULL)
,m_pShaderCom(NULL)
,m_pParentTransform(NULL)
,m_isOnce(false)
,m_iImgIdx(iImgIdx)
{
	m_vOrigin = _vec3(0.f,0.f,0.f);
	m_fTimeAcc = 0.f;

	m_fSize = 0.8f;
	m_dwVTXSize = 2048;
	m_dwVTXOffSet = 0;
	m_dwBatchSize = 512;

	m_pParentTransform = (Engine::CTransform*)Engine::GetList(L"Layer_Player")->front()->FindComponent(Engine::CGameObject::UPDATE,L"Component_Transform");
	m_pParentTransform->AddRefCnt();

	CParticleSystem::Init(iNumParticles);

	for(int i =0; i<iNumParticles ; ++i)
		AddParticle();

	m_fAlpha = 0.f;
	m_fSpeed = 1.f;
}

CTrailParticle::~CTrailParticle()
{

}

HRESULT CTrailParticle::Ready_GameObject()
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);

	m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_bomb");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_bomb",m_pTextureCom);

	m_pShaderCom = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Particle");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Particle",m_pShaderCom);


	return S_OK; 
}

void CTrailParticle::ResetParticle(Engine::PARICLEATT* pAttribute,_uint iIndex)
{
	pAttribute->isAlive = true;
	m_fSpeed = rand()%3 + 1.f;
	_float fDir = rand()%3 - 1.f;
	
	_float fRandRadius = rand()%5 + 1.f;
	_float fRandAngle = D3DXToRadian(_float(rand()%360));
	_vec3 vOffSet = _vec3(fRandRadius,2.5f,0.f);
	_matrix matRot;
	D3DXMatrixRotationY(&matRot,fRandAngle);
	D3DXVec3TransformCoord(&vOffSet,&vOffSet,&matRot);

	pAttribute->vPos = *(_vec3*)&m_pParentTransform->GetWorldMatrix()->m[3][0] + vOffSet  - _vec3(m_fTimeAcc * fDir,m_fTimeAcc*m_fSpeed,m_fTimeAcc * fDir);
	
	m_vBoundingMinMax[0] = _vec3(-1.f,-1.f,-1.f);
	m_vBoundingMinMax[1] = _vec3(1.f,1.f,1.f);

	Engine::CParticleSystem::GetRandomVector(&pAttribute->vVelocity,&m_vBoundingMinMax[0],&m_vBoundingMinMax[1]);

	D3DXVec3Normalize(&pAttribute->vVelocity,&pAttribute->vVelocity);

	pAttribute->vVelocity *= 1.f;

	float fBlue = 1.f;
	float fGreen = (rand()%255) / 255.f;
	float fRed = (rand() % 255) / 255.f;
	float fAlpha = (rand() % 256) / 255.f;
	pAttribute->vColor = D3DXCOLOR(1,1,1,1.f);
	
	pAttribute->fAge = 0.0f;
	pAttribute->fLifeTime = (_float)(rand()%3) * 0.1f;

	pAttribute->fViewZ = (_float)(rand()%6);


}


_int CTrailParticle::Update_GameObject(const _float& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	if(m_fTimeAcc > 1.f)
		m_fAlpha -= fTimeDelta;

	if(m_fAlpha < 0.f)
		m_isDestroy = true;


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


void CTrailParticle::Render_GameObject()
{

	m_pShaderCom->SetMatrixOnShader();
	m_pShaderCom->GetEffect()->SetTexture("g_baseTexture",m_pTextureCom->GetTexture(m_iImgIdx));
	m_pShaderCom->GetEffect()->SetBool("g_isZbufferEnable",true);
	
	float fRand = rand()%10+10.f;
	m_pShaderCom->GetEffect()->SetFloat("g_fMaxSize",fRand);
	m_pShaderCom->GetEffect()->SetFloat("g_fAlpha", 1.f);
	m_pShaderCom->GetEffect()->SetFloat("g_fRandValue",0.f);

	m_pShaderCom->GetEffect()->Begin(NULL,0);
	m_pShaderCom->GetEffect()->BeginPass(2);

	Engine::CParticleSystem::Render_GameObject();

	m_pShaderCom->GetEffect()->EndPass();
	m_pShaderCom->GetEffect()->End();
}



CTrailParticle* CTrailParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iNumParticles ,  _int iImgIdx)
{
	CTrailParticle* pInst = new CTrailParticle(pGraphicDev,iNumParticles,iImgIdx);
	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;

}

void CTrailParticle::Free()
{
	Engine::Safe_Release(m_pParentTransform);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShaderCom);

	Engine::CParticleSystem::Free();
}

