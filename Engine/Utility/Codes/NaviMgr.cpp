#include "NaviMgr.h"
#include "Transform.h"

IMPLEMENT_SINGLETON(Engine::CNaviMgr)

Engine::CNaviMgr::CNaviMgr()
{

}

Engine::CNaviMgr::~CNaviMgr()
{
}

void Engine::CNaviMgr::Initialize(LPDIRECT3DDEVICE9 pDevice, DWORD dwTotalCell)
{
	m_pDevice = pDevice;
	m_vecCell.reserve(dwTotalCell);
}

void Engine::CNaviMgr::Render()
{
	for(size_t i =0; i<m_vecCell.size() ; ++i)
	{
		m_vecCell[i]->Render();
	}
}


void Engine::CNaviMgr::SetNeighbor()
{
	for(size_t i = 0; i<m_vecCell.size() ; ++i)
	{
		vector<CNaviCell*>::iterator iter = m_vecCell.begin();
		for(; iter != m_vecCell.end(); ++iter)
		{
			if(m_vecCell[i] == *iter)
				continue;

			DWORD dwCnt = ComparePoint(m_vecCell[i]->GetPoint(),(*iter)->GetPoint());
			if(dwCnt == 2)
			{
				m_vecCell[i]->SetNeighbor(*iter);
				//(*iter)->SetNeighbor(m_vecCell[i]);
			}

		}
	}
}

DWORD Engine::CNaviMgr::ComparePoint(D3DXVECTOR3* pFirstArr, D3DXVECTOR3* pSecondArr)
{
	DWORD dwCnt = 0;
	for(int i=0; i<3; ++i)
	{
		for(int j=0;j<3;++j)
		{
			if(pFirstArr[i] == pSecondArr[j])
			{
				++dwCnt;
				break;
			}
		}
	}
	return dwCnt;
}

void Engine::CNaviMgr::MoveOnNavi(CTransform* pTransform, DWORD& CurIndex,_float fTimeDelta, MOVETYPE eMove)
{
	_float fSpeed = 200.f * fTimeDelta;

	D3DXVECTOR3 vDir1 = m_vecCell[CurIndex]->GetPoint()[1] - m_vecCell[CurIndex]->GetPoint()[0];
	D3DXVECTOR3 vDir2 = m_vecCell[CurIndex]->GetPoint()[2] - m_vecCell[CurIndex]->GetPoint()[1];
	D3DXVECTOR3 vDir3 = m_vecCell[CurIndex]->GetPoint()[0] - m_vecCell[CurIndex]->GetPoint()[2];

	D3DXVec3Normalize(&vDir1,&vDir1);
	D3DXVec3Normalize(&vDir2,&vDir2);
	D3DXVec3Normalize(&vDir3,&vDir3);

	D3DXVec3Cross(&vDir1,&vDir1,&D3DXVECTOR3(0.f,1.f,0.f));
	D3DXVec3Cross(&vDir2,&vDir2,&D3DXVECTOR3(0.f,1.f,0.f));
	D3DXVec3Cross(&vDir3,&vDir3,&D3DXVECTOR3(0.f,1.f,0.f));

	_matrix matWorld = *pTransform->GetWorldMatrix();
	_vec3 vPos,vNewPos,vDir;
	memcpy(&vDir,&matWorld.m[2][0],sizeof(_float)*3);
	memcpy(&vPos,&matWorld.m[3][0],sizeof(_float)*3);
	vNewPos = vPos;

	if(eMove == MOVE_BACKWARD)
		vDir *= -1.f;

	vPos += vDir*fSpeed;

	D3DXVECTOR3 vComp1 = vPos - m_vecCell[CurIndex]->GetPoint()[0];
	D3DXVECTOR3 vComp2 = vPos - m_vecCell[CurIndex]->GetPoint()[1];
	D3DXVECTOR3 vComp3 = vPos - m_vecCell[CurIndex]->GetPoint()[2];

	D3DXVec3Normalize(&vComp1,&vComp1);
	D3DXVec3Normalize(&vComp2,&vComp2);
	D3DXVec3Normalize(&vComp3,&vComp3);

	vector<CNaviCell*> pNeighbor = *m_vecCell[CurIndex]->GetNeighbor();
	if(D3DXVec3Dot(&vDir1,&vComp1) >0 && D3DXVec3Dot(&vDir1,&vComp2) >0)
	{
		_uint iCheck = 0;
		D3DXVECTOR3 vPoint[3];
		ZeroMemory(vPoint,sizeof(D3DXVECTOR3)*3);
		vPoint[0] = m_vecCell[CurIndex]->GetPoint()[0];
		vPoint[1] = m_vecCell[CurIndex]->GetPoint()[1];

		for(size_t i=0; i<pNeighbor.size(); ++i)
		{
			DWORD dwCnt = ComparePoint(vPoint,pNeighbor[i]->GetPoint());
			if(dwCnt == 2)
				CurIndex = pNeighbor[i]->GetIndex();
			else
				++iCheck;
		}

		if(iCheck == pNeighbor.size())
		{
			_vec3 vTempDir = vPoint[1] - vPoint[0];
			D3DXVec3Normalize(&vTempDir,&vTempDir);
			_float fRatio = D3DXVec3Dot(&vTempDir,&vDir);
					
			vNewPos += vTempDir *fSpeed * fRatio;

			//////Sliding///////////
			D3DXVECTOR3 vComp1 = vNewPos - m_vecCell[CurIndex]->GetPoint()[0];
			D3DXVECTOR3 vComp2 = vNewPos - m_vecCell[CurIndex]->GetPoint()[1];
			D3DXVECTOR3 vComp3 = vNewPos - m_vecCell[CurIndex]->GetPoint()[2];
			D3DXVec3Normalize(&vComp1,&vComp1);
			D3DXVec3Normalize(&vComp2,&vComp2);
			D3DXVec3Normalize(&vComp3,&vComp3);

			if(D3DXVec3Dot(&vDir2,&vComp1) >0 && D3DXVec3Dot(&vDir2,&vComp2) >0)
			{
				D3DXVECTOR3 vPoint[3];
				ZeroMemory(vPoint,sizeof(D3DXVECTOR3)*3);
				vPoint[1] = m_vecCell[CurIndex]->GetPoint()[1];
				vPoint[2] = m_vecCell[CurIndex]->GetPoint()[2];
				for(size_t i=0; i<pNeighbor.size(); ++i)
				{
					DWORD dwCnt = ComparePoint(vPoint,pNeighbor[i]->GetPoint());
					if(dwCnt == 2)
						CurIndex = pNeighbor[i]->GetIndex();
				}
				return;
			}
			else if(D3DXVec3Dot(&vDir3,&vComp3) >0 && D3DXVec3Dot(&vDir3,&vComp1) >0)
			{
				_uint iCheck = 0;
				D3DXVECTOR3 vPoint[3];
				ZeroMemory(vPoint,sizeof(D3DXVECTOR3)*3);
				vPoint[0] = m_vecCell[CurIndex]->GetPoint()[0];
				vPoint[2] = m_vecCell[CurIndex]->GetPoint()[2];
				for(size_t i=0; i<pNeighbor.size(); ++i)
				{
					DWORD dwCnt = ComparePoint(vPoint,pNeighbor[i]->GetPoint());
					if(dwCnt == 2)
						CurIndex = pNeighbor[i]->GetIndex();
				}
				return;
			}



			pTransform->SetInfomation(Engine::CTransform::POSITION,vNewPos);
		}
			

		//첫선분통과
		return;
	}
	else if(D3DXVec3Dot(&vDir2,&vComp2) >0 && D3DXVec3Dot(&vDir2,&vComp3) >0)
	{
		_uint iCheck = 0;
		D3DXVECTOR3 vPoint[3];
		ZeroMemory(vPoint,sizeof(D3DXVECTOR3)*3);
		vPoint[1] = m_vecCell[CurIndex]->GetPoint()[1];
		vPoint[2] = m_vecCell[CurIndex]->GetPoint()[2];
		for(size_t i=0; i<pNeighbor.size(); ++i)
		{
			DWORD dwCnt = ComparePoint(vPoint,pNeighbor[i]->GetPoint());
			if(dwCnt == 2)
				CurIndex = pNeighbor[i]->GetIndex();
			else
				iCheck++;
		}
		if(iCheck == pNeighbor.size())
		{
			_vec3 vTempDir = vPoint[2] - vPoint[1];
			D3DXVec3Normalize(&vTempDir,&vTempDir);
			_float fRatio = D3DXVec3Dot(&vTempDir,&vDir);
						
			vNewPos += vTempDir *fSpeed * fRatio;

			//////Sliding///////////
			D3DXVECTOR3 vComp1 = vNewPos - m_vecCell[CurIndex]->GetPoint()[0];
			D3DXVECTOR3 vComp2 = vNewPos - m_vecCell[CurIndex]->GetPoint()[1];
			D3DXVECTOR3 vComp3 = vNewPos - m_vecCell[CurIndex]->GetPoint()[2];
			D3DXVec3Normalize(&vComp1,&vComp1);
			D3DXVec3Normalize(&vComp2,&vComp2);
			D3DXVec3Normalize(&vComp3,&vComp3);

			if(D3DXVec3Dot(&vDir1,&vComp1) >0 && D3DXVec3Dot(&vDir1,&vComp2) >0)
			{
				D3DXVECTOR3 vPoint[3];
				ZeroMemory(vPoint,sizeof(D3DXVECTOR3)*3);
				vPoint[0] = m_vecCell[CurIndex]->GetPoint()[0];
				vPoint[1] = m_vecCell[CurIndex]->GetPoint()[1];
				for(size_t i=0; i<pNeighbor.size(); ++i)
				{
					DWORD dwCnt = ComparePoint(vPoint,pNeighbor[i]->GetPoint());
					if(dwCnt == 2)
						CurIndex = pNeighbor[i]->GetIndex();
				}
				return;
			}
			else if(D3DXVec3Dot(&vDir3,&vComp3) >0 && D3DXVec3Dot(&vDir3,&vComp1) >0)
			{
				_uint iCheck = 0;
				D3DXVECTOR3 vPoint[3];
				ZeroMemory(vPoint,sizeof(D3DXVECTOR3)*3);
				vPoint[0] = m_vecCell[CurIndex]->GetPoint()[0];
				vPoint[2] = m_vecCell[CurIndex]->GetPoint()[2];
				for(size_t i=0; i<pNeighbor.size(); ++i)
				{
					DWORD dwCnt = ComparePoint(vPoint,pNeighbor[i]->GetPoint());
					if(dwCnt == 2)
						CurIndex = pNeighbor[i]->GetIndex();
				}
				return;
			}


			pTransform->SetInfomation(Engine::CTransform::POSITION,vNewPos);
		}

		return;

	}
	else if(D3DXVec3Dot(&vDir3,&vComp3) >0 && D3DXVec3Dot(&vDir3,&vComp1) >0)
	{
		_uint iCheck = 0;
		D3DXVECTOR3 vPoint[3];
		ZeroMemory(vPoint,sizeof(D3DXVECTOR3)*3);
		vPoint[0] = m_vecCell[CurIndex]->GetPoint()[0];
		vPoint[2] = m_vecCell[CurIndex]->GetPoint()[2];
		for(size_t i=0; i<pNeighbor.size(); ++i)
		{
			DWORD dwCnt = ComparePoint(vPoint,pNeighbor[i]->GetPoint());
			if(dwCnt == 2)
				CurIndex = pNeighbor[i]->GetIndex();
			else
				iCheck++;
		}
		if(iCheck == pNeighbor.size())
		{
			_vec3 vTempDir = vPoint[0] - vPoint[2];
			D3DXVec3Normalize(&vTempDir,&vTempDir);
			_float fRatio=D3DXVec3Dot(&vTempDir,&vDir);
					
			vNewPos += vTempDir *fSpeed*fRatio;

			//////Sliding///////////
			D3DXVECTOR3 vComp1 = vNewPos - m_vecCell[CurIndex]->GetPoint()[0];
			D3DXVECTOR3 vComp2 = vNewPos - m_vecCell[CurIndex]->GetPoint()[1];
			D3DXVECTOR3 vComp3 = vNewPos - m_vecCell[CurIndex]->GetPoint()[2];
			D3DXVec3Normalize(&vComp1,&vComp1);
			D3DXVec3Normalize(&vComp2,&vComp2);
			D3DXVec3Normalize(&vComp3,&vComp3);

			if(D3DXVec3Dot(&vDir1,&vComp1) >0 && D3DXVec3Dot(&vDir1,&vComp2) >0)
			{
				D3DXVECTOR3 vPoint[3];
				ZeroMemory(vPoint,sizeof(D3DXVECTOR3)*3);
				vPoint[0] = m_vecCell[CurIndex]->GetPoint()[0];
				vPoint[1] = m_vecCell[CurIndex]->GetPoint()[1];
				for(size_t i=0; i<pNeighbor.size(); ++i)
				{
					DWORD dwCnt = ComparePoint(vPoint,pNeighbor[i]->GetPoint());
					if(dwCnt == 2)
						CurIndex = pNeighbor[i]->GetIndex();
				}
				return;
			}
			else if(D3DXVec3Dot(&vDir2,&vComp1) >0 && D3DXVec3Dot(&vDir2,&vComp2) >0)
			{
				D3DXVECTOR3 vPoint[3];
				ZeroMemory(vPoint,sizeof(D3DXVECTOR3)*3);
				vPoint[1] = m_vecCell[CurIndex]->GetPoint()[1];
				vPoint[2] = m_vecCell[CurIndex]->GetPoint()[2];
				for(size_t i=0; i<pNeighbor.size(); ++i)
				{
					DWORD dwCnt = ComparePoint(vPoint,pNeighbor[i]->GetPoint());
					if(dwCnt == 2)
						CurIndex = pNeighbor[i]->GetIndex();
				}
				return;
			}

			pTransform->SetInfomation(Engine::CTransform::POSITION,vNewPos);
		}
		return;
	}

	D3DXPLANE MyPlane;
	D3DXPlaneFromPoints(&MyPlane,&m_vecCell[CurIndex]->GetPoint()[0],&m_vecCell[CurIndex]->GetPoint()[1], &m_vecCell[CurIndex]->GetPoint()[2]);
	
	// ax + by + cz + d = 0;
	// by = -ax -cz - d;
	
	float fY = -(MyPlane.a * vPos.x) - (MyPlane.c * vPos.z) - MyPlane.d;
	fY /= MyPlane.b;

	vPos.y = fY;

	pTransform->SetInfomation(Engine::CTransform::POSITION,vPos);


}

std::vector<Engine::CNaviCell*>* Engine::CNaviMgr::GetNaviVec()
{
	return &m_vecCell;
}

void Engine::CNaviMgr::AddNaviCell(D3DXVECTOR3* pPointA,D3DXVECTOR3* pPointB, D3DXVECTOR3* pPointC,bool _ToolMode)
{
	CNaviCell* pCell = CNaviCell::Create(m_pDevice,pPointA,pPointB,pPointC,_ToolMode);
	pCell->SetMyIndex(m_vecCell.size());
	m_vecCell.push_back(pCell);
}

void Engine::CNaviMgr::Free(void)
{
	for_each(m_vecCell.begin(),m_vecCell.end(),Engine::CRelease_Single());
	m_vecCell.clear();
}

