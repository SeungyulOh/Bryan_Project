#include "stdafx.h"
#include "Default_Camera.h"
#include "Export_Engine.h"
#include "Resource.h"
#include "MainFrm.h"

CDefault_Camera::CDefault_Camera(LPDIRECT3DDEVICE9 pGraphicDev)
:Engine::CCamera(pGraphicDev)
,iMousePointCnt(0)
{

}

CDefault_Camera::~CDefault_Camera()
{

}

HRESULT CDefault_Camera::Ready_GameObject(_vec3 vCamPos, _vec3 vTarget, _vec3 vUp, _float fFovY, _float fAspect, _float fNear, _float fFar)
{
	m_vCamPos = vCamPos;
	m_vTarget = vTarget;
	m_vUp = vUp;

	m_fAspect = fAspect;
	m_fFovY = fFovY;
	m_fNear = fNear;
	m_fFar = fFar;

	m_fDist = 20.f;
	Engine::CCamera::Ready_GameObject();

	return S_OK;
}

_int CDefault_Camera::Update_GameObject(const _float& fTimeDelta,_bool bKeyDown)
{
	KeyInput(fTimeDelta,bKeyDown);

	Engine::CCamera::Update_GameObject(fTimeDelta);

	return 0;
}

void CDefault_Camera::KeyInput(const _float& fTimeDelta,_bool bKeyDown)
{
	if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_TabCtrl.GetCurFocus() == 0)
	{
		if(bKeyDown == true)
		{
			if(Engine::Get_DIKeyState(DIK_W) & 0x80)
			{
				m_vCamPos += m_vCamDir * fTimeDelta*10.f;
				m_vTarget = m_vCamPos + m_vCamDir;
			}
			if(Engine::Get_DIKeyState(DIK_S) & 0x80)
			{
				m_vCamPos -= m_vCamDir * fTimeDelta*10.f;
				m_vTarget = m_vCamPos + m_vCamDir;
			}
			if(Engine::Get_DIKeyState(DIK_D) & 0x80)
			{
				m_vCamPos += Engine::MyGetCamPosition(m_pGraphicDev,Engine::VEC_RIGHT) * fTimeDelta*10.f;
				m_vTarget = m_vCamPos + m_vCamDir;
			}
			if(Engine::Get_DIKeyState(DIK_A) & 0x80)
			{
				m_vCamPos += -1.f *Engine::MyGetCamPosition(m_pGraphicDev,Engine::VEC_RIGHT) * fTimeDelta*10.f;
				m_vTarget = m_vCamPos + m_vCamDir;
			}

		}
		else
		{
			++iMousePointCnt;

			_long lMoveDist = 0;
			if(lMoveDist = Engine::Get_DIMouseMove(Engine::CInput_Device::DIMS_X))
			{
				_matrix matRot;
				D3DXMatrixRotationAxis(&matRot,&Engine::MyGetCamPosition(m_pGraphicDev,Engine::VEC_UP),lMoveDist/500.f);
				D3DXVec3TransformNormal(&m_vCamDir,&m_vCamDir,&matRot);

				m_vTarget = m_vCamPos + m_vCamDir;
			}

			if(lMoveDist = Engine::Get_DIMouseMove(Engine::CInput_Device::DIMS_Y))
			{
				_matrix matRot;
				D3DXMatrixRotationAxis(&matRot,&Engine::MyGetCamPosition(m_pGraphicDev,Engine::VEC_RIGHT),lMoveDist/500.f);
				D3DXVec3TransformNormal(&m_vCamDir,&m_vCamDir,&matRot);

				m_vTarget = m_vCamPos + m_vCamDir;
			}

			if(iMousePointCnt > 2)
			{
				POINT pt = {WINCX/2,WINCY/2};
				ClientToScreen(g_hWnd,&pt);
				SetCursorPos(pt.x,pt.y);

				iMousePointCnt = 0;
			}
		}
	}
	else if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_TabCtrl.GetCurFocus() == 1)
	{
		if(GetAsyncKeyState(VK_RIGHT))
		{
			_matrix matRot;
			D3DXMatrixRotationY(&matRot,0.05f);
			D3DXVec3TransformNormal(&m_vCamDir,&m_vCamDir,&matRot);
			m_vCamPos = m_vTarget - m_vCamDir * m_fDist;
		}
		if(GetAsyncKeyState(VK_LEFT))
		{
			_matrix matRot;
			D3DXMatrixRotationY(&matRot,-0.05f);
			D3DXVec3TransformNormal(&m_vCamDir,&m_vCamDir,&matRot);
			m_vCamPos = m_vTarget - m_vCamDir * m_fDist;
		}
		if(GetAsyncKeyState(VK_UP))
		{
			_matrix matRot;
			D3DXMatrixRotationAxis(&matRot,&Engine::MyGetCamPosition(m_pGraphicDev,Engine::VEC_RIGHT),0.05f);
			D3DXVec3TransformNormal(&m_vCamDir,&m_vCamDir,&matRot);
			m_vCamPos = m_vTarget - m_vCamDir * m_fDist;
		}
		if(GetAsyncKeyState(VK_DOWN))
		{
			_matrix matRot;
			D3DXMatrixRotationAxis(&matRot,&Engine::MyGetCamPosition(m_pGraphicDev,Engine::VEC_RIGHT),-0.05f);
			D3DXVec3TransformNormal(&m_vCamDir,&m_vCamDir,&matRot);
			m_vCamPos = m_vTarget - m_vCamDir * m_fDist;
		}
		if(Engine::Get_DIKeyState(DIK_W) & 0x80)
		{
			m_fDist -= fTimeDelta;
			m_vCamPos = m_vTarget - m_vCamDir * m_fDist;
		}
		if(Engine::Get_DIKeyState(DIK_S) & 0x80)
		{
			m_fDist += fTimeDelta;
			m_vCamPos = m_vTarget - m_vCamDir * m_fDist;
		}
		
	}
	

	((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.UpdateData(TRUE);
	((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.PosX = m_vCamPos.x;
	((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.PosY = m_vCamPos.y;
	((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.PosZ = m_vCamPos.z;
	((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.UpdateData(FALSE);
	
}


CDefault_Camera* CDefault_Camera::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vCamPos, _vec3 vTarget, _vec3 vUp, _float fFovY, _float fAspect, _float fNear, _float fFar)
{
	CDefault_Camera* pInst = new CDefault_Camera(pGraphicDev);
	if(FAILED(pInst->Ready_GameObject(vCamPos,vTarget,vUp,fFovY,fAspect,fNear,fFar)))
		return NULL;

	return pInst;
}

void CDefault_Camera::Free(void)
{
	Engine::CCamera::Free();
}

