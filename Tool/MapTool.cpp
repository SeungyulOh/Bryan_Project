// MapTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool.h"
#include "MainFrm.h"
#include "Export_Engine.h"
#include "CubeObj.h"
#include "MeshObj.h"

// CMapTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(CMapTool::IDD, pParent)
	, PosX(0)
	, PosY(0)
	, PosZ(0)
	, fTargetY(0)
	, fCubeX(0)
	, fCubeY(0)
	, fCubeZ(0)
	, m_iCellCnt(0)
	, m_bAddObj(FALSE)
{
	m_pSelectedMesh = NULL;
}

CMapTool::~CMapTool()
{
	for( list<_tchar*>::iterator iter = m_NameList.begin();
		iter != m_NameList.end(); ++iter)
	{
		Engine::Safe_Delete_Array(*iter);
	}
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, PosX);
	DDX_Text(pDX, IDC_EDIT2, PosY);
	DDX_Text(pDX, IDC_EDIT3, PosZ);
	DDX_Text(pDX, IDC_EDIT4, fTargetY);
	DDX_Control(pDX, IDC_RADIO1, m_CubeControl[0]);
	DDX_Control(pDX, IDC_RADIO2, m_CubeControl[1]);
	DDX_Control(pDX, IDC_RADIO3, m_CubeControl[2]);
	DDX_Text(pDX, IDC_EDIT5, fCubeX);
	DDX_Text(pDX, IDC_EDIT6, fCubeY);
	DDX_Text(pDX, IDC_EDIT7, fCubeZ);
	DDX_Text(pDX, IDC_EDIT8, m_iCellCnt);
	DDX_Control(pDX, IDC_RADIO4, m_AxisControl[0]);
	DDX_Control(pDX, IDC_RADIO5, m_AxisControl[1]);
	DDX_Control(pDX, IDC_RADIO6, m_AxisControl[2]);
	DDX_Control(pDX, IDC_LIST1, m_ListBoxCtrl);
	DDX_Control(pDX, IDC_RADIO7, m_ModiControl[0]);
	DDX_Control(pDX, IDC_RADIO8, m_ModiControl[1]);
	DDX_Control(pDX, IDC_RADIO9, m_ModiControl[2]);
	DDX_Control(pDX, IDC_RADIO10, m_DirControl[0]);
	DDX_Control(pDX, IDC_RADIO11, m_DirControl[1]);
	DDX_Control(pDX, IDC_RADIO12, m_DirControl[2]);
	DDX_Control(pDX, IDC_LIST2, m_ObjListFromFile);
	DDX_Check(pDX, IDC_CHECK1, m_bAddObj);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_BN_CLICKED(IDC_RADIO1, &CMapTool::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_RADIO2, &CMapTool::OnBnClickedPickCube)
	ON_BN_CLICKED(IDC_RADIO3, &CMapTool::OnBnClickedMakeCell)
	ON_BN_CLICKED(IDC_RADIO4, &CMapTool::OnBnClickedX)
	ON_BN_CLICKED(IDC_RADIO5, &CMapTool::OnBnClickedY)
	ON_BN_CLICKED(IDC_RADIO6, &CMapTool::OnBnClickedZ)
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnSave)
	ON_BN_CLICKED(IDC_BUTTON2, &CMapTool::OnLoad)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_RADIO7, &CMapTool::OnBnClickedRadio7)
	ON_BN_CLICKED(IDC_RADIO8, &CMapTool::OnBnClickedRadio8)
	ON_BN_CLICKED(IDC_RADIO9, &CMapTool::OnBnClickedRadio9)
	ON_BN_CLICKED(IDC_RADIO10, &CMapTool::OnBnClickedRadio10)
	ON_BN_CLICKED(IDC_RADIO11, &CMapTool::OnBnClickedRadio11)
	ON_BN_CLICKED(IDC_RADIO12, &CMapTool::OnBnClickedRadio12)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON3, &CMapTool::OnBnSaveForObject)
	ON_BN_CLICKED(IDC_BUTTON4, &CMapTool::OnBnLoadForObject)
	ON_BN_CLICKED(IDC_BUTTON5, &CMapTool::OnBnDeleteMeshObj)
END_MESSAGE_MAP()


// CMapTool 메시지 처리기입니다.

void CMapTool::OnBnClickedAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	UpdateData(FALSE);
	::SetFocus(((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_hWnd);
}

void CMapTool::OnBnClickedPickCube()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	UpdateData(FALSE);
	::SetFocus(((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_hWnd);
}

void CMapTool::OnBnClickedMakeCell()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	UpdateData(FALSE);
	::SetFocus(((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_hWnd);
}

void CMapTool::OnBnClickedX()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	UpdateData(FALSE);


	::SetFocus(((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_hWnd);
}

void CMapTool::OnBnClickedY()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	UpdateData(FALSE);


	::SetFocus(((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_hWnd);
}

void CMapTool::OnBnClickedZ()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	UpdateData(FALSE);


	::SetFocus(((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_hWnd);
}

void CMapTool::OnSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CFileDialog		Dlg(FALSE,	// false - save, true - load
		L"dat", // 파일의 확장자명
		L"*.dat", // 최초에 띄워줄 파일 이름
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		L"*.dat", // 저장 시 밑에 띄워주는 파일 형식
		this);

	if(Dlg.DoModal() == IDCANCEL)
		return;

	TCHAR		szDirPath[MAX_PATH] = L"";

	GetCurrentDirectory(sizeof(szDirPath), szDirPath);	// vcproj 파일 위치를 얻어 옴
	// szDirPath = 0x002ef084 "c:\Users\Administrator\Desktop\FrameKor02\ToolKor02"

	SetCurrentDirectory(szDirPath);	// 응용 프로그램의 현재 작업 디렉토리를 지정된 디렉토리로 바꿔주는 함수
	// szDirPath = 0x002ef084 "c:\Users\Administrator\Desktop\FrameKor02\ToolKor02"

	PathRemoveFileSpec(szDirPath);	// 파일 이름만 남기고 마지막 경로를 삭제
	// szDirPath = 0x002ef084 "c:\Users\Administrator\Desktop\FrameKor02\ToolKor02"

	lstrcat(szDirPath, L"\\Data");
	//szDirPath = 0x002ef084 "c:\Users\Administrator\Desktop\FrameKor02\Data"


	Dlg.m_ofn.lpstrInitialDir = szDirPath;	

	DWORD	dwByte = 0;

	HANDLE	hFile  = CreateFile(Dlg.GetPathName(), 
		GENERIC_WRITE, 
		0, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);

	vector<Engine::CNaviCell*>::iterator iter = Engine::Get_NaviMgr()->GetNaviVec()->begin();
	vector<Engine::CNaviCell*>::iterator iter_end = Engine::Get_NaviMgr()->GetNaviVec()->end();

	for( ; iter != iter_end ; ++iter)
	{
		D3DXVECTOR3* pSave = (*iter)->GetPoint();
		WriteFile(hFile,pSave,sizeof(D3DXVECTOR3)*3,&dwByte,NULL);
	}

	CloseHandle(hFile);
}

void CMapTool::OnLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CFileDialog		Dlg(TRUE,	// false - save, true - load
		L"dat", // 파일의 확장자명
		L"*.dat", // 최초에 띄워줄 파일 이름
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		L"*.dat", // 저장 시 밑에 띄워주는 파일 형식
		this);

	if(Dlg.DoModal() == IDCANCEL)
		return;


	TCHAR		szDirPath[MAX_PATH] = L"";

	GetCurrentDirectory(sizeof(szDirPath), szDirPath);	// vcproj 파일 위치를 얻어 옴
	// szDirPath = 0x002ef084 "c:\Users\Administrator\Desktop\FrameKor02\ToolKor02"

	SetCurrentDirectory(szDirPath);	// 응용 프로그램의 현재 작업 디렉토리를 지정된 디렉토리로 바꿔주는 함수
	// szDirPath = 0x002ef084 "c:\Users\Administrator\Desktop\FrameKor02\ToolKor02"

	PathRemoveFileSpec(szDirPath);	// 파일 이름만 남기고 마지막 경로를 삭제
	// szDirPath = 0x002ef084 "c:\Users\Administrator\Desktop\FrameKor02\ToolKor02"

	lstrcat(szDirPath, L"\\Data");
	//szDirPath = 0x002ef084 "c:\Users\Administrator\Desktop\FrameKor02\Data"


	Dlg.m_ofn.lpstrInitialDir = szDirPath;	

	DWORD	dwByte = 0;

	HANDLE	hFile  = CreateFile(Dlg.GetPathName(), 
		GENERIC_READ, 
		0, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);

	vector<D3DXVECTOR3> m_VecLoad;

	D3DXVECTOR3 vLoad[3];
	list<CCubeObj*> CubeList;
	while(true)
	{
		ReadFile(hFile, vLoad, sizeof(D3DXVECTOR3)*3, &dwByte, NULL);

		if(dwByte == 0)
			break;

		CCubeObj* pCubeObj[3];
		for(int i=0;i<3;++i)
		{
			bool bCreateCube = false;
			list<CCubeObj*>::iterator iter_Target;
			if(CubeList.size()!=0)
			{
				for(list<CCubeObj*>::iterator iter = CubeList.begin();
					iter!=CubeList.end(); ++iter)
				{
					if(*((*iter)->GetPos()) == vLoad[i])
					{
						pCubeObj[i] = (*iter);
						bCreateCube = true;
					}
				}
			}

			if(bCreateCube == false)
			{
				pCubeObj[i] = CCubeObj::Create(((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_pGraphicDev,vLoad[i]);
				((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_CubeList.push_back(pCubeObj[i]);
				CubeList.push_back(pCubeObj[i]);
			}			
		}
		Engine::Get_NaviMgr()->AddNaviCell(pCubeObj[0]->GetPos(),pCubeObj[1]->GetPos(),pCubeObj[2]->GetPos(),true);
	}

	((CMainFrame*)AfxGetMainWnd())->m_pToolView->Invalidate(FALSE);

	CloseHandle(hFile);
}

BOOL CMapTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	UpdateData(TRUE);


	
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMapTool::OnLbnSelchangeList1()
{
	UpdateData(TRUE);

	int iSelected = m_ListBoxCtrl.GetCurSel();

	/*CString cstrSelected = L"";
	m_ListBoxCtrl.GetText(iSelected,cstrSelected);

	map<const _tchar*, CMeshObj*>::iterator iter = ((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_MapMeshObj.begin();
	map<const _tchar*, CMeshObj*>::iterator iter_end = ((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_MapMeshObj.end();

	iter = find_if(iter,iter_end,Engine::CTagFinder(cstrSelected));

	if(iter == ((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_MapMeshObj.end())
		return;*/

	m_pSelectedMesh = ((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_vecMeshObj[iSelected];

	UpdateData(FALSE);

	::SetFocus(((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_hWnd);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CMapTool::OnBnClickedRadio7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	::SetFocus(((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_hWnd);
}

void CMapTool::OnBnClickedRadio8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	::SetFocus(((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_hWnd);
}


void CMapTool::OnBnClickedRadio9()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	::SetFocus(((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_hWnd);
}

void CMapTool::OnBnClickedRadio10()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	::SetFocus(((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_hWnd);
}

void CMapTool::OnBnClickedRadio11()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	::SetFocus(((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_hWnd);
}

void CMapTool::OnBnClickedRadio12()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	::SetFocus(((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_hWnd);
}


void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	UpdateData(TRUE);

	int		iFileNumber = DragQueryFile(hDropInfo, -1, NULL, 0);
	// 1인자 : 드롭된 파일 정보를 저장하는 구조체
	// 2인자 : 파일의 인덱스를 지정, -1(0xffffffff)을 지정하면 이 함수의 반환값으로 파일 개수를 반환
	// 3인자 : 취득한 파일명을 격납하는 버퍼에의 포인터를 지정
	// 4인자 : 지정한 버퍼 사이즈

	TCHAR	szFullPath[MAX_PATH] = L"";

	for(int i = 0; i < iFileNumber; ++i)
	{
		DragQueryFile(hDropInfo, i, (LPWSTR)szFullPath, MAX_PATH);
		CFileFind finder;
		BOOL bCheck = finder.FindFile(szFullPath);
		finder.FindNextFile();
		CString cstrName = finder.GetFileTitle();

		m_ObjListFromFile.AddString(cstrName);

		_tchar* pName = new _tchar[lstrlen(cstrName)+1];
		lstrcpy(pName,cstrName);
		m_NameList.push_back(pName);

		_tchar szFullFileName[256] = L"";
		lstrcpy(szFullFileName,cstrName);
		lstrcat(szFullFileName,L".X");
		Engine::CComponent* pComponent = Engine::CStaticMesh::Create(((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_pGraphicDev,L"../Client/bin/Resources/Mesh/StaticMesh/Object/",szFullFileName);
		Engine::Add_Component(0, pName, pComponent);
	}

	UpdateData(FALSE);


	CDialog::OnDropFiles(hDropInfo);
}

void CMapTool::OnBnSaveForObject()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CFileDialog		Dlg(FALSE,	// false - save, true - load
		L"dat", // 파일의 확장자명
		L"*.dat", // 최초에 띄워줄 파일 이름
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		L"*.dat", // 저장 시 밑에 띄워주는 파일 형식
		this);

	if(Dlg.DoModal() == IDCANCEL)
		return;

	TCHAR		szDirPath[MAX_PATH] = L"";

	GetCurrentDirectory(sizeof(szDirPath), szDirPath);	// vcproj 파일 위치를 얻어 옴
	// szDirPath = 0x002ef084 "c:\Users\Administrator\Desktop\FrameKor02\ToolKor02"

	SetCurrentDirectory(szDirPath);	// 응용 프로그램의 현재 작업 디렉토리를 지정된 디렉토리로 바꿔주는 함수
	// szDirPath = 0x002ef084 "c:\Users\Administrator\Desktop\FrameKor02\ToolKor02"

	PathRemoveFileSpec(szDirPath);	// 파일 이름만 남기고 마지막 경로를 삭제
	// szDirPath = 0x002ef084 "c:\Users\Administrator\Desktop\FrameKor02\ToolKor02"

	lstrcat(szDirPath, L"\\Data");
	//szDirPath = 0x002ef084 "c:\Users\Administrator\Desktop\FrameKor02\Data"

	Dlg.m_ofn.lpstrInitialDir = szDirPath;	

	DWORD	dwByte = 0;

	HANDLE	hFile  = CreateFile(Dlg.GetPathName(), 
		GENERIC_WRITE, 
		0, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);

	

	vector<CMeshObj*>::iterator iter = ((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_vecMeshObj.begin();
	vector<CMeshObj*>::iterator iter_end = ((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_vecMeshObj.end();

	SaveMeshObj tSaveFile;
	for(; iter !=iter_end; ++iter)
	{
		lstrcpy(tSaveFile.szName, (*iter)->GetName());
		
		tSaveFile.vScale = *(*iter)->GetTransform()->GetInfomation(Engine::CTransform::SCALE);
		tSaveFile.vAngle = *(*iter)->GetTransform()->GetInfomation(Engine::CTransform::ANGLE);
		tSaveFile.vPos = *(*iter)->GetTransform()->GetInfomation(Engine::CTransform::POSITION);
		WriteFile(hFile,&tSaveFile,sizeof(SaveMeshObj),&dwByte,NULL);

	}
	


	CloseHandle(hFile);
}

void CMapTool::OnBnLoadForObject()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CFileDialog		Dlg(TRUE,	// false - save, true - load
		L"dat", // 파일의 확장자명
		L"*.dat", // 최초에 띄워줄 파일 이름
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		L"*.dat", // 저장 시 밑에 띄워주는 파일 형식
		this);

	if(Dlg.DoModal() == IDCANCEL)
		return;


	TCHAR		szDirPath[MAX_PATH] = L"";

	GetCurrentDirectory(sizeof(szDirPath), szDirPath);	// vcproj 파일 위치를 얻어 옴
	// szDirPath = 0x002ef084 "c:\Users\Administrator\Desktop\FrameKor02\ToolKor02"

	SetCurrentDirectory(szDirPath);	// 응용 프로그램의 현재 작업 디렉토리를 지정된 디렉토리로 바꿔주는 함수
	// szDirPath = 0x002ef084 "c:\Users\Administrator\Desktop\FrameKor02\ToolKor02"

	PathRemoveFileSpec(szDirPath);	// 파일 이름만 남기고 마지막 경로를 삭제
	// szDirPath = 0x002ef084 "c:\Users\Administrator\Desktop\FrameKor02\ToolKor02"

	lstrcat(szDirPath, L"\\Data");
	//szDirPath = 0x002ef084 "c:\Users\Administrator\Desktop\FrameKor02\Data"


	Dlg.m_ofn.lpstrInitialDir = szDirPath;	

	DWORD	dwByte = 0;

	HANDLE	hFile  = CreateFile(Dlg.GetPathName(), 
		GENERIC_READ, 
		0, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);

	SaveMeshObj tLoadFile;
	while(true)
	{
		ReadFile(hFile, &tLoadFile, sizeof(SaveMeshObj), &dwByte, NULL);

		if(dwByte == 0)
			break;

		_tchar* pName = new _tchar[lstrlen(tLoadFile.szName)+1];
		lstrcpy(pName,tLoadFile.szName);
		m_NameList.push_back(pName);

		_tchar szFullFileName[256] = L"";
		lstrcpy(szFullFileName,pName);
		lstrcat(szFullFileName,L".X");

		
		Engine::CComponent* pComponent_Found = Engine::Find_Component(0,pName);
		if(pComponent_Found == NULL)
		{
			Engine::CComponent* pComponent = Engine::CStaticMesh::Create(((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_pGraphicDev,L"../Client/bin/Resources/Mesh/StaticMesh/Object/",szFullFileName);
			Engine::Add_Component(0, pName, pComponent);
		}
		
		CMeshObj* pObj = CMeshObj::Create(((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_pGraphicDev,pName,tLoadFile.vPos);
		pObj->GetTransform()->SetInfomation(Engine::CTransform::SCALE,tLoadFile.vScale);
		pObj->GetTransform()->SetInfomation(Engine::CTransform::ANGLE,tLoadFile.vAngle);
		pObj->GetTransform()->SetInfomation(Engine::CTransform::POSITION,tLoadFile.vPos);
		pObj->GetTransform()->Update_Component(0.f);

		((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_vecMeshObj.push_back(pObj);
		((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool.m_ListBoxCtrl.AddString(pName);

	}

	((CMainFrame*)AfxGetMainWnd())->m_pToolView->Invalidate(FALSE);

	CloseHandle(hFile);
}

void CMapTool::OnBnDeleteMeshObj()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	_uint iIndex = m_ListBoxCtrl.GetCurSel();

	if(iIndex < ((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_vecMeshObj.size())
	{
		Engine::Safe_Release(((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_vecMeshObj[iIndex]);


		vector<CMeshObj*>::iterator iter = find(((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_vecMeshObj.begin(), ((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_vecMeshObj.end(), ((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_vecMeshObj[iIndex]);
		if(iter == ((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_vecMeshObj.end())
			return;

		((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_vecMeshObj.erase(iter);
		m_ListBoxCtrl.DeleteString(iIndex);
	}
	UpdateData(FALSE);
	((CMainFrame*)AfxGetMainWnd())->m_pToolView->Invalidate(FALSE);
}
