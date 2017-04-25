#include "Transform.h"
#include "NaviMgr.h"

using namespace Engine;

Engine::CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphicDev)
:m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matLocal);
	ZeroMemory(m_vInfomation,sizeof(_vec3)*MAT_END);
	m_vInfomation[SCALE] = _vec3(1.f,1.f,1.f);

	D3DXMatrixIdentity(&m_matBilboard);
	D3DXMatrixIdentity(&m_matOffSetRot);
	
}

Engine::CTransform::CTransform(const CTransform& rhs)
:m_pGraphicDev(rhs.m_pGraphicDev)
,m_matWorld(rhs.m_matWorld)

{
	m_pGraphicDev->AddRef();
	memcpy(m_vInfomation,rhs.m_vInfomation,sizeof(_vec3)*MAT_END);
	D3DXMatrixIdentity(&m_matLocal);
	D3DXMatrixIdentity(&m_matBilboard);
	D3DXMatrixIdentity(&m_matOffSetRot);
}
Engine::CTransform::~CTransform(void)
{
	
}

void Engine::CTransform::Update_Component(const _float& fTimeDelta)
{
	MakeWorldMat();
}

HRESULT Engine::CTransform::Ready_Transform(void)
{
	return S_OK;
}
void Engine::CTransform::MakeWorldMat(void)
{
	_matrix matScale, matRotX, matRotY, matRotZ, matTrans;
	D3DXMatrixScaling(&matScale,m_vInfomation[SCALE].x,m_vInfomation[SCALE].y,m_vInfomation[SCALE].z);
	D3DXMatrixRotationX(&matRotX,m_vInfomation[ANGLE].x);
	D3DXMatrixRotationY(&matRotY,m_vInfomation[ANGLE].y);
	D3DXMatrixRotationZ(&matRotZ,m_vInfomation[ANGLE].z);
	D3DXMatrixTranslation(&matTrans,m_vInfomation[POSITION].x,m_vInfomation[POSITION].y,m_vInfomation[POSITION].z);

	m_matWorld = m_matLocal * matScale * matRotZ * matRotX * matRotY * m_matOffSetRot * m_matBilboard * matTrans;
}


void Engine::CTransform::Adjust_ParentMat(const _matrix* pParent)
{
	m_matWorld *= *pParent;
}


void Engine::CTransform::SetUp_OnGraphicDev(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD,&m_matWorld);	
}


Engine::_vec3* Engine::CTransform::GetInfomation(MATINFO eInfo)
{
	return &m_vInfomation[eInfo];
}



void Engine::CTransform::SetInfomation(MATINFO eInfo,const _vec3 vInfo)
{
	m_vInfomation[eInfo] = vInfo;
}


Engine::CTransform* Engine::CTransform::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTransform* pInst = new CTransform(pGraphicDev);

	if(FAILED(pInst->Ready_Transform()))
		return NULL;

	return pInst;
}

Engine::CComponent* Engine::CTransform::Clone(void)
{
	return new CTransform(*this);
}

void Engine::CTransform::Free(void)
{
	Engine::CComponent::Free();

	Engine::Safe_Release(m_pGraphicDev);
}

bool Engine::CTransform::Align_TargetDirection(_vec3 vTarget, _float fTimeDelta)
{
	vTarget.y = 0.f;
	D3DXVec3Normalize(&vTarget,&vTarget);

	_vec3 vMyDir;	
	memcpy(&vMyDir,&m_matWorld.m[2][0],sizeof(float)*3);
	D3DXVec3Normalize(&vMyDir,&vMyDir);

	_vec3 vResult;
	D3DXVec3Cross(&vResult,&vMyDir,&vTarget);

	_float fTempAngle = m_vInfomation[ANGLE].y;
	if(vResult.y > 0.f) //CW
		fTempAngle = fTimeDelta;
	else
		fTempAngle = -fTimeDelta;

	_vec3 vNewDir;
	_matrix matRot;
	D3DXMatrixRotationY(&matRot,fTempAngle);
	D3DXVec3TransformNormal(&vNewDir,&vMyDir,&matRot);

	float fTemp = D3DXVec3Dot(&vNewDir,&vTarget);
	if(fTemp < 0.99f)
	{
		m_vInfomation[POSITION] += vMyDir * fTimeDelta * 30.f;
		m_vInfomation[ANGLE].y += fTempAngle * 5.f;
		MakeWorldMat();
		return false;
	}

	float fCos = D3DXVec3Dot(&vTarget,&_vec3(1.f,0.f,0.f));
	float fAngle = 0.f;
	if(vTarget.z < 0.f)
		fAngle = acosf(fCos);
	else
		fAngle = D3DX_PI*2 - acosf(fCos);

	m_vInfomation[ANGLE].y = fAngle + D3DX_PI/2.f;
	MakeWorldMat();

	/*

	if(D3DXVec3Length(&vResult) > 0.1f)
		return false;*/

	return true;
}

bool Engine::CTransform::Align_TargetDirection(CTransform* pTransform,_ulong dwCurIndex, _vec3 vTarget, _float fTimeDelta)
{
	vTarget.y = 0.f;
	D3DXVec3Normalize(&vTarget,&vTarget);

	_vec3 vMyDir;	
	memcpy(&vMyDir,(_vec3*)&pTransform->GetWorldMatrix()->m[2][0],sizeof(float)*3);
	D3DXVec3Normalize(&vMyDir,&vMyDir);

	_vec3 vResult;
	D3DXVec3Cross(&vResult,&vMyDir,&vTarget);

	_float fTempAngle = m_vInfomation[ANGLE].y;
	if(vResult.y > 0.f) //CW
		fTempAngle = fTimeDelta;
	else
		fTempAngle = -fTimeDelta;

	_vec3 vNewDir;
	_matrix matRot;
	D3DXMatrixRotationY(&matRot,fTempAngle);
	D3DXVec3TransformNormal(&vNewDir,&vMyDir,&matRot);

	float fTemp = D3DXVec3Dot(&vNewDir,&vTarget);
	if(fTemp < 0.99f)
	{
		//m_vInfomation[POSITION] += vMyDir * fTimeDelta;// * 30.f;
		m_vInfomation[ANGLE].y += fTempAngle * 5.f;
		MakeWorldMat();
		CNaviMgr::GetInstance()->MoveOnNavi(pTransform,dwCurIndex,fTimeDelta);
		return false;
	}

	float fCos = D3DXVec3Dot(&vTarget,&_vec3(1.f,0.f,0.f));
	float fAngle = 0.f;
	if(vTarget.z < 0.f)
		fAngle = acosf(fCos);
	else
		fAngle = D3DX_PI*2 - acosf(fCos);

	m_vInfomation[ANGLE].y = fAngle + D3DX_PI/2.f;
	MakeWorldMat();

	/*

	if(D3DXVec3Length(&vResult) > 0.1f)
		return false;*/

	return true;
}

void Engine::CTransform::SetAngleWithLook(_vec3 vNormalizedLook)
{
	// RotY
	_vec3 vDirForY = vNormalizedLook;
	vDirForY.y = 0.f;

	float fCos = D3DXVec3Dot(D3DXVec3Normalize(&vDirForY,&vDirForY),&_vec3(0.f,0.f,1.f));

	if(vNormalizedLook.x > 0)
		m_vInfomation[ANGLE].y = acosf(fCos);
	else
		m_vInfomation[ANGLE].y = D3DX_PI*2.f - acosf(fCos);
}

void Engine::CTransform::SetBillBoardAfterUpdate()
{
	_matrix matWorld = m_matWorld;
	ZeroMemory(&matWorld.m[3][0],sizeof(_vec3));
	
	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW,&matView);
	D3DXMatrixInverse(&matView,0,&matView);

	matWorld *= matView;
	memcpy(&matWorld.m[3][0],&m_matWorld.m[3][0],sizeof(_vec3));

	m_matWorld = matWorld;
}
