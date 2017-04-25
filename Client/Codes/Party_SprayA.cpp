#include "stdafx.h"
#include "Party_SprayA.h"
#include "Export_Engine.h"

CParty_SprayA::CParty_SprayA(LPDIRECT3DDEVICE9 pGraphicDev,_int iNumParticles)
:Engine::CParticleSystem(pGraphicDev)
,m_pRendererCom(NULL)
,m_pTextureCom(NULL)
,m_pShaderCom(NULL)
,m_pParentTransform(NULL)
,m_isOnce(false)
,m_pTransformCom(NULL)
,m_fTimeAcc(0.f)
{
	m_vOrigin = _vec3(0.f,0.f,0.f);
	/*_matrix matRot;
	D3DXMatrixRotationY(&matRot,D3DX_PI/2.f);
	D3DXVec3TransformCoord(&m_vOrigin,&m_vOrigin,&matRot);*/

	m_iMaxParticles = iNumParticles;
	m_fSize = 1000.f;
	m_dwVTXSize = 2048; 
	m_dwVTXOffSet = 0;
	m_dwBatchSize = 512;

	CParticleSystem::Init(m_iMaxParticles);

	for(_uint i =0; i<m_iMaxParticles ; ++i)
		AddParticle(i);
}

CParty_SprayA::~CParty_SprayA()
{

}

HRESULT CParty_SprayA::Ready_GameObject()
{

	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);

	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);

	m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_Particle_dust_2_white");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_Particle_Spray_A",m_pTextureCom);

	m_pShaderCom = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Particle");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Particle",m_pShaderCom);

	m_pShaderCom->GetEffect()->SetFloat("g_fInputSize",m_fSize);

	D3DVIEWPORT9 tViewPort;
	m_pGraphicDev->GetViewport(&tViewPort);
	m_pShaderCom->GetEffect()->SetFloat("g_fViewPortY",(float)tViewPort.Height);
	

	return S_OK; 
}

void CParty_SprayA::ResetParticle(Engine::PARICLEATT* pAttribute, _uint iIdx)
{
	pAttribute->isAlive = true;

	_float iRadius = 100.f;
	_vec3 vDir = _vec3(0.f,0.f,1.f);
	_matrix matRot;
	D3DXMatrixRotationY(&matRot,((D3DX_PI*2.f)/m_iMaxParticles) * iIdx);
	D3DXVec3TransformNormal(&vDir,&vDir,&matRot);
	pAttribute->vPos = m_vOrigin + vDir * iRadius;

	pAttribute->vVelocity = vDir;
	pAttribute->vVelocity *= _float(rand() % 10 + 5);

	pAttribute->vColor = D3DXCOLOR(1.f,1.f,1.f,1.f);
	pAttribute->fLifeTime = rand()%5 * 0.1f + 0.8f;
	pAttribute->fAge = 0.f;

	
}


_int CParty_SprayA::Update_GameObject(const _float& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	if(m_fTimeAcc > 2.f)
		m_isDestroy = true;

	if(m_isOnce == false)
	{
		m_isOnce = true;

		m_pParentTransform = (Engine::CTransform*)Engine::GetList(L"Layer_Player")->front()->FindComponent(Engine::CGameObject::UPDATE,L"Component_Transform");
		m_pParentTransform->AddRefCnt();

		m_pTransformCom->SetWorldmat(Engine::MyGetNewMatExcRot(*m_pParentTransform->GetWorldMatrix()));
		//ComputeViewZ(m_pTransformCom->GetWorldMatrix());
	}


	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW,&matView);
	D3DXMatrixInverse(&matView,0,&matView);
	_vec3 vCamPos;
	memcpy(&vCamPos,(_vec3*)&matView.m[3][0],sizeof(_vec3));

	list<Engine::PARICLEATT>::iterator iter = m_AttributeList.begin();
	list<Engine::PARICLEATT>::iterator iter_end = m_AttributeList.end();

	_uint iIndex = 0;
	for ( ; iter!=iter_end; ++iter)
	{
		if(iter->isAlive)
		{
			//AddParticle(*iter,iIndex);

			iter->fAge += fTimeDelta;
			iter->vVelocity.y -= 0.1f;
			iter->vPos += iter->vVelocity * fTimeDelta;//iter->fAge;
			iter->fViewZ = D3DXVec3Length(&(vCamPos - *D3DXVec3TransformCoord(&_vec3(),&iter->vPos,m_pTransformCom->GetWorldMatrix())));

			if(iter->fAge > iter->fLifeTime)
				MakeInvisible(&(*iter),fTimeDelta);

			if((*iter).vColor.a < 0.f)
				iter->isAlive = false;
		}
		//else // Revive After Dead?
		//	ResetParticle(&(*iter),iIndex);

		++iIndex;
	}


	//_matrix matTrans,matScale,matRot,matPlayerWorld;
	//D3DXMatrixTranslation(&matTrans,50.f,50.f,0.f);
	//D3DXMatrixScaling(&matScale,1.f,1.f,1.f);
	///*D3DXMatrixRotationY(&matRot,D3DX_PI);*/
	//D3DXMatrixIdentity(&matPlayerWorld);
	//memcpy(&matPlayerWorld.m[3][0],&m_pParentTransform->GetWorldMatrix()->m[3][0],sizeof(_vec3));

	//m_pTransformCom->SetWorldmat(*m_matPlayerFrame * matPlayerWorld);
	//ComputeViewZ(m_pTransformCom->GetWorldMatrix());
	m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_ALPHA);

	

	return 0;
}


void CParty_SprayA::Render_GameObject()
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD,&(*m_matPlayerRH * *m_pParentTransform->GetWorldMatrix()));
	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW,&matView);
	D3DXMatrixInverse(&matView,0,&matView);
	m_pShaderCom->GetEffect()->SetVector("g_vCamWorldPos", (_vec4*)&matView.m[3][0]);


	m_pTransformCom->SetUp_OnGraphicDev();
	m_pShaderCom->SetMatrixOnShader(*m_pTransformCom->GetWorldMatrix());
	m_pShaderCom->GetEffect()->SetTexture("g_baseTexture",m_pTextureCom->GetTexture());
	m_pShaderCom->GetEffect()->SetFloat("g_fMaxSize",200.f);

	m_pShaderCom->GetEffect()->Begin(NULL,0);
	m_pShaderCom->GetEffect()->BeginPass(0);

	Engine::CParticleSystem::Render_GameObject();

	m_pShaderCom->GetEffect()->EndPass();
	m_pShaderCom->GetEffect()->End();
}


CParty_SprayA* CParty_SprayA::Create(LPDIRECT3DDEVICE9 pGraphicDev,_int iNumParticles)
{
	CParty_SprayA* pInst = new CParty_SprayA(pGraphicDev,iNumParticles);
	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;

}

void CParty_SprayA::Free()
{
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pParentTransform);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShaderCom);

	Engine::CParticleSystem::Free();
}

