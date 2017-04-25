
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
#include "Tool.h"

#include "MainFrm.h"
#include "ToolDoc.h"
#include "ToolView.h"
#include "Export_Engine.h"
#include "BossMap.h"
#include "Default_Camera.h"
#include "CubeObj.h"
#include "MeshObj.h"
#include "EffectBack.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToolView

IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CToolView::OnFilePrintPreview)
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CToolView 생성/소멸

HWND g_hWnd;
CToolView::CToolView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_iCellCnt = 0;
	bCamFocus = TRUE;

}

CToolView::~CToolView()
{

	for_each(m_vecMeshObj.begin(),m_vecMeshObj.end(),Engine::CRelease_Single());
	m_vecMeshObj.clear();

	Engine::Safe_Release(m_pBossMap);
	Engine::Safe_Release(m_pCamera);
	Engine::Safe_Release(m_pEffectBack);
	Engine::Safe_Release(m_pGraphicDev);
	

	list<CCubeObj*>::iterator iter = m_CubeList.begin();
	list<CCubeObj*>::iterator iter_end = m_CubeList.end();
	for( ; iter != iter_end ; ++iter)
	{
		Engine::Safe_Release(*iter);
	}
	m_CubeList.clear();

	Engine::Release_Utility();
	Engine::Release_System();
	
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}



// CToolView 그리기

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	Engine::Render_Begin(D3DXCOLOR(0.2f,0.2f,0.2f,1.f));

	if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_TabCtrl.GetCurFocus() == 0)
	{
		m_pBossMap->Render_GameObject();

		for(vector<CMeshObj*>::iterator iter = m_vecMeshObj.begin(); 
			iter != m_vecMeshObj.end() ; ++iter)
		{
			(*iter)->Render_GameObject();
		}

		list<CCubeObj*>::iterator iter = m_CubeList.begin();
		list<CCubeObj*>::iterator iter_end = m_CubeList.end();
		for( ; iter != iter_end ; ++iter)
		{
			(*iter)->Render_GameObject();
		}
		Engine::Get_NaviMgr()->Render();
	}
	else if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_TabCtrl.GetCurFocus() == 1)
	{
		m_pEffectBack->Render_GameObject();
	}


	Engine::Render_End();

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CToolView 인쇄

void CToolView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CToolView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CToolView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기

void CToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	g_hWnd = m_hWnd;

	Engine::Ready_GraphicDev(m_hWnd,Engine::CGraphicDev::MODE_WIN,WINCX,WINCY);
	m_pGraphicDev = Engine::Get_GraphicDev();
	m_pGraphicDev->AddRef();

	CMainFrame*	pMainFrm = (CMainFrame*)AfxGetMainWnd();

	RECT rc_window = {};
	pMainFrm->GetWindowRect(&rc_window);

	SetRect(&rc_window,0,0,rc_window.right-rc_window.left,rc_window.bottom-rc_window.top);

	RECT rc_view = {};
	GetClientRect(&rc_view);

	float fColFrm = float(rc_window.right-rc_view.right);
	float fRowFrm = float(rc_window.bottom-rc_view.bottom);

	pMainFrm->SetWindowPos(NULL,100,100,
		int(WINCX+fColFrm),int(WINCY+fRowFrm),SWP_NOZORDER);

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	Ready_Component();
	Ready_DefaultSetting();

	Engine::Get_NaviMgr()->Initialize(m_pGraphicDev,50);
		
}


void CToolView::Ready_Component(void)
{
	Engine::Ready_ComManager(1);

	Engine::CComponent* pComponent = Engine::CRenderer::Create(m_pGraphicDev);
	Engine::Add_Component(0,L"Component_Renderer",pComponent);
	Engine::Set_Renderer((Engine::CRenderer*)pComponent);

	//Transform
	pComponent = Engine::CTransform::Create(m_pGraphicDev);
	Engine::Add_Component(0,L"Component_Transform",pComponent);

	//Buffer
	pComponent = Engine::CRect_Texture::Create(m_pGraphicDev);
	Engine::Add_Component(0,L"Component_Buffer_RectTexture",pComponent);

	//// For.CubeTex Buffer
	pComponent = Engine::CCube_Tex::Create(m_pGraphicDev);
	Engine::Add_Component(0, L"Component_Buffer_CubeTex", pComponent);

	////For. Material Loading
	pComponent = Engine::CMaterial::Create(m_pGraphicDev, D3DXCOLOR(1.f,1.f,1.f,1.f), D3DXCOLOR(1.f,1.f,1.f,1.f), D3DXCOLOR(1.f,1.f,1.f,1.f), D3DXCOLOR(0.f,0.f,0.f,0.f),1.f);
	Engine::Add_Component(0, L"Component_Material", pComponent);

	// For.Meshes Loading
	pComponent = Engine::CStaticMesh::Create(m_pGraphicDev,L"../Client/bin/Resources/Mesh/StaticMesh/Map/",L"Kadan_v4.X");
	Engine::Add_Component(0, L"Component_Mesh_BossMap", pComponent);

	//Shader_water
	pComponent = Engine::CShader::Create(m_pGraphicDev,L"../Client/Codes/Shader_Water.hpp");
	Engine::Add_Component(0,L"Component_Shader_Water",pComponent);

	//Shader_Mesh
	pComponent = Engine::CShader::Create(m_pGraphicDev,L"../Client/Codes/Shader_Mesh.hpp");
	Engine::Add_Component(0,L"Component_Shader_Mesh",pComponent);

	//Shader_Effect
	pComponent = Engine::CShader::Create(m_pGraphicDev,L"../Client/Codes/Shader_Effect.hpp");
	Engine::Add_Component(0,L"Component_Shader_Effect",pComponent);

	//Shader_Mesh_Phong
	pComponent = Engine::CShader::Create(m_pGraphicDev,L"../Client/Codes/Shader_Mesh_Phong.hpp");
	Engine::Add_Component(0,L"Component_Shader_Mesh_Phong",pComponent);

	//TrailBuffer
	pComponent = Engine::CTrail_Texture::Create(m_pGraphicDev);
	Engine::Add_Component(0,L"Component_Buffer_TrailTexture",pComponent);

	////For.Trail
	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Client/bin/Resources/Texture/Trail/trail01.tga", Engine::CTexture::TYPE_NORMAL);
	Engine::Add_Component(0, L"Component_Texture_PhantomTrail", pComponent);

	_matrix matInput;
	D3DXMatrixRotationY(&matInput,D3DX_PI/2.f);
	pComponent = Engine::CDynamicMesh::Create(m_pGraphicDev,L"../Client/bin/Resources/Mesh/DynamicMesh/Hagie/",L"Hagie_bin.X",matInput);
	Engine::Add_Component(0, L"Component_Mesh_Hagie", pComponent);
}

void CToolView::Ready_DefaultSetting(void)
{
	m_pBossMap = CBossMap::Create(m_pGraphicDev);
	
	m_pCamera = CDefault_Camera::Create(m_pGraphicDev,_vec3(0.f,3000.f,0.f),_vec3(0.f,0.f,0.f),_vec3(0.f,1.f,0.f),D3DX_PI/4.f,(_float)WINCX/WINCY,0.1f,10000.f);

	m_pEffectBack = CEffectBack::Create(m_pGraphicDev);

	D3DLIGHT9 tLightInfo;
	ZeroMemory(&tLightInfo,sizeof(D3DLIGHT9));
	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;
	D3DXCOLOR color = D3DXCOLOR(1.f,1.f,1.f,1.f);
	tLightInfo.Diffuse = color;
	tLightInfo.Ambient = color;
	tLightInfo.Specular = color;
	tLightInfo.Direction = _vec3(0.f,-1.f,-2.f);

	Engine::AddLight(m_pGraphicDev,tLightInfo);
	Engine::LightEnable(m_pGraphicDev,0);

	
	Engine::Ready_InputDevice(AfxGetInstanceHandle(),g_hWnd);
}

//void CToolView::OnMouseMove(UINT nFlags, CPoint point)
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//
//	/*Engine::SetUp_InputState();
//
//	((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.UpdateData(TRUE);
//	
//	if(bCamFocus == true)
//		m_pCamera->Update_GameObject(0.01f,false);
//
//	
//
//	((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.UpdateData(FALSE);
//	Invalidate(FALSE);*/
//	
//	//CView::OnMouseMove(nFlags, point);
//}

void CToolView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	Engine::SetUp_InputState();

	((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.UpdateData(TRUE);

	if(GetAsyncKeyState(VK_RETURN) & 0x0001)
		bCamFocus = !bCamFocus;
	else if(GetAsyncKeyState('1'))
	{
		if(CCubeObj::m_pSelectedCube != NULL)
		{
			if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_AxisControl[0].GetCheck() == TRUE)
			{
				CCubeObj::m_pSelectedCube->m_vPos.x += 1.f;
				((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.fCubeX = CCubeObj::m_pSelectedCube->m_vPos.x;
				
				for(int i=0; i<8;++i)
					CCubeObj::m_pSelectedCube->m_pVertex[i].vPos.x += 1.f;
			}
			else if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_AxisControl[1].GetCheck() == TRUE)
			{
				CCubeObj::m_pSelectedCube->m_vPos.y += 1.f;
				((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.fCubeY = CCubeObj::m_pSelectedCube->m_vPos.y;

				for(int i=0; i<8;++i)
					CCubeObj::m_pSelectedCube->m_pVertex[i].vPos.y += 1.f;
			}
			else if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_AxisControl[2].GetCheck() == TRUE)
			{
				CCubeObj::m_pSelectedCube->m_vPos.z += 1.f;
				((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.fCubeZ = CCubeObj::m_pSelectedCube->m_vPos.z;

				for(int i=0; i<8;++i)
					CCubeObj::m_pSelectedCube->m_pVertex[i].vPos.z += 1.f;
			}
			CCubeObj::m_pSelectedCube->UpdateWorldMatrix();

		}


	}
	else if(GetAsyncKeyState('2'))
	{
		if(CCubeObj::m_pSelectedCube != NULL)
		{
			if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_AxisControl[0].GetCheck() == TRUE)
			{
				CCubeObj::m_pSelectedCube->m_vPos.x -= 1.f;
				((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.fCubeX = CCubeObj::m_pSelectedCube->m_vPos.x;

				for(int i=0; i<8;++i)
					CCubeObj::m_pSelectedCube->m_pVertex[i].vPos.x -= 1.f;
			}
			else if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_AxisControl[1].GetCheck() == TRUE)
			{
				CCubeObj::m_pSelectedCube->m_vPos.y -= 1.f;
				((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.fCubeY = CCubeObj::m_pSelectedCube->m_vPos.y;

				for(int i=0; i<8;++i)
					CCubeObj::m_pSelectedCube->m_pVertex[i].vPos.y -= 1.f;
			}
			else if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_AxisControl[2].GetCheck() == TRUE)
			{
				CCubeObj::m_pSelectedCube->m_vPos.z -= 1.f;
				((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.fCubeZ = CCubeObj::m_pSelectedCube->m_vPos.z;

				for(int i=0; i<8;++i)
					CCubeObj::m_pSelectedCube->m_pVertex[i].vPos.x -= 1.f;
			}

			CCubeObj::m_pSelectedCube->UpdateWorldMatrix();
		}
	}
	else if(GetAsyncKeyState('3'))
	{
		if(NULL != ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh)
		{

			//decrease

			if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_ModiControl[0].GetCheck() == TRUE)
			{
				_vec3* pScale = ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->GetInfomation(Engine::CTransform::SCALE);
				if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_DirControl[0].GetCheck() == TRUE)
				{
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->SetInfomation(Engine::CTransform::SCALE,*pScale - _vec3(0.1f,0.f,0.f));
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->Update_Component(0.f);
				}
				else if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_DirControl[1].GetCheck() == TRUE)
				{
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->SetInfomation(Engine::CTransform::SCALE,*pScale - _vec3(0.0f,0.1f,0.f));
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->Update_Component(0.f);
				}
				else if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_DirControl[2].GetCheck() == TRUE)
				{
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->SetInfomation(Engine::CTransform::SCALE,*pScale - _vec3(0.0f,0.0f,0.1f));
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->Update_Component(0.f);
				}
			}
			else if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_ModiControl[1].GetCheck() == TRUE)
			{
				_vec3* pAngle = ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->GetInfomation(Engine::CTransform::ANGLE);
				if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_DirControl[0].GetCheck() == TRUE)
				{
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->SetInfomation(Engine::CTransform::ANGLE,*pAngle - _vec3(0.01f,0.f,0.f));
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->Update_Component(0.f);
				}
				else if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_DirControl[1].GetCheck() == TRUE)
				{
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->SetInfomation(Engine::CTransform::ANGLE,*pAngle - _vec3(0.0f,0.01f,0.f));
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->Update_Component(0.f);
				}
				else if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_DirControl[2].GetCheck() == TRUE)
				{
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->SetInfomation(Engine::CTransform::ANGLE,*pAngle - _vec3(0.0f,0.0f,0.01f));
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->Update_Component(0.f);
				}
			}
			else if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_ModiControl[2].GetCheck() == TRUE)
			{
				_vec3* pTrans = ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->GetInfomation(Engine::CTransform::POSITION);
				if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_DirControl[0].GetCheck() == TRUE)
				{
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->SetInfomation(Engine::CTransform::POSITION,*pTrans - _vec3(1.f,0.f,0.f));
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->Update_Component(0.f);
				}
				else if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_DirControl[1].GetCheck() == TRUE)
				{
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->SetInfomation(Engine::CTransform::POSITION,*pTrans - _vec3(0.0f,1.f,0.f));
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->Update_Component(0.f);
				}
				else if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_DirControl[2].GetCheck() == TRUE)
				{
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->SetInfomation(Engine::CTransform::POSITION,*pTrans - _vec3(0.0f,0.0f,1.f));
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->Update_Component(0.f);
				}
			}
		}
	}
	else if(GetAsyncKeyState('4'))
	{
		if(NULL != ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh)
		{
			
			//increase

			if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_ModiControl[0].GetCheck() == TRUE)
			{
				_vec3* pScale = ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->GetInfomation(Engine::CTransform::SCALE);
				if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_DirControl[0].GetCheck() == TRUE)
				{
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->SetInfomation(Engine::CTransform::SCALE,*pScale + _vec3(0.1f,0.f,0.f));
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->Update_Component(0.f);
				}
				else if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_DirControl[1].GetCheck() == TRUE)
				{
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->SetInfomation(Engine::CTransform::SCALE,*pScale + _vec3(0.0f,0.1f,0.f));
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->Update_Component(0.f);
				}
				else if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_DirControl[2].GetCheck() == TRUE)
				{
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->SetInfomation(Engine::CTransform::SCALE,*pScale + _vec3(0.0f,0.0f,0.1f));
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->Update_Component(0.f);
				}
			}
			else if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_ModiControl[1].GetCheck() == TRUE)
			{
				_vec3* pAngle = ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->GetInfomation(Engine::CTransform::ANGLE);
				if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_DirControl[0].GetCheck() == TRUE)
				{
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->SetInfomation(Engine::CTransform::ANGLE,*pAngle + _vec3(0.01f,0.f,0.f));
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->Update_Component(0.f);
				}
				else if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_DirControl[1].GetCheck() == TRUE)
				{
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->SetInfomation(Engine::CTransform::ANGLE,*pAngle + _vec3(0.0f,0.01f,0.f));
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->Update_Component(0.f);
				}
				else if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_DirControl[2].GetCheck() == TRUE)
				{
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->SetInfomation(Engine::CTransform::ANGLE,*pAngle + _vec3(0.0f,0.0f,0.01f));
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->Update_Component(0.f);
				}
			}
			else if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_ModiControl[2].GetCheck() == TRUE)
			{
				_vec3* pTrans = ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->GetInfomation(Engine::CTransform::POSITION);
				if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_DirControl[0].GetCheck() == TRUE)
				{
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->SetInfomation(Engine::CTransform::POSITION,*pTrans + _vec3(1.f,0.f,0.f));
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->Update_Component(0.f);
				}
				else if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_DirControl[1].GetCheck() == TRUE)
				{
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->SetInfomation(Engine::CTransform::POSITION,*pTrans + _vec3(0.0f,1.f,0.f));
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->Update_Component(0.f);
				}
				else if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_DirControl[2].GetCheck() == TRUE)
				{
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->SetInfomation(Engine::CTransform::POSITION,*pTrans + _vec3(0.0f,0.0f,1.f));
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_pSelectedMesh->GetTransform()->Update_Component(0.f);
				}
			}
		}
	}
	else
	{
		m_pCamera->Update_GameObject(5.f,true);
	}
	
	((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.UpdateData(FALSE);
	Invalidate(FALSE);

	//CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnLButtonDown(nFlags, point);

	((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.UpdateData(TRUE);


	BOOL bAddCheck = ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_CubeControl[0].GetCheck();
	BOOL bPickCube = ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_CubeControl[1].GetCheck();
	BOOL bMakeCube = ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_CubeControl[2].GetCheck();
	BOOL bAddObj = ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_bAddObj;

	if(bAddCheck == TRUE && GetAsyncKeyState(VK_LBUTTON) & 0x0001)
		AddCube(point);	
	else if(bPickCube == TRUE && GetAsyncKeyState(VK_LBUTTON) & 0x0001)
		PickCube(point);
	else if(bMakeCube == TRUE && GetAsyncKeyState(VK_LBUTTON) & 0x0001)
		PickCube(point);
	else if(bAddObj == TRUE && GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		AddObject(point);

	((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.UpdateData(FALSE);


	Invalidate(FALSE);
}

void CToolView::AddCube(CPoint point)
{
	D3DXMATRIX matViewPort,matView,matProj;
	D3DXMatrixIdentity(&matViewPort);
	matViewPort._11 = WINCX/2;
	matViewPort._22 = -WINCY/2;
	matViewPort._33 = 1;
	matViewPort._41 = WINCX/2;
	matViewPort._42 = WINCY/2;
	matViewPort._43 = 0.f;
	matViewPort._44 = 1.f;
	m_pGraphicDev->GetTransform(D3DTS_VIEW,&matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION,&matProj);
	D3DXMatrixInverse(&matViewPort,0,&matViewPort);
	D3DXMatrixInverse(&matView,0,&matView);
	D3DXMatrixInverse(&matProj,0,&matProj);

	float fX = (float)point.x;
	float fY = (float)point.y;
	D3DXVECTOR3 vMousePos = D3DXVECTOR3(fX,fY,0.f);

	D3DXVec3TransformCoord(&vMousePos,&vMousePos,&matViewPort);
	D3DXVec3TransformCoord(&vMousePos,&vMousePos,&matProj);

	D3DXVECTOR3 vRayPos,vRayDir;
	vRayPos = D3DXVECTOR3(0.f,0.f,0.f);
	vRayDir = vMousePos;
	D3DXVec3Normalize(&vRayDir,&vRayDir);

	D3DXVec3TransformCoord(&vRayPos,&vRayPos,&matView);
	D3DXVec3TransformNormal(&vRayDir,&vRayDir,&matView);

	float fTargetY = ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.fTargetY;
	float fDiff = vRayPos.y - fTargetY;
	vRayDir = (vRayDir / abs(vRayDir.y));
	vRayDir *= abs(fDiff);

	D3DXVECTOR3 vCubePos = vRayPos + vRayDir;
	if(vCubePos.y != fTargetY)
		return;

	CCubeObj* pCubeObj = CCubeObj::Create(m_pGraphicDev,vCubePos);
	m_CubeList.push_back(pCubeObj);
	Invalidate(false);
}

void CToolView::PickCube(CPoint point)
{
	D3DXMATRIX matViewPort,matView,matProj;
	D3DXMatrixIdentity(&matViewPort);
	matViewPort._11 = WINCX/2;
	matViewPort._22 = -WINCY/2;
	matViewPort._33 = 1;
	matViewPort._41 = WINCX/2;
	matViewPort._42 = WINCY/2;
	matViewPort._43 = 0.f;
	matViewPort._44 = 1.f;
	m_pGraphicDev->GetTransform(D3DTS_VIEW,&matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION,&matProj);
	D3DXMatrixInverse(&matViewPort,0,&matViewPort);
	D3DXMatrixInverse(&matView,0,&matView);
	D3DXMatrixInverse(&matProj,0,&matProj);

	float fX = (float)point.x;
	float fY = (float)point.y;
	D3DXVECTOR3 vMousePos = D3DXVECTOR3(fX,fY,0.f);

	D3DXVec3TransformCoord(&vMousePos,&vMousePos,&matViewPort);
	D3DXVec3TransformCoord(&vMousePos,&vMousePos,&matProj);

	D3DXVECTOR3 vRayPos,vRayDir;
	vRayPos = D3DXVECTOR3(0.f,0.f,0.f);
	vRayDir = vMousePos;
	D3DXVec3Normalize(&vRayDir,&vRayDir);

	D3DXVec3TransformCoord(&vRayPos,&vRayPos,&matView);
	D3DXVec3TransformNormal(&vRayDir,&vRayDir,&matView);

	list<CCubeObj*>::iterator iter = m_CubeList.begin();
	list<CCubeObj*>::iterator iter_end = m_CubeList.end();
	for( ; iter != iter_end ; ++iter)
	{
		for(int i=0; i<12; ++i)
		{
			float fU,fV,fDist;
			if(D3DXIntersectTri(&(*iter)->m_pVertex[(*iter)->m_pIndex[i]._0].vPos,
				&(*iter)->m_pVertex[(*iter)->m_pIndex[i]._1].vPos,
				&(*iter)->m_pVertex[(*iter)->m_pIndex[i]._2].vPos,
				&vRayPos,&vRayDir,&fU,&fV,&fDist))
			{
				D3DXVECTOR3 vCenter = *(*iter)->GetPos();

				((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.fCubeX = vCenter.x;
				((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.fCubeY = vCenter.y;
				((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.fCubeZ = vCenter.z;

				if(CCubeObj::m_pSelectedCube != (*iter) && 
					((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_CubeControl[2].GetCheck() == TRUE)
					m_SelectedPointVec.push_back((*iter)->GetPos());

				CCubeObj::m_pSelectedCube = (*iter);

				break;
			}
		}
	}


	if(m_SelectedPointVec.size() >= 3 && 
		((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_CubeControl[2].GetCheck() == TRUE)
	{
		Engine::Get_NaviMgr()->AddNaviCell(m_SelectedPointVec[0],m_SelectedPointVec[1],m_SelectedPointVec[2],true);
		Engine::Get_NaviMgr()->Render();
		Invalidate(false);
		m_SelectedPointVec.clear();
		++m_iCellCnt;
	}

	((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_iCellCnt = m_iCellCnt;


}

void CToolView::AddObject(CPoint point)
{
	int iCheck = ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_ObjListFromFile.GetCurSel();
	if(iCheck & 0x8000)
		return;

	CString cstrSelName;
	((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_ObjListFromFile.GetText(iCheck,cstrSelName);

	D3DXMATRIX matViewPort,matView,matProj;
	D3DXMatrixIdentity(&matViewPort);
	matViewPort._11 = WINCX/2;
	matViewPort._22 = -WINCY/2;
	matViewPort._33 = 1;
	matViewPort._41 = WINCX/2;
	matViewPort._42 = WINCY/2;
	matViewPort._43 = 0.f;
	matViewPort._44 = 1.f;
	m_pGraphicDev->GetTransform(D3DTS_VIEW,&matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION,&matProj);
	D3DXMatrixInverse(&matViewPort,0,&matViewPort);
	D3DXMatrixInverse(&matView,0,&matView);
	D3DXMatrixInverse(&matProj,0,&matProj);

	float fX = (float)point.x;
	float fY = (float)point.y;
	D3DXVECTOR3 vMousePos = D3DXVECTOR3(fX,fY,0.f);

	D3DXVec3TransformCoord(&vMousePos,&vMousePos,&matViewPort);
	D3DXVec3TransformCoord(&vMousePos,&vMousePos,&matProj);

	D3DXVECTOR3 vRayPos,vRayDir;
	vRayPos = D3DXVECTOR3(0.f,0.f,0.f);
	vRayDir = vMousePos;
	D3DXVec3Normalize(&vRayDir,&vRayDir);

	D3DXVec3TransformCoord(&vRayPos,&vRayPos,&matView);
	D3DXVec3TransformNormal(&vRayDir,&vRayDir,&matView);

	float fTargetY = ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.fTargetY;
	float fDiff = vRayPos.y - fTargetY;
	vRayDir = (vRayDir / abs(vRayDir.y));
	vRayDir *= abs(fDiff);

	_vec3 vFirstPos = vRayPos + vRayDir;
	if(vFirstPos.y != fTargetY)
		return;


	/*map<const _tchar*, CMeshObj*>::iterator iter = m_MapMeshObj.begin();
	map<const _tchar*, CMeshObj*>::iterator iter_end = m_MapMeshObj.end();

	map<const _tchar*, CMeshObj*>::iterator iter_found = find_if(iter,iter_end,Engine::CTagFinder(cstrSelName));

	if(iter_found == m_MapMeshObj.end())*/
	{
		CMeshObj* pObj = CMeshObj::Create(m_pGraphicDev,cstrSelName,vFirstPos);
		m_vecMeshObj.push_back(pObj);
		((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_ListBoxCtrl.AddString(cstrSelName);
	}
}
