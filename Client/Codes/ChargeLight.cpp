#include "stdafx.h"
#include "ChargeLight.h"
#include "Export_Engine.h"

CChargeLight::CChargeLight(LPDIRECT3DDEVICE9 pGraphicDev,eType MyType, _vec3* pOriginPos, _int iNumParticles, _float fDeadTime, _int iImgIdx)
:Engine::CParticleSystem(pGraphicDev)
,m_pRendererCom(NULL)
,m_pTextureCom(NULL)
,m_pShaderCom(NULL)
,m_pParentTransform(NULL)
,m_isOnce(false)
,m_pEffectLayer(NULL)
,m_pTransformCom(NULL)
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
	m_iImgIdx = iImgIdx;

	CParticleSystem::Init(iNumParticles);

	for(int i =0; i<iNumParticles ; ++i)
		AddParticle();
}

CChargeLight::~CChargeLight()
{

}

HRESULT CChargeLight::Ready_GameObject()
{

	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);

	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);

	m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_bomb");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_bomb",m_pTextureCom);

	m_pShaderCom = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Particle");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Particle",m_pShaderCom);


	m_pShaderCom->GetEffect()->SetFloat("g_fInputSize",m_fSize);

	D3DVIEWPORT9 tViewPort;
	m_pGraphicDev->GetViewport(&tViewPort);

	m_pShaderCom->GetEffect()->SetFloat("g_fViewPortY",(float)tViewPort.Height);
	

	return S_OK; 
}

void CChargeLight::ResetParticle(Engine::PARICLEATT* pAttribute,_uint iIndex)
{
	pAttribute->isAlive = true;
	pAttribute->vPos = m_vOrigin + _vec3(_float(rand()%3 - 1.f),_float(rand()%3 - 1.f),_float(rand()%3 - 1.f));
	m_vBoundingMinMax[0] = _vec3(-1.f,-1.f,-1.f);
	m_vBoundingMinMax[1] = _vec3(1.f,1.f,1.f);

	Engine::CParticleSystem::GetRandomVector(&pAttribute->vVelocity,&m_vBoundingMinMax[0],&m_vBoundingMinMax[1]);
	D3DXVec3Normalize(&pAttribute->vVelocity,&pAttribute->vVelocity);
	pAttribute->vVelocity *= _float(rand() % 15 + 5);


	float fBlue = (rand()%50 +185) / 255.f;
	float fGreen = (rand()%40 + 140) / 255.f;
	float fRed = (rand() % 90 + 100) / 255.f;
	//pAttribute->vColor = D3DXCOLOR(fRed,fGreen,fBlue,1.f);
	pAttribute->vColor = D3DXCOLOR(1,1,1,1.f);
	
	pAttribute->fAge = 0.0f;
	pAttribute->fLifeTime = (_float)(rand()%3) * 0.1f + 0.5f;

}


_int CChargeLight::Update_GameObject(const _float& fTimeDelta)
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


void CChargeLight::Render_GameObject()
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD,&(*m_matPlayerRH * *m_pParentTransform->GetWorldMatrix()));
	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW,&matView);
	D3DXMatrixInverse(&matView,0,&matView);
	m_pShaderCom->GetEffect()->SetVector("g_vCamWorldPos", (_vec4*)&matView.m[3][0]);

	m_pShaderCom->SetMatrixOnShader(*m_pTransformCom->GetWorldMatrix());
	m_pShaderCom->GetEffect()->SetTexture("g_baseTexture",m_pTextureCom->GetTexture(m_iImgIdx));
	m_pShaderCom->GetEffect()->SetFloat("g_fMaxSize",m_fGlowSize*20.f);
	m_pShaderCom->GetEffect()->SetFloat("g_fAlpha",m_fAlpha);
	
	m_pShaderCom->GetEffect()->Begin(NULL,0);
	m_pShaderCom->GetEffect()->BeginPass(1);

	Engine::CParticleSystem::Render_GameObject();

	m_pShaderCom->GetEffect()->EndPass();
	m_pShaderCom->GetEffect()->End();
}


CChargeLight* CChargeLight::Create(LPDIRECT3DDEVICE9 pGraphicDev,eType MyType,_vec3* pOriginPos, _int iNumParticles, _float fDeadTime ,  _int iImgIdx)
{
	CChargeLight* pInst = new CChargeLight(pGraphicDev,MyType,pOriginPos,iNumParticles,fDeadTime,iImgIdx);
	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;

}

void CChargeLight::Free()
{
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pParentTransform);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShaderCom);

	Engine::CParticleSystem::Free();
}

