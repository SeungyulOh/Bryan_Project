// MyFormView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyFormView.h"
#include "MainFrm.h"
#include "Default_Camera.h"
#include "Export_Engine.h"


// CMyFormView

IMPLEMENT_DYNCREATE(CMyFormView, CFormView)

CMyFormView::CMyFormView()
	: CFormView(CMyFormView::IDD)
{

}

CMyFormView::~CMyFormView()
{
}

void CMyFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
}

BEGIN_MESSAGE_MAP(CMyFormView, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMyFormView::OnTabControl)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CMyFormView 진단입니다.

#ifdef _DEBUG
void CMyFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyFormView 메시지 처리기입니다.

void CMyFormView::OnTabControl(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(true);

	int iResult = m_TabCtrl.GetCurSel();

	
	switch(iResult)
	{
	case 0:
		m_MapTool.ShowWindow(SW_SHOW);
		m_EffectTool.ShowWindow(SW_HIDE);
		/*m_UnitTool.ShowWindow(SW_HIDE);
		m_AniTool.ShowWindow(SW_HIDE);
		m_AiTool.ShowWindow(SW_HIDE);*/

		((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_pCamera->Ready_GameObject(_vec3(0.f,2000.f,-10.f),_vec3(0.f,0.f,0.f),_vec3(0.f,1.f,0.f),D3DX_PI/4.f,(_float)WINCX/WINCY,0.1f,10000.f);
		break;
	case 1:
		m_MapTool.ShowWindow(SW_HIDE);
		m_EffectTool.ShowWindow(SW_SHOW);
	/*	m_UnitTool.ShowWindow(SW_SHOW);
		m_AniTool.ShowWindow(SW_HIDE);
		m_AiTool.ShowWindow(SW_HIDE);*/
		((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_pCamera->SetCamDir(*D3DXVec3Normalize(&_vec3(),&_vec3(10.f,-20.f,0.f)));
		((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_pCamera->Ready_GameObject(_vec3(-10.f,20.f,0.f),_vec3(0.f,5.f,0.f),_vec3(0.f,1.f,0.f),D3DX_PI/4.f,(_float)WINCX/WINCY,0.1f,1000.f);
		break;
	case 2:
		m_MapTool.ShowWindow(SW_HIDE);
		/*m_UnitTool.ShowWindow(SW_HIDE);
		m_AniTool.ShowWindow(SW_SHOW);
		m_AiTool.ShowWindow(SW_HIDE);*/
		break;
	case 3:
		m_MapTool.ShowWindow(SW_HIDE);
		/*m_UnitTool.ShowWindow(SW_HIDE);
		m_AniTool.ShowWindow(SW_HIDE);
		m_AiTool.ShowWindow(SW_SHOW);*/
		break;
	}

	

	UpdateData(false);

	((CMainFrame*)AfxGetMainWnd())->m_pToolView->Invalidate(false);

	*pResult = 0;
}

void CMyFormView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_TabCtrl.InsertItem(1,L"MapTool");
	m_TabCtrl.InsertItem(2,L"EffectTool");
	m_TabCtrl.InsertItem(3,L"Not Yet");
	m_TabCtrl.InsertItem(4,L"Not Yet");

	CRect rc;
	m_TabCtrl.GetClientRect(&rc);

	m_MapTool.Create(IDD_MAPTOOL);
	m_MapTool.SetWindowPos(NULL,WINCX+25,40,rc.Width()-10,rc.Height()-30,SWP_SHOWWINDOW);
	m_MapTool.ShowWindow(SW_SHOW);

	m_EffectTool.Create(IDD_EFFECTTOOL);
	m_EffectTool.SetWindowPos(NULL,WINCX+25,40,rc.Width()-10,rc.Height()-30,SWP_SHOWWINDOW);
	m_EffectTool.ShowWindow(SW_HIDE);

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}

void CMyFormView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFormView::OnKeyDown(nChar, nRepCnt, nFlags);
}
