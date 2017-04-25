#include "stdafx.h"
#include "BossWeapon.h"
#include "Engine_Struct.h"
#include "Export_Engine.h"
#include "BossDevil.h"
#include "Kadan_Trail.h"
#include "ChildEffect.h"
#include "ExpEffect.h"
#include "BossWeaponParticle.h"

CBossWeapon::CBossWeapon(LPDIRECT3DDEVICE9 pGraphicDev)
:Engine::CGameObject(pGraphicDev)
,m_isOnce(false)
,m_pTargetCollider(NULL)
,m_pTransformCom(NULL)
,m_pRendererCom(NULL)
,m_pStaticMeshCom(NULL)
,m_pBoxColliderCom(NULL)
,m_pShaderCom(NULL)
,m_pBossVisible(NULL)
,m_pMadMeshCom(NULL)
,m_isMad(false)
{

}

CBossWeapon::~CBossWeapon(void)
{

}

HRESULT CBossWeapon::Ready_GameObject(void)
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);

	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);

	m_pStaticMeshCom = (Engine::CStaticMesh*)Engine::Clone(COM_STAGE,L"Component_Mesh_BossWeapon");
	if(m_pStaticMeshCom == NULL)
		return S_OK;
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Mesh_BossWeapon",m_pStaticMeshCom);

	m_pMadMeshCom = (Engine::CStaticMesh*)Engine::Clone(COM_STAGE,L"Component_Mesh_BossWeaponMad");
	if(m_pMadMeshCom == NULL)
		return S_OK;

	m_pBoxColliderCom = (Engine::CBoxCollider*)Engine::Clone(COM_STAGE,L"Component_BoxCollider");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_BoxCollider",m_pBoxColliderCom);
	m_pBoxColliderCom->Compute_MinMax(m_pStaticMeshCom);

	m_pShaderCom = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Mesh");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Mesh",m_pShaderCom);

	m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,_vec3(0.f,D3DX_PI,D3DX_PI/2.f));

	return S_OK;
}

_int CBossWeapon::Update_GameObject(const _float& fTimeDelta)
{

	if(m_pStaticMeshCom == NULL)
		return 0;

	if(m_isOnce == false)
	{
		m_isOnce=true;
		SetUpOnce();

		CBossDevil* pBoss = dynamic_cast<CBossDevil*>(Engine::GetList(L"Layer_Boss")->front());
		if(pBoss != NULL)
		{
			m_pBossVisible = pBoss->GetVisible();

			Engine::CGameObject* pGameObject = CKadan_Trail::Create(m_pGraphicDev,m_pTransformCom->GetWorldMatrix());
			m_WeaponEffectList.push_back(pGameObject);
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);

		}
			
	}
	if(Engine::Get_KeyMgr()->KeyPressed(KEY_M))
	{
		Engine::CGameObject* pGameObject = NULL;
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::KADANWEAPON,CExpEffect::KADANWEAPON2, m_pTransformCom->GetWorldMatrix());
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::KADANWEAPON,CExpEffect::KADANWEAPON4, m_pTransformCom->GetWorldMatrix());
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::KADANWEAPON,CExpEffect::KADANWEAPON12, m_pTransformCom->GetWorldMatrix());
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::KADANWEAPON,CExpEffect::KADANWEAPON13, m_pTransformCom->GetWorldMatrix());
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);

		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEHIT,CExpEffect::WAVEHIT6,m_pBossMat);
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEHIT,CExpEffect::WAVEHIT7,m_pBossMat);
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
	}

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	if(m_pBossVisible == NULL)
		m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_NONALPHA);
	else if(m_pBossVisible != NULL && *m_pBossVisible == true)
		m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_NONALPHA);


	m_pTransformCom->Adjust_ParentMat(&(m_OffSetMat * *m_pRHFrameFromBoss* *m_pBossMat));




	/////


	CBossDevil* pBoss = dynamic_cast<CBossDevil*>(Engine::GetList(L"Layer_Boss")->front());
	if(pBoss != NULL)
	{
		if(pBoss->GetState() == CBossDevil::TELEPORTBEGIN)
		{
			list<CGameObject*>::iterator iter = m_WeaponEffectList.begin();
			list<CGameObject*>::iterator iter_end = m_WeaponEffectList.end();
			for( ; iter!=iter_end ; ++iter)
			{
				((CExpEffect*)(*iter))->SetVisible(false);
			}
		}
		else
		{
			list<CGameObject*>::iterator iter = m_WeaponEffectList.begin();
			list<CGameObject*>::iterator iter_end = m_WeaponEffectList.end();
			for( ; iter!=iter_end ; ++iter)
			{
				((CExpEffect*)(*iter))->SetVisible(true);
			}
		}
	}



	return 0;
}


_int CBossWeapon::LastUpdate_GameObject(const _float& fTimeDelta)
{

	if(m_pStaticMeshCom == NULL)
		return 0;

	if(m_isMad == true)
	{
		m_isMad = false;

		MAPCOMPONENT::iterator iter = m_MapComponent[CGameObject::NONUPDATE].find(L"Component_Mesh_BossWeapon");
		if(iter == m_MapComponent[CGameObject::NONUPDATE].end())
			return 0;
		Engine::Safe_Release(iter->second);
		Engine::Safe_Release(iter->second);
		m_MapComponent[CGameObject::NONUPDATE].erase(iter);

		m_pStaticMeshCom = m_pMadMeshCom;
		m_pMadMeshCom = NULL;
		Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Mesh_BossWeapon",m_pStaticMeshCom);
		SetUpOnce();

		Engine::CGameObject* pGameObject = NULL;
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::KADANWEAPON,CExpEffect::KADANWEAPON2, m_pTransformCom->GetWorldMatrix());
		m_WeaponEffectList.push_back(pGameObject);
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);

		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::KADANWEAPON,CExpEffect::KADANWEAPON4, m_pTransformCom->GetWorldMatrix());
		m_WeaponEffectList.push_back(pGameObject);
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);

		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::KADANWEAPON,CExpEffect::KADANWEAPON12, m_pTransformCom->GetWorldMatrix());
		m_WeaponEffectList.push_back(pGameObject);
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);

		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::KADANWEAPON,CExpEffect::KADANWEAPON13, m_pTransformCom->GetWorldMatrix());
		m_WeaponEffectList.push_back(pGameObject);
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);


		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEHIT,CExpEffect::WAVEHIT6,m_pBossMat);
		m_WeaponEffectList.push_back(pGameObject);
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEHIT,CExpEffect::WAVEHIT7,m_pBossMat);
		m_WeaponEffectList.push_back(pGameObject);
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
	}

	/*if(m_pTargetCollider == NULL)
		m_pTargetCollider = (Engine::CBoxCollider*)Engine::GetList(L"Layer_Player")->front()->FindComponent(Engine::CGameObject::NONUPDATE,L"Component_BoxCollider");
	else	
		m_isCol = m_pBoxColliderCom->CollisionCheck_OBB(m_pTargetCollider);*/

	return 0;
}


void CBossWeapon::Render_GameObject(void)
{
	if(m_isDestroy == true)
		return;

	/*CBossDevil* pBoss = dynamic_cast<CBossDevil*>(Engine::GetList(L"Layer_Boss")->front());
	if(pBoss != NULL)
	{
		if(pBoss->GetState() == CBossDevil::TELEPORTBEGIN)
		{
			list<CGameObject*>::iterator iter = m_WeaponEffectList.begin();
			list<CGameObject*>::iterator iter_end = m_WeaponEffectList.end();
			for( ; iter!=iter_end ; ++iter)
			{
				((CExpEffect*)(*iter))->SetVisible(false);
			}
		}
		else
		{
			list<CGameObject*>::iterator iter = m_WeaponEffectList.begin();
			list<CGameObject*>::iterator iter_end = m_WeaponEffectList.end();
			for( ; iter!=iter_end ; ++iter)
			{
				((CExpEffect*)(*iter))->SetVisible(true);
			}
		}
	}*/
	
	

	m_pTransformCom->SetUp_OnGraphicDev();
	m_pStaticMeshCom->SetUp_ConstantTable(m_pShaderCom->GetEffect());
	m_pStaticMeshCom->Render_SubSetsWithShader(m_pShaderCom->GetEffect(),m_pStaticMeshCom->GetSubSetCnt());
	m_pStaticMeshCom->Render_SubSets();

	m_pBoxColliderCom->Render(Engine::CBoxCollider::BOX_OBB,*m_pTransformCom->GetWorldMatrix(),m_isCol,false);
}


CBossWeapon* CBossWeapon::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBossWeapon* pInst = new CBossWeapon(pGraphicDev);

	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;
}

void CBossWeapon::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pStaticMeshCom);
	Engine::Safe_Release(m_pBoxColliderCom);
	Engine::Safe_Release(m_pShaderCom);
	//Engine::Safe_Release(m_pMadMeshCom);

	if(m_isMad == false)
		Engine::Safe_Release(m_pMadMeshCom);

	Engine::CGameObject::Free();
}

void CBossWeapon::SetUpOnce()
{
	Engine::CTransform* pBossTransform = (Engine::CTransform*)Engine::GetList(L"Layer_Boss")->front()->FindComponent(Engine::CGameObject::UPDATE,L"Component_Transform");
	m_pBossMat = pBossTransform->GetWorldMatrix();

	Engine::CDynamicMesh* pBossMesh = (Engine::CDynamicMesh*)Engine::GetList(L"Layer_Boss")->front()->FindComponent(Engine::CGameObject::NONUPDATE,L"Component_Mesh_Kadan");
	Engine::D3DXFRAME_DERIVED* pRightHand = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(pBossMesh->GetRootFrame(),"ValveBiped_Anim_Attachment_RH");
	m_pRHFrameFromBoss = &pRightHand->CombinedMatrix;
	m_OffSetMat =*pBossMesh->GetInitMatrix();

	CBossDevil* pBossDevil = dynamic_cast<CBossDevil*>(Engine::GetList(L"Layer_Boss")->front());


	//m_WeaponEffectList.push_back(pGameObject);

	if(pBossDevil != NULL)
	{
		m_pBossVisible = pBossDevil->GetVisible();
		/*m_pTransformCom->Update_Component(0.f);
		
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::KADANWEAPON,CExpEffect::KADANWEAPON2, m_pTransformCom->GetWorldMatrix());
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
		m_WeaponEffectList.push_back(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::KADANWEAPON,CExpEffect::KADANWEAPON4, m_pTransformCom->GetWorldMatrix());
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
		m_WeaponEffectList.push_back(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::KADANWEAPON,CExpEffect::KADANWEAPON12, m_pTransformCom->GetWorldMatrix());
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
		m_WeaponEffectList.push_back(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::KADANWEAPON,CExpEffect::KADANWEAPON13, m_pTransformCom->GetWorldMatrix());
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
		m_WeaponEffectList.push_back(pGameObject);*/
	}
}
