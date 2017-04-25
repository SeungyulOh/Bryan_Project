// MapTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool.h"
#include "MainFrm.h"
#include "Export_Engine.h"
#include "CubeObj.h"
#include "MeshObj.h"

// CMapTool ��ȭ �����Դϴ�.

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


// CMapTool �޽��� ó�����Դϴ�.

void CMapTool::OnBnClickedAdd()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	UpdateData(FALSE);
	::SetFocus(((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_hWnd);
}

void CMapTool::OnBnClickedPickCube()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	UpdateData(FALSE);
	::SetFocus(((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_hWnd);
}

void CMapTool::OnBnClickedMakeCell()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	UpdateData(FALSE);
	::SetFocus(((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_hWnd);
}

void CMapTool::OnBnClickedX()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	UpdateData(FALSE);


	::SetFocus(((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_hWnd);
}

void CMapTool::OnBnClickedY()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	UpdateData(FALSE);


	::SetFocus(((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_hWnd);
}

void CMapTool::OnBnClickedZ()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	UpdateData(FALSE);


	::SetFocus(((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_hWnd);
}

void CMapTool::OnSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CFileDialog		Dlg(FALSE,	// false - save, true - load
		L"dat", // ������ Ȯ���ڸ�
		L"*.dat", // ���ʿ� ����� ���� �̸�
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		L"*.dat", // ���� �� �ؿ� ����ִ� ���� ����
		this);

	if(Dlg.DoModal() == IDCANCEL)
		return;

	TCHAR		szDirPath[MAX_PATH] = L"";

	GetCurrentDirectory(sizeof(szDirPath), szDirPath);	// vcproj ���� ��ġ�� ��� ��
	// szDirPath = 0x002ef084 "c:\Users\Administrator\Desktop\FrameKor02\ToolKor02"

	SetCurrentDirectory(szDirPath);	// ���� ���α׷��� ���� �۾� ���丮�� ������ ���丮�� �ٲ��ִ� �Լ�
	// szDirPath = 0x002ef084 "c:\Users\Administrator\Desktop\FrameKor02\ToolKor02"

	PathRemoveFileSpec(szDirPath);	// ���� �̸��� ����� ������ ��θ� ����
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CFileDialog		Dlg(TRUE,	// false - save, true - load
		L"dat", // ������ Ȯ���ڸ�
		L"*.dat", // ���ʿ� ����� ���� �̸�
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		L"*.dat", // ���� �� �ؿ� ����ִ� ���� ����
		this);

	if(Dlg.DoModal() == IDCANCEL)
		return;


	TCHAR		szDirPath[MAX_PATH] = L"";

	GetCurrentDirectory(sizeof(szDirPath), szDirPath);	// vcproj ���� ��ġ�� ��� ��
	// szDirPath = 0x002ef084 "c:\Users\Administrator\Desktop\FrameKor02\ToolKor02"

	SetCurrentDirectory(szDirPath);	// ���� ���α׷��� ���� �۾� ���丮�� ������ ���丮�� �ٲ��ִ� �Լ�
	// szDirPath = 0x002ef084 "c:\Users\Administrator\Desktop\FrameKor02\ToolKor02"

	PathRemoveFileSpec(szDirPath);	// ���� �̸��� ����� ������ ��θ� ����
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

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	UpdateData(TRUE);


	
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CMapTool::OnBnClickedRadio7()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	::SetFocus(((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_hWnd);
}

void CMapTool::OnBnClickedRadio8()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	::SetFocus(((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_hWnd);
}


void CMapTool::OnBnClickedRadio9()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	::SetFocus(((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_hWnd);
}

void CMapTool::OnBnClickedRadio10()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	::SetFocus(((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_hWnd);
}

void CMapTool::OnBnClickedRadio11()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	::SetFocus(((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_hWnd);
}

void CMapTool::OnBnClickedRadio12()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	::SetFocus(((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_hWnd);
}


void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	UpdateData(TRUE);

	int		iFileNumber = DragQueryFile(hDropInfo, -1, NULL, 0);
	// 1���� : ��ӵ� ���� ������ �����ϴ� ����ü
	// 2���� : ������ �ε����� ����, -1(0xffffffff)�� �����ϸ� �� �Լ��� ��ȯ������ ���� ������ ��ȯ
	// 3���� : ����� ���ϸ��� �ݳ��ϴ� ���ۿ��� �����͸� ����
	// 4���� : ������ ���� ������

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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CFileDialog		Dlg(FALSE,	// false - save, true - load
		L"dat", // ������ Ȯ���ڸ�
		L"*.dat", // ���ʿ� ����� ���� �̸�
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		L"*.dat", // ���� �� �ؿ� ����ִ� ���� ����
		this);

	if(Dlg.DoModal() == IDCANCEL)
		return;

	TCHAR		szDirPath[MAX_PATH] = L"";

	GetCurrentDirectory(sizeof(szDirPath), szDirPath);	// vcproj ���� ��ġ�� ��� ��
	// szDirPath = 0x002ef084 "c:\Users\Administrator\Desktop\FrameKor02\ToolKor02"

	SetCurrentDirectory(szDirPath);	// ���� ���α׷��� ���� �۾� ���丮�� ������ ���丮�� �ٲ��ִ� �Լ�
	// szDirPath = 0x002ef084 "c:\Users\Administrator\Desktop\FrameKor02\ToolKor02"

	PathRemoveFileSpec(szDirPath);	// ���� �̸��� ����� ������ ��θ� ����
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CFileDialog		Dlg(TRUE,	// false - save, true - load
		L"dat", // ������ Ȯ���ڸ�
		L"*.dat", // ���ʿ� ����� ���� �̸�
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		L"*.dat", // ���� �� �ؿ� ����ִ� ���� ����
		this);

	if(Dlg.DoModal() == IDCANCEL)
		return;


	TCHAR		szDirPath[MAX_PATH] = L"";

	GetCurrentDirectory(sizeof(szDirPath), szDirPath);	// vcproj ���� ��ġ�� ��� ��
	// szDirPath = 0x002ef084 "c:\Users\Administrator\Desktop\FrameKor02\ToolKor02"

	SetCurrentDirectory(szDirPath);	// ���� ���α׷��� ���� �۾� ���丮�� ������ ���丮�� �ٲ��ִ� �Լ�
	// szDirPath = 0x002ef084 "c:\Users\Administrator\Desktop\FrameKor02\ToolKor02"

	PathRemoveFileSpec(szDirPath);	// ���� �̸��� ����� ������ ��θ� ����
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
