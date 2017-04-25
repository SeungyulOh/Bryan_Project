#include "stdafx.h"
#include "Atk4Throw.h"
#include "Export_Engine.h"

CAtk4Throw::CAtk4Throw(LPDIRECT3DDEVICE9 pGraphicDev, _int iNumParticles)
:Engine::CParticleSystem(pGraphicDev)
,m_pRendererCom(NULL)
,m_pTextureCom(NULL)
,m_pShaderCom(NULL)
,m_pParentTransform(NULL)
,m_isOnce(false)
,m_matPlayerRH(NULL)
{
	m_vOrigin = _vec3(0.f,0.f,0.f);

	m_fSize = 0.8f;
	m_dwVTXSize = 2048;
	m_dwVTXOffSet = 0;
	m_dwBatchSize = 512;

	m_pParentTransform = (Engine::CTransform*)Engine::GetList(L"Layer_Player")->front()->FindComponent(Engine::CGameObject::UPDATE,L"Component_Transform");
	m_pParentTransform->AddRefCnt();

	Engine::CDynamicMesh* pPlayerMesh = (Engine::CDynamicMesh*)Engine::GetList(L"Layer_Player")->front()->FindComponent(Engine::CGameObject::NONUPDATE,L"Component_Mesh_Hagie");
	Engine::D3DXFRAME_DERIVED* pFrameFound = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(pPlayerMesh->GetRootFrame(),"ValveBiped_Bip01_R_Hand");
	m_matPlayerRH = &pFrameFound->CombinedMatrix;


	CParticleSystem::Init(iNumParticles);

	for(int i =0; i<iNumParticles ; ++i)
		AddParticle();
	
	D3DXMatrixIdentity(&m_matLocal);
	m_fAlpha = 1.f;
	m_fTimeAcc = 0.f;
	m_iRadius = 20;
	m_fXdirection = 1.f;
}

CAtk4Throw::~CAtk4Throw()
{

}

HRESULT CAtk4Throw::Ready_GameObject()
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);

	m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_bomb");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_bomb",m_pTextureCom);

	m_pShaderCom = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Particle");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Particle",m_pShaderCom);


	//D3DXMatrixScaling(&m_matLocal,3.f,1.f,4.f);
	_matrix matRot,matTrans;
	D3DXMatrixRotationZ(&matRot,D3DX_PI/2.f);
	D3DXMatrixTranslation(&matTrans,0.f,30.f,0.f);
	m_matLocal = matTrans;


	

	return S_OK; 
}

void CAtk4Throw::ResetParticle(Engine::PARICLEATT* pAttribute,_uint iIndex)
{
	pAttribute->isAlive = true;

	CalculateParticlePos(pAttribute);

	m_vBoundingMinMax[0] = _vec3(-1.f,-1.f,-1.f);
	m_vBoundingMinMax[1] = _vec3(1.f,1.f,1.f);

	Engine::CParticleSystem::GetRandomVector(&pAttribute->vVelocity,&m_vBoundingMinMax[0],&m_vBoundingMinMax[1]);

	D3DXVec3Normalize(&pAttribute->vVelocity,&pAttribute->vVelocity);

	pAttribute->vVelocity *= 3.f;
	//pAttribute->vVelocity = *(_vec3*)&m_pParentTransform->GetWorldMatrix()->m[2][0];

	/*pAttribute->vColor = D3DXCOLOR(Engine::CParticleSystem::GetRandomFloat(0.f,1.f),
									Engine::CParticleSystem::GetRandomFloat(0.f,1.f),
									Engine::CParticleSystem::GetRandomFloat(0.f,1.f),
									1.f);*/

	float fBlue = 1.f;
	float fGreen = (rand()%255) / 255.f;
	float fRed = (rand() % 255) / 255.f;
	float fAlpha = (rand() % 256) / 255.f;
	pAttribute->vColor = D3DXCOLOR(1.f,1.f,1.f,1.f);
	
	pAttribute->fAge = 0.0f;
	pAttribute->fLifeTime = (_float)(rand()%3) * 0.1f;

	pAttribute->fViewZ = (_float)(rand()%6);


}


_int CAtk4Throw::Update_GameObject(const _float& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	m_fTimeDelta = m_fTimeDelta;
	if(m_fTimeAcc > 0.3f)
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

	//m_pParentTransform->SetWorldmat(*m_matPlayerRH * m_matLocal *  *m_pParentTransform->GetWorldMatrix())
	m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_ALPHA);

	return 0;
}


void CAtk4Throw::Render_GameObject()
{


	m_pShaderCom->GetEffect()->SetTexture("g_baseTexture",m_pTextureCom->GetTexture(2));
	m_pShaderCom->GetEffect()->SetBool("g_isZbufferEnable",true);
	
	float fRand = rand()%10+60.f + m_fTimeAcc * 300.f;
	m_pShaderCom->GetEffect()->SetFloat("g_fMaxSize",fRand);
	m_pShaderCom->GetEffect()->SetFloat("g_fAlpha", m_fAlpha);
	m_pShaderCom->GetEffect()->SetFloat("g_fRandValue",0.f);

	m_pShaderCom->GetEffect()->Begin(NULL,0);
	m_pShaderCom->GetEffect()->BeginPass(2);

	Engine::CParticleSystem::Render_GameObject();

	m_pShaderCom->GetEffect()->EndPass();
	m_pShaderCom->GetEffect()->End();
}



CAtk4Throw* CAtk4Throw::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iNumParticles)
{
	CAtk4Throw* pInst = new CAtk4Throw(pGraphicDev,iNumParticles);
	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;

}

void CAtk4Throw::Free()
{
	Engine::Safe_Release(m_pParentTransform);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShaderCom);

	Engine::CParticleSystem::Free();
}

void CAtk4Throw::CalculateParticlePos(Engine::PARICLEATT* pAttribute)
{
	_matrix matTemp;
	_float fRandX = rand()% 5- 2.f;
	_float fRandY = rand()% 5 - 2.f;
	_float fRandZ = rand()% 5 - 2.f;
	_vec3 vPos = _vec3(fRandX,fRandY,fRandZ);
	_vec3 vDir = _vec3(0.f,1.f,0.f);
	_float fRandSpeed = rand()%100 + 700.f;
	vPos.z += m_fTimeAcc * fRandSpeed;
	_matrix matRot;
	D3DXMatrixRotationZ(&matRot,m_fTimeAcc*10.f);
	D3DXVec3TransformNormal(&vDir,&vDir,&matRot);

	_float fRand = rand()% 50 + 50.f;

	vPos.x =vDir.x * m_fTimeAcc * fRand;
	vPos.y =vDir.y * m_fTimeAcc * fRand;

	D3DXMatrixTranslation(&matTemp,vPos.x,vPos.y,vPos.z);
	
	_matrix matWorld =matTemp* m_matLocal* *m_pParentTransform->GetWorldMatrix();//*m_matPlayerRH * *m_pParentTransform->GetWorldMatrix();
	pAttribute->vPos = *(_vec3*)&matWorld.m[3][0];
	/*pAttribute->vPos += *(_vec3*)&m_pParentTransform->GetWorldMatrix()->m[2][0] * m_fTimeAcc * 200.f;
	pAttribute->vPos -= *(_vec3*)&m_pParentTransform->GetWorldMatrix()->m[0][0] * m_fTimeAcc * 150.f;*/

}

