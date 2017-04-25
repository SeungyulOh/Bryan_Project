#include "stdafx.h"
#include "EyeFollowParticle.h"
#include "Export_Engine.h"

CEyeFollowParticle::CEyeFollowParticle(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pOriginPos, _int iNumParticles, Engine::CTransform* pParentTransform,_matrix* pEyeMat,_int* iImgIdx,MYTYPE eType)
:Engine::CParticleSystem(pGraphicDev)
,m_pRendererCom(NULL)
,m_pTextureCom(NULL)
,m_pShaderCom(NULL)
,m_pParentTransform(pParentTransform)
,m_matEyeBone(pEyeMat)
,m_isOnce(false)
,m_iImgIdx(iImgIdx)
,m_eType(eType)
,m_pBossVisible(NULL)
{
	m_vOrigin = *pOriginPos;

	m_fSize = 0.8f;
	m_dwVTXSize = 2048;
	m_dwVTXOffSet = 0;
	m_dwBatchSize = 512;

	m_pParentTransform->AddRefCnt();
	
	CParticleSystem::Init(iNumParticles);

	for(int i =0; i<iNumParticles ; ++i)
		AddParticle();

	m_fAlpha = 1.f;
	m_fTimeAcc = 0.f;


}

CEyeFollowParticle::~CEyeFollowParticle()
{

}

HRESULT CEyeFollowParticle::Ready_GameObject()
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);

	m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_Particle_Glow");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_Particle_Glow",m_pTextureCom);

	m_pShaderCom = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Particle");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Particle",m_pShaderCom);


	CBossDevil* pBoss = dynamic_cast<CBossDevil*>(Engine::GetList(L"Layer_Boss")->front());
	if(pBoss != NULL)
		m_pBossVisible = pBoss->GetVisible();

	return S_OK; 
}

void CEyeFollowParticle::ResetParticle(Engine::PARICLEATT* pAttribute,_uint iIndex)
{
	pAttribute->isAlive = true;
	_matrix matWorld;
	if(*m_iImgIdx == 11 && m_eType == BASE)
		matWorld = *m_matEyeBone* *m_pParentTransform->GetWorldMatrix();
	else
	{
		_vec3 vOffSet;
		if(m_eType == RIGHT)
			vOffSet = *D3DXVec3Normalize(&_vec3(),(_vec3*)&m_matEyeBone->m[2][0]) * 10.f + *D3DXVec3Normalize(&_vec3(),(_vec3*)&m_matEyeBone->m[0][0]) * 2.5f + *D3DXVec3Normalize(&_vec3(),(_vec3*)&m_matEyeBone->m[1][0]) * -5.f;
		else
			vOffSet = *D3DXVec3Normalize(&_vec3(),(_vec3*)&m_matEyeBone->m[2][0]) * 10.f + *D3DXVec3Normalize(&_vec3(),(_vec3*)&m_matEyeBone->m[0][0]) * -2.5f + *D3DXVec3Normalize(&_vec3(),(_vec3*)&m_matEyeBone->m[1][0]) * -5.f;


		_matrix matTrans,matScale,matRot;
		D3DXMatrixTranslation(&matTrans,vOffSet.x,vOffSet.y,vOffSet.z);
		D3DXMatrixScaling(&matScale,1.f,1.f,1.f);
		D3DXMatrixRotationY(&matRot,D3DX_PI/2.f);
		matWorld = matScale* matRot * matTrans * *m_matEyeBone * *m_pParentTransform->GetWorldMatrix();
	}
		

	pAttribute->vPos = *(_vec3*)&matWorld.m[3][0];
	
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



	pAttribute->vColor=D3DXCOLOR(1.f,1.f,1.f,1.f);


	
	pAttribute->fAge = 0.0f;
	pAttribute->fLifeTime = (_float)(rand()%3) * 0.1f + 0.3f;

}


_int CEyeFollowParticle::Update_GameObject(const _float& fTimeDelta)
{

	if(m_isDestroy == true)
		return 0;

	/*if(m_eTarget != SPIRIT)
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
	*/

	if(m_isOnce == false)
	{
		m_isOnce = true;
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

	if(m_isDestroy == false && m_eType == BASE && Engine::GetList(L"Layer_Boss")->size() == 2 && *m_pBossVisible == true)
		m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_ALPHA);
	else if(m_eType != BASE)
		m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_ALPHA);

	return 0;
}


void CEyeFollowParticle::Render_GameObject()
{
	if(m_isDestroy == true)
		return;

	//m_pShaderCom->SetMatrixOnShader(*m_matPlayerRH *  *m_pParentTransform->GetWorldMatrix());
	_matrix matLocal;
	D3DXMatrixTranslation(&matLocal,50.f,50.f,0.f);
	_matrix matWorld;
	matWorld = *m_matEyeBone * *m_pParentTransform->GetWorldMatrix();
	
	m_pShaderCom->SetMatrixOnShader(matWorld);


	m_pShaderCom->GetEffect()->SetTexture("g_baseTexture",m_pTextureCom->GetTexture(*m_iImgIdx + 1));
	

	float fRand;
	fRand = rand()%20 + 25.f;
	m_pShaderCom->GetEffect()->SetFloat("g_fMaxSize",fRand);
	m_pShaderCom->GetEffect()->SetFloat("g_fAlpha", m_fAlpha);
	m_pShaderCom->GetEffect()->SetFloat("g_fRandValue",0.f);

	m_pShaderCom->GetEffect()->Begin(NULL,0);
	m_pShaderCom->GetEffect()->BeginPass(2);

	Engine::CParticleSystem::Render_GameObject();

	m_pShaderCom->GetEffect()->EndPass();
	m_pShaderCom->GetEffect()->End();
}



CEyeFollowParticle* CEyeFollowParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev,_vec3* pOriginPos, _int iNumParticles,Engine::CTransform* pParentTransform, _matrix* pEyeMat,_int* iImgIdx,MYTYPE eType )
{
	CEyeFollowParticle* pInst = new CEyeFollowParticle(pGraphicDev,pOriginPos,iNumParticles,pParentTransform,pEyeMat,iImgIdx,eType);
	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;

}

void CEyeFollowParticle::Free()
{
	Engine::Safe_Release(m_pParentTransform);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShaderCom);

	Engine::CParticleSystem::Free();
}

