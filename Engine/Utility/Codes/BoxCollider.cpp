#include "BoxCollider.h"
#include "StaticMesh.h"
#include "DynamicMesh.h"

using namespace Engine;

Engine::CBoxCollider::CBoxCollider(LPDIRECT3DDEVICE9 pGraphicDev)
:m_pGraphicDev(pGraphicDev)
,m_pVB(NULL)
,m_pIB(NULL)
{
	m_pGraphicDev->AddRef();
	ZeroMemory(m_MyLength,sizeof(_float)*6);
	ZeroMemory(m_TargetLength,sizeof(_float)*6);
}

Engine::CBoxCollider::CBoxCollider(const CBoxCollider& rhs)
:m_pGraphicDev(rhs.m_pGraphicDev)
,m_pVB(rhs.m_pVB)
,m_pIB(rhs.m_pIB)
{
	m_pGraphicDev->AddRef();
	m_pVB->AddRef();
	m_pIB->AddRef();

	for(_ulong i = 0 ; i<COL_END ; ++i)
	{
		m_pTexture[i] = rhs.m_pTexture[i];
		m_pTexture[i]->AddRef();
	}

	D3DXMatrixIdentity(&m_LocalMat);
}

Engine::CBoxCollider::~CBoxCollider(void)
{

}

void Engine::CBoxCollider::Ready_Collider(void)
{
	for(_uint i = 0; i<COL_END; ++i)
		D3DXCreateTexture(m_pGraphicDev,1,1,1,0,D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,&m_pTexture[i]);

	D3DLOCKED_RECT LockedRect;

	ZeroMemory(&LockedRect,sizeof(D3DLOCKED_RECT));
	m_pTexture[0]->LockRect(0,&LockedRect,NULL,0);
	*((_ulong*)LockedRect.pBits) = D3DXCOLOR(0.f,1.f,0.f,1.f);
	m_pTexture[0]->UnlockRect(0);

	ZeroMemory(&LockedRect,sizeof(D3DLOCKED_RECT));
	m_pTexture[1]->LockRect(0,&LockedRect,NULL,0);
	*((_ulong*)LockedRect.pBits) = D3DXCOLOR(1.f,0.f,0.f,1.f);
	m_pTexture[1]->UnlockRect(0);





	if(FAILED(m_pGraphicDev->CreateVertexBuffer(sizeof(VTXCUBETEX)*8,0,D3DFVF_VTXCUBETEX,D3DPOOL_MANAGED,&m_pVB,NULL)))
		return;

	VTXCUBETEX* pVertex = NULL;
	m_pVB->Lock(0,0,(void**)&pVertex,0);

	pVertex[0].vPos = _vec3(-0.5f,0.5f,-0.5f);
	pVertex[0].vTexUV = pVertex[0].vPos;

	pVertex[1].vPos = _vec3(0.5f,0.5f,-0.5f);
	pVertex[1].vTexUV = pVertex[1].vPos;

	pVertex[2].vPos = _vec3(0.5f,-0.5f,-0.5f);
	pVertex[2].vTexUV = pVertex[2].vPos;

	pVertex[3].vPos = _vec3(-0.5f,-0.5f,-0.5f);
	pVertex[3].vTexUV = pVertex[3].vPos;

	pVertex[4].vPos = _vec3(-0.5f,0.5f,0.5f);
	pVertex[4].vTexUV = pVertex[4].vPos;

	pVertex[5].vPos = _vec3(0.5f,0.5f,0.5f);
	pVertex[5].vTexUV = pVertex[5].vPos;

	pVertex[6].vPos = _vec3(0.5f,-0.5f,0.5f);
	pVertex[6].vTexUV = pVertex[6].vPos;

	pVertex[7].vPos = _vec3(-0.5f,-0.5f,0.5f);
	pVertex[7].vTexUV = pVertex[7].vPos;

	m_pVB->Unlock();

	if(FAILED(m_pGraphicDev->CreateIndexBuffer(sizeof(INDEX32)*12,NULL,D3DFMT_INDEX32,D3DPOOL_MANAGED,&m_pIB,NULL)))
		return;

	INDEX32* pIndex = NULL;
	m_pIB->Lock(0,0,(void**)&pIndex,0);

	pIndex[0]._0 = 3;
	pIndex[0]._1 = 0;
	pIndex[0]._2 = 1;

	pIndex[1]._0 = 3;
	pIndex[1]._1 = 1;
	pIndex[1]._2 = 2;

	pIndex[2]._0 = 4;
	pIndex[2]._1 = 7;
	pIndex[2]._2 = 6;

	pIndex[3]._0 = 4;
	pIndex[3]._1 = 6;
	pIndex[3]._2 = 5;

	pIndex[4]._0 = 7;
	pIndex[4]._1 = 3;
	pIndex[4]._2 = 2;

	pIndex[5]._0 = 7;
	pIndex[5]._1 = 2;
	pIndex[5]._2 = 6;

	pIndex[6]._0 = 0;
	pIndex[6]._1 = 4;
	pIndex[6]._2 = 5;

	pIndex[7]._0 = 0;
	pIndex[7]._1 = 5;
	pIndex[7]._2 = 1;

	pIndex[8]._0 = 2;
	pIndex[8]._1 = 1;
	pIndex[8]._2 = 5;

	pIndex[9]._0 = 2;
	pIndex[9]._1 = 5;
	pIndex[9]._2 = 6;

	pIndex[10]._0 = 7;
	pIndex[10]._1 = 4;
	pIndex[10]._2 = 0;

	pIndex[11]._0 = 7;
	pIndex[11]._1 = 0;
	pIndex[11]._2 = 3;

	m_pIB->Unlock();
}

void Engine::CBoxCollider::Update_Component(const _float& fTimeDelta)
{

}


void Engine::CBoxCollider::Compute_MinMax(Engine::CStaticMesh* pStaticMesh)
{
	pStaticMesh->Compute_MinMax(&m_vMin,&m_vMax);

	_float fWidth = m_vMax.x - m_vMin.x;
	_float fHeight = m_vMax.y - m_vMin.y;
	_float fDepth = m_vMax.z - m_vMin.z;

	_matrix matScale, matTrans;
	D3DXMatrixScaling(&matScale,fWidth,fHeight,fDepth);
	D3DXMatrixTranslation(&matTrans,(m_vMin.x + m_vMax.x) * 0.5f, (m_vMin.y + m_vMax.y) * 0.5f, (m_vMin.z + m_vMax.z) * 0.5f);
	m_LocalMat = matScale * matTrans;
}

void Engine::CBoxCollider::Compute_MinMax(Engine::CDynamicMesh* pDynamicMesh,_matrix matInit, _ulong lMeshContainerIdx /*= 0*/,_vec3 vScale)
{
	pDynamicMesh->Compute_MinMax(&m_vMin,&m_vMax,lMeshContainerIdx);

	_float fWidth = m_vMax.x - m_vMin.x;
	_float fHeight = m_vMax.y - m_vMin.y;
	_float fDepth = m_vMax.z - m_vMin.z;

	fWidth *= vScale.x;
	fHeight *= vScale.y;
	fDepth *= vScale.z;

	_matrix matScale, matTrans;
	D3DXMatrixScaling(&matScale,fWidth,fHeight,fDepth);
	D3DXMatrixTranslation(&matTrans,(m_vMin.x + m_vMax.x) * 0.5f, (m_vMin.y + m_vMax.y) * 0.5f, (m_vMin.z + m_vMax.z) * 0.5f);
	m_LocalMat = matScale * matInit * matTrans;
}

void Engine::CBoxCollider::Render(BOX_TYPE eType, _matrix matPreWorld, _bool isCol ,  _bool isRender)
{
	if(false == isCol)
		m_pGraphicDev->SetTexture(0,m_pTexture[COL_X]);
	else
		m_pGraphicDev->SetTexture(0,m_pTexture[COL_O]);

	

	if(eType == BOX_OBB)
		m_matWorld = matPreWorld;
	else
	{
		_matrix matScale, matTrans;
		D3DXMatrixScaling(&matScale,D3DXVec3Length((_vec3*)&matPreWorld.m[0][0]),
			D3DXVec3Length((_vec3*)&matPreWorld.m[1][0]),
			D3DXVec3Length((_vec3*)&matPreWorld.m[2][0]));
		D3DXMatrixTranslation(&matTrans,matPreWorld._41,matPreWorld._42,matPreWorld._43);
		m_matWorld = matScale * matTrans;
	}
	
	if(isRender == false)
		return;

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING,FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pGraphicDev->SetTransform(D3DTS_WORLD,&(m_LocalMat * m_matWorld));

	m_pGraphicDev->SetStreamSource(NULL,m_pVB,0,sizeof(VTXCUBETEX));
	m_pGraphicDev->SetFVF(D3DFVF_VTXCUBETEX);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,8,0,12);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING,TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

_bool Engine::CBoxCollider::CollisionCheck_AABB(CBoxCollider* pTargetCollider)
{
	_vec3 vWorldMin, vWorldMax;
	_vec3 vWorldMin_Target, vWorldMax_Target;

	D3DXVec3TransformCoord(&vWorldMin,&m_vMin,&m_matWorld);
	D3DXVec3TransformCoord(&vWorldMax,&m_vMax,&m_matWorld);

	D3DXVec3TransformCoord(&vWorldMin_Target,&pTargetCollider->m_vMin,&pTargetCollider->m_matWorld);
	D3DXVec3TransformCoord(&vWorldMax_Target,&pTargetCollider->m_vMax,&pTargetCollider->m_matWorld);

	if( max(vWorldMin.x,vWorldMin_Target.x) > min(vWorldMax.x , vWorldMax_Target.x))
		return false;

	if( max(vWorldMin.z,vWorldMin_Target.z) > min(vWorldMax.z , vWorldMax_Target.z))
		return false;

	if( max(vWorldMin.y,vWorldMin_Target.y) > min(vWorldMax.y , vWorldMax_Target.y))
		return false;

	return true;
}


_bool Engine::CBoxCollider::CollisionCheck_OBB(CBoxCollider* pTargetCollider)
{
	///////////NewTry........../////////

	GetMinMaxPos(this,pTargetCollider,CAL_MIN);
	GetMinMaxPos(pTargetCollider,this,CAL_MIN);

	if(m_MyLength[1].x > m_MyLength[0].x + 0.5f)
		return false;
	if(m_MyLength[1].y > m_MyLength[0].y + 0.5f)
		return false;
	if(m_MyLength[1].z > m_MyLength[0].z + 0.5f)
		return false;

	if(m_TargetLength[1].x > m_TargetLength[0].x + 0.5f)
		return false;
	if(m_TargetLength[1].y > m_TargetLength[0].y + 0.5f)
		return false;
	if(m_TargetLength[1].z > m_TargetLength[0].z + 0.5f)
		return false;

	//////////////GG//////////////////

	
	/*_vec3 vTargetMinPos = GetMinMaxPos(this,pTargetCollider,CAL_MIN);
	_vec3 vTargetMaxPos = GetMinMaxPos(this,pTargetCollider,CAL_MAX);

	if(max(-0.5f,vTargetMinPos.x) > min(0.5f,vTargetMaxPos.x))
		return false;

	if(max(-0.5f,vTargetMinPos.y) > min(0.5f,vTargetMaxPos.y))
		return false;

	if(max(-0.5f,vTargetMinPos.z) > min(0.5f,vTargetMaxPos.z))
		return false;

	_vec3 vMyMinPos = GetMinMaxPos(pTargetCollider,this,CAL_MIN);
	_vec3 vMyMaxPos = GetMinMaxPos(pTargetCollider,this,CAL_MAX);

	if(max(-0.5f,vMyMinPos.z) > min(0.5f,vMyMaxPos.z))
		return false;
	if(max(-0.5f,vMyMinPos.x) > min(0.5f,vMyMaxPos.x))
		return false;*/


	return true;

	
}


_vec3 Engine::CBoxCollider::GetMinMaxPos(CBoxCollider* pMyCollider, CBoxCollider* pTargetCollider, CALC_TYPE eType)
{
	_matrix matInverse =pMyCollider->m_LocalMat * pMyCollider->m_matWorld;
	D3DXMatrixInverse(&matInverse,0,&matInverse);

	//_matrix matLocalTarget = pTargetCollider->m_LocalMat*pTargetCollider->m_matWorld*matInverse;
	/*pVec[CAL_STD].x = abs(D3DXVec3Dot((_vec3*)&matLocalTarget.m[3][0],&_vec3(1.f,0.f,0.f)));
	pVec[CAL_STD].y = abs(D3DXVec3Dot((_vec3*)&matLocalTarget.m[3][0],&_vec3(0.f,1.f,0.f)));
	pVec[CAL_STD].z = abs(D3DXVec3Dot((_vec3*)&matLocalTarget.m[3][0],&_vec3(0.f,0.f,1.f)));

	pVec[CAL_CURR].x = 0.5f * (abs(D3DXVec3Dot(&(*(_vec3*)&matLocalTarget.m[0][0]),&_vec3(1.f,0.f,0.f))) + abs(D3DXVec3Dot(&(*(_vec3*)&matLocalTarget.m[1][0]),&_vec3(1.f,0.f,0.f))) + abs(D3DXVec3Dot(&(*(_vec3*)&matLocalTarget.m[2][0]),&_vec3(1.f,0.f,0.f))));
	pVec[CAL_CURR].y = 0.5f * (abs(D3DXVec3Dot(&(*(_vec3*)&matLocalTarget.m[0][0]),&_vec3(0.f,1.f,0.f))) + abs(D3DXVec3Dot(&(*(_vec3*)&matLocalTarget.m[1][0]),&_vec3(0.f,1.f,0.f))) + abs(D3DXVec3Dot(&(*(_vec3*)&matLocalTarget.m[2][0]),&_vec3(0.f,1.f,0.f))));
	pVec[CAL_CURR].z = 0.5f * (abs(D3DXVec3Dot(&(*(_vec3*)&matLocalTarget.m[0][0]),&_vec3(0.f,0.f,1.f))) + abs(D3DXVec3Dot(&(*(_vec3*)&matLocalTarget.m[1][0]),&_vec3(0.f,0.f,1.f))) + abs(D3DXVec3Dot(&(*(_vec3*)&matLocalTarget.m[2][0]),&_vec3(0.f,0.f,1.f))));*/

	_vec3 Vertex[8], vResult;
	//Vertex[0] = _vec3(-0.5f,0.5f,-0.5f);
	//Vertex[1] = _vec3(0.5f,0.5f,-0.5f);
	//Vertex[2] = _vec3(0.5f,-0.5f,-0.5f);
	//Vertex[3] = _vec3(-0.5f,-0.5f,-0.5f);
	//Vertex[4] = _vec3(-0.5f,0.5f,0.5f);
	//Vertex[5] = _vec3(0.5f,0.5f,0.5f);
	//Vertex[6] = _vec3(0.5f,-0.5f,0.5f);
	//Vertex[7] = _vec3(-0.5f,-0.5f,0.5f);

	//for(_ulong i =0;i<8;++i)
	//	D3DXVec3TransformCoord(&Vertex[i],&Vertex[i],&(pTargetCollider->m_LocalMat*pTargetCollider->m_matWorld*matInverse));

	_vec3 vCenter,vDir,vDir2,vDir3;
	D3DXVec3TransformCoord(&vCenter,&_vec3(0.0f,0.0f,0.0f),&(pTargetCollider->m_LocalMat*pTargetCollider->m_matWorld*matInverse));
	D3DXVec3TransformCoord(&vDir,&_vec3(0.5f,0.0f,0.0f),&(pTargetCollider->m_LocalMat*pTargetCollider->m_matWorld*matInverse));
	D3DXVec3TransformCoord(&vDir2,&_vec3(0.0f,0.5f,0.0f),&(pTargetCollider->m_LocalMat*pTargetCollider->m_matWorld*matInverse));
	D3DXVec3TransformCoord(&vDir3,&_vec3(0.0f,0.0f,0.5f),&(pTargetCollider->m_LocalMat*pTargetCollider->m_matWorld*matInverse));

	//_matrix matTest = pTargetCollider->m_LocalMat*pTargetCollider->m_matWorld*matInverse;
	//_float fTest = D3DXVec3Dot((_vec3*)&matTest.m[0][0],(_vec3*)&matTest.m[1][0]);

	if(this == pMyCollider)
	{
		m_TargetLength[1].x = abs(D3DXVec3Dot(&vCenter,&_vec3(1.f,0.f,0.f)));
		m_TargetLength[1].y = abs(D3DXVec3Dot(&vCenter,&_vec3(0.f,1.f,0.f)));
		m_TargetLength[1].z = abs(D3DXVec3Dot(&vCenter,&_vec3(0.f,0.f,1.f)));

		m_TargetLength[0].x = abs(D3DXVec3Dot(&(vDir - vCenter) , &_vec3(1.f,0.f,0.f))) + abs(D3DXVec3Dot(&(vDir2 - vCenter) , &_vec3(1.f,0.f,0.f))) + abs(D3DXVec3Dot(&(vDir3 - vCenter) , &_vec3(1.f,0.f,0.f)));
		m_TargetLength[0].y = abs(D3DXVec3Dot(&(vDir - vCenter) , &_vec3(0.f,1.f,0.f))) + abs(D3DXVec3Dot(&(vDir2 - vCenter) , &_vec3(0.f,1.f,0.f))) + abs(D3DXVec3Dot(&(vDir3 - vCenter) , &_vec3(0.f,1.f,0.f)));
		m_TargetLength[0].z = abs(D3DXVec3Dot(&(vDir - vCenter) , &_vec3(0.f,0.f,1.f))) + abs(D3DXVec3Dot(&(vDir2 - vCenter) , &_vec3(0.f,0.f,1.f))) + abs(D3DXVec3Dot(&(vDir3 - vCenter) , &_vec3(0.f,0.f,1.f)));
	}
	else
	{
		m_MyLength[1].x = abs(D3DXVec3Dot(&vCenter,&_vec3(1.f,0.f,0.f)));
		m_MyLength[1].y = abs(D3DXVec3Dot(&vCenter,&_vec3(0.f,1.f,0.f)));
		m_MyLength[1].z = abs(D3DXVec3Dot(&vCenter,&_vec3(0.f,0.f,1.f)));

		m_MyLength[0].x = abs(D3DXVec3Dot(&(vDir - vCenter) , &_vec3(1.f,0.f,0.f))) + abs(D3DXVec3Dot(&(vDir2 - vCenter) , &_vec3(1.f,0.f,0.f))) + abs(D3DXVec3Dot(&(vDir3 - vCenter) , &_vec3(1.f,0.f,0.f)));
		m_MyLength[0].y = abs(D3DXVec3Dot(&(vDir - vCenter) , &_vec3(0.f,1.f,0.f))) + abs(D3DXVec3Dot(&(vDir2 - vCenter) , &_vec3(0.f,1.f,0.f))) + abs(D3DXVec3Dot(&(vDir3 - vCenter) , &_vec3(0.f,1.f,0.f)));
		m_MyLength[0].z = abs(D3DXVec3Dot(&(vDir - vCenter) , &_vec3(0.f,0.f,1.f))) + abs(D3DXVec3Dot(&(vDir2 - vCenter) , &_vec3(0.f,0.f,1.f))) + abs(D3DXVec3Dot(&(vDir3 - vCenter) , &_vec3(0.f,0.f,1.f)));
	}
	

	/*multimap<float,int> multi_calc;
	if(CAL_MIN == eType)
	{
		for(_ulong i = 0; i<8; ++i)
			multi_calc.insert(make_pair(Vertex[i].x,i));

		vResult.x =multi_calc.begin()->first;
		multi_calc.clear();

		for(_ulong i = 0; i<8; ++i)
			multi_calc.insert(make_pair(Vertex[i].y,i));

		vResult.y =multi_calc.begin()->first;
		multi_calc.clear();

		for(_ulong i = 0; i<8; ++i)
			multi_calc.insert(make_pair(Vertex[i].z,i));

		vResult.z =multi_calc.begin()->first;
		multi_calc.clear();
	}
	else
	{
		for(_ulong i = 0; i<8; ++i)
			multi_calc.insert(make_pair(Vertex[i].x,i));

		vResult.x =(--multi_calc.end())->first;
		multi_calc.clear();

		for(_ulong i = 0; i<8; ++i)
			multi_calc.insert(make_pair(Vertex[i].y,i));

		vResult.y =(--multi_calc.end())->first;
		multi_calc.clear();

		for(_ulong i = 0; i<8; ++i)
			multi_calc.insert(make_pair(Vertex[i].z,i));

		vResult.z =(--multi_calc.end())->first;
		multi_calc.clear();
	}*/
	return vResult;

}



CBoxCollider* Engine::CBoxCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBoxCollider* pInst = new CBoxCollider(pGraphicDev);
	pInst->Ready_Collider();
	return pInst;
}

CComponent* Engine::CBoxCollider::Clone(void)
{
	return new CBoxCollider(*this);
}

void Engine::CBoxCollider::Free(void)
{
	Engine::CComponent::Free();

	Engine::Safe_Release(m_pGraphicDev);
	Engine::Safe_Release(m_pVB);
	Engine::Safe_Release(m_pIB);

	for(_ulong i =0; i<COL_END; ++i)
		Engine::Safe_Release(m_pTexture[i]);
}

_matrix Engine::CBoxCollider::GetNewMatExcScale(_matrix matIn)
{
	_matrix matResult, matScale;
	D3DXMatrixIdentity(&matResult);

	D3DXMatrixScaling(&matScale,D3DXVec3Length((_vec3*)&matIn.m[0][0]),D3DXVec3Length((_vec3*)&matIn.m[1][0]),D3DXVec3Length((_vec3*)&matIn.m[2][0]));
	D3DXMatrixInverse(&matScale,0,&matScale);
	matResult = matScale * matIn;
	return matResult;
}

void Engine::CBoxCollider::SetMinMaxDirect(_vec3 vMin,_vec3 vMax)
{
	m_vMin = vMin;
	m_vMax = vMax;

	_float fWidth = m_vMax.x - m_vMin.x;
	_float fHeight = m_vMax.y - m_vMin.y;
	_float fDepth = m_vMax.z - m_vMin.z;

	_matrix matScale, matTrans;
	D3DXMatrixScaling(&matScale,fWidth,fHeight,fDepth);
	D3DXMatrixTranslation(&matTrans,(m_vMin.x + m_vMax.x) * 0.5f, (m_vMin.y + m_vMax.y) * 0.5f, (m_vMin.z + m_vMax.z) * 0.5f);
	m_LocalMat = matScale * matTrans;
}


