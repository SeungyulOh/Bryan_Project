#include "stdafx.h"
#include "StarGlow.h"
#include "Export_Engine.h"

CStarGlow::CStarGlow(LPDIRECT3DDEVICE9 pGraphicDev,  _matrix* matParent, COLORTYPE eType, Engine::CDynamicMesh* pDynamicMesh)
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
,m_matParent(matParent)
,m_eType(eType)
{
	m_vOrigin = _vec3(0.f,0.f,0.f);
	int iNumParticles = 10;

	m_fSize = 20.f;
	m_dwVTXSize = 2048;
	m_dwVTXOffSet = 0;
	m_dwBatchSize = 512;

	Engine::D3DXFRAME_DERIVED* pFrameFound = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(pDynamicMesh->GetRootFrame(),"lamp_light"); 
	m_matFrame = &pFrameFound->CombinedMatrix;

	CParticleSystem::Init(iNumParticles);

	for(int i =0; i<iNumParticles ; ++i)
		AddParticle();

	m_fViewZ = 0.1f;

}

CStarGlow::~CStarGlow()
{

}

HRESULT CStarGlow::Ready_GameObject()
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

void CStarGlow::ResetParticle(Engine::PARICLEATT* pAttribute, _uint iIndex)
{
	pAttribute->isAlive = true;
	m_iImgIdx = 10;

	_matrix matWorld = *m_matFrame * *m_matParent;
	pAttribute->vPos = *(_vec3*)&matWorld.m[3][0];
	
	m_vBoundingMinMax[0] = _vec3(-1.f,-1.f,-1.f);
	m_vBoundingMinMax[1] = _vec3(1.f,1.f,1.f);
	Engine::CParticleSystem::GetRandomVector(&pAttribute->vVelocity,&m_vBoundingMinMax[0],&m_vBoundingMinMax[1]);
	D3DXVec3Normalize(&pAttribute->vVelocity,&pAttribute->vVelocity);
	pAttribute->vVelocity *= 2.f;

	float fBlue =min((rand()%255) / 255.f , 0.5f);
	float fGreen = min((rand()%255) / 255.f, 0.5f);
	float fRed = min((rand() % 255) / 255.f, 0.5f);
	float fAlpha = 1.f;//(rand() % 256) / 255.f;

	if(m_eType == RED)
		pAttribute->vColor = D3DXCOLOR(1.f,fGreen*0.2f,fBlue*0.2f,1.f);
	else if(m_eType == GREEN)
		pAttribute->vColor = D3DXCOLOR(fRed*0.2f,1.f,fBlue*0.2f,1.f);
	else if(m_eType == BLUE)
		pAttribute->vColor = D3DXCOLOR(fRed*0.2f,fGreen*0.2f,1.f,1.f);
	
	pAttribute->fLifeTime = (_float)(rand()%3) * 0.1f + 0.3f;
	pAttribute->fAge = 0.f;
}

_int CStarGlow::Update_GameObject(const _float& fTimeDelta)
{
	if(m_isOnce == false)
	{
		m_isOnce = true;
	}

	m_fTimeAcc += fTimeDelta;
	
	list<Engine::PARICLEATT>::iterator iter = m_AttributeList.begin();
	list<Engine::PARICLEATT>::iterator iter_end = m_AttributeList.end();

	_uint iIndex = 0;
	for ( ; iter!=iter_end; ++iter)
	{
		if(iter->isAlive)
		{
			//AddParticle(*iter,iIndex);

			iter->fAge += fTimeDelta;
			//iter->vPos = m_vOrigin + iter->vVelocity *2.f;//iter->fAge;
			iter->vPos += iter->vVelocity * fTimeDelta;

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


void CStarGlow::Render_GameObject()
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
	m_pShaderCom->GetEffect()->SetFloat("g_fAlpha",1.f);

	m_pShaderCom->GetEffect()->Begin(NULL,0);
	m_pShaderCom->GetEffect()->BeginPass(2);

	Engine::CParticleSystem::Render_GameObject();

	m_pShaderCom->GetEffect()->EndPass();
	m_pShaderCom->GetEffect()->End();

	//TCHAR szTemp[256] = L"%d";
//	wsprintf(szTemp,L"Alpha : %d",_uint(m_fTimeAcc *100));
	//Engine::Render_Font(L"Font_Batang",szTemp,&_vec2(0.f,300.f),D3DXCOLOR(1.f,1.f,1.f,1.f));

}


CStarGlow* CStarGlow::Create(LPDIRECT3DDEVICE9 pGraphicDev, _matrix* matParent, COLORTYPE eType, Engine::CDynamicMesh* pDynamicMesh)
{
	CStarGlow* pInst = new CStarGlow(pGraphicDev,matParent,eType,pDynamicMesh);
	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;

}

void CStarGlow::Free()
{
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pParentTransform);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShaderCom);

	Engine::CParticleSystem::Free();
}

