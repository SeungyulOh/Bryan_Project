#include "stdafx.h"
#include "WindBlade.h"
#include "Export_Engine.h"
#include "BossDevil.h"

CWindBlade::CWindBlade(LPDIRECT3DDEVICE9 pGraphicDev,_vec3 vWorldCreatePos,_float fEffectDir, _int iCloneCnt)
:Engine::CGameObject(pGraphicDev)
,m_pRendererCom(NULL)
,m_pTransformCom(NULL)
,m_pShader(NULL)
,m_fUV(0.f)
,m_isOnce(false)
,m_vWorldFirstPos(vWorldCreatePos)
,m_fEffectDir(fEffectDir)
,m_iCloneCnt(iCloneCnt)
{
	m_fTimeAcc = 0.f;
	m_fAlpha = 1.f;
	m_fSpeed = (_float)(rand()% 5) + 15.f;
	m_fDir = -1.f;
}

CWindBlade::~CWindBlade()
{
	
}

HRESULT CWindBlade::Ready_GameObject(void)
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);
		
	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);
	
	m_pBufferCom = (Engine::CRect_Texture*)Engine::Clone(COM_STATIC,L"Component_Buffer_RectTexture");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Buffer_RectTexture",m_pBufferCom);

	m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_windblade");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_windblade",m_pTextureCom);

	m_pShader = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Default");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Default",m_pShader);
	

	m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,m_vWorldFirstPos);

	_float fRand = (rand()%6) * 0.1f + 0.5f;
	m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,Engine::_vec3(fRand,fRand,fRand));
	m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,_vec3(D3DX_PI/2.f - D3DX_PI/5.f,D3DXToRadian(rand()%180) * m_fEffectDir,0.f));

	m_iImgIdx = rand()%2;//rand()%3;

	CBossDevil* pBoss = ((CBossDevil*)Engine::GetList(L"Layer_Boss")->front());
	float fDist = D3DXVec3Length(&(m_vWorldFirstPos - *(_vec3*)&pBoss->GetWorldMatrix().m[3][0]));

	if(fDist < 20.f)
		pBoss->SetisCol(true);

	return S_OK;
}

_int CWindBlade::Update_GameObject(const _float& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	if(m_fTimeAcc > 0.05f)
		m_fAlpha -= fTimeDelta;
	if(m_fAlpha < 0.f)
		m_isDestroy = true;

	if(m_isOnce == false && m_fTimeAcc > 0.15f && m_iCloneCnt > 0)
	{
		m_isOnce = true;

		_int iClone = --m_iCloneCnt;
		Engine::CGameObject* pGameObject = CWindBlade::Create(m_pGraphicDev,m_vWorldFirstPos,1,iClone);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
		pGameObject = CWindBlade::Create(m_pGraphicDev,m_vWorldFirstPos,-1.f,iClone);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
	}

	Engine::CGameObject::Update_GameObject(fTimeDelta);
		
	//ComputeViewZ(m_pTransformCom->GetWorldMatrix());
	m_pRendererCom->Add_RenderObject(this, Engine::CRenderer::RT_ALPHA);
	return 0;
}

void CWindBlade::Render_GameObject(void)
{
	if(m_fAlpha < 0)
		return;

	_matrix matScale;
	if(m_fTimeAcc < 0.35f && m_fAlpha > 0)
		D3DXMatrixScaling(&matScale,m_fTimeAcc*70.f,m_fTimeAcc*70.f,m_fTimeAcc*70.f);
	else
		return;

	m_pShader->SetMatrixOnShader(matScale * *m_pTransformCom->GetWorldMatrix());
	m_pShader->GetEffect()->SetTexture("g_baseTexture",m_pTextureCom->GetTexture(1));
	m_pShader->GetEffect()->SetFloat("g_fAlpha",m_fAlpha);
	m_pShader->GetEffect()->SetFloat("g_fProcess",m_fTimeAcc);

	_uint iNumPass = 0;

	m_pShader->GetEffect()->Begin(&iNumPass,0);
	m_pShader->GetEffect()->BeginPass(2);
	
	m_pBufferCom->Render_Buffer();

	m_pShader->GetEffect()->EndPass();
	m_pShader->GetEffect()->End();
}

CWindBlade* CWindBlade::Create(LPDIRECT3DDEVICE9 pGraphicDev,_vec3 vWorldCreatePos,_float fEffectDir , _int iCloneCnt)
{
	CWindBlade* pInst = new CWindBlade(pGraphicDev,vWorldCreatePos,fEffectDir,iCloneCnt);
	
	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;
}

void CWindBlade::Free(void)
{
	Engine::CGameObject::Free();

	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShader);
	
}

