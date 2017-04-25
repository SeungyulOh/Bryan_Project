// EffectTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "EffectTool.h"
#include "Export_Engine.h"
#include "GameObject.h"
#include "MainFrm.h"
#include "EffectBack.h"


// CEffectTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CEffectTool, CDialog)

CEffectTool::CEffectTool(CWnd* pParent /*=NULL*/)
	: CDialog(CEffectTool::IDD, pParent)
	, m_fSlope(0)
	, m_fRadius(0)
	, m_fScale(0)
	, m_fOffSetX(0)
	, m_fOffSetY(0)
	, m_fOffSetZ(0)
	, m_fRotX(0)
	, m_fRotY(0)
	, m_fRotZ(0)
	, m_fStartRatio(0)
	, m_strEffectName(_T(""))
	, m_fLifeTime(0)
	, m_strAniName(_T(""))
	, m_isJustTrail(FALSE)
	, m_ScaleX(0)
	, m_ScaleY(0)
	, m_ScaleZ(0)
{

}

CEffectTool::~CEffectTool()
{
	list<_tchar*>::iterator iter = m_NameList.begin();
	list<_tchar*>::iterator iter_end = m_NameList.end();

	for(; iter!=iter_end;++iter)
	{
		Engine::Safe_Delete_Array(*iter);
	}
}

void CEffectTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_AnimationList);
	DDX_Text(pDX, IDC_EDIT1, m_fSlope);
	DDX_Text(pDX, IDC_EDIT3, m_fRadius);
	DDX_Text(pDX, IDC_EDIT2, m_fScale);
	DDX_Text(pDX, IDC_EDIT4, m_fOffSetX);
	DDX_Text(pDX, IDC_EDIT9, m_fOffSetY);
	DDX_Text(pDX, IDC_EDIT10, m_fOffSetZ);
	DDX_Text(pDX, IDC_EDIT11, m_fRotX);
	DDX_Text(pDX, IDC_EDIT12, m_fRotY);
	DDX_Text(pDX, IDC_EDIT13, m_fRotZ);
	DDX_Text(pDX, IDC_EDIT7, m_fStartRatio);
	DDX_Text(pDX, IDC_EDIT14, m_strEffectName);
	DDX_Text(pDX, IDC_EDIT8, m_fLifeTime);
	DDX_Control(pDX, IDC_LIST2, m_EffectList);
	DDX_Text(pDX, IDC_EDIT15, m_strAniName);
	DDX_Check(pDX, IDC_CHECK1, m_isJustTrail);
	DDX_Text(pDX, IDC_EDIT16, m_ScaleX);
	DDX_Text(pDX, IDC_EDIT17, m_ScaleY);
	DDX_Text(pDX, IDC_EDIT18, m_ScaleZ);
}


BEGIN_MESSAGE_MAP(CEffectTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CEffectTool::OnBnClickedPlay)
	ON_EN_CHANGE(IDC_EDIT1, &CEffectTool::OnEnChangeSlop)
	ON_EN_CHANGE(IDC_EDIT2, &CEffectTool::OnEnChangeScale)
	ON_EN_CHANGE(IDC_EDIT3, &CEffectTool::OnEnChangeRadius)
	ON_BN_CLICKED(IDC_BUTTON2, &CEffectTool::OnBnSaveEffect)
	ON_BN_CLICKED(IDC_BUTTON3, &CEffectTool::OnBnLoadEffect)
	ON_BN_CLICKED(IDC_BUTTON4, &CEffectTool::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_BUTTON5, &CEffectTool::OnBnClickedDelEffect)
	ON_BN_CLICKED(IDC_BUTTON6, &CEffectTool::OnBnClickedPlayAniEffect)
	ON_LBN_SELCHANGE(IDC_LIST1, &CEffectTool::OnLbnSelchangeAnimationList)
	ON_LBN_SELCHANGE(IDC_LIST2, &CEffectTool::OnLbnSelchangeEffectList)
	ON_BN_CLICKED(IDC_BUTTON7, &CEffectTool::OnBnClickedAddAnimation)
END_MESSAGE_MAP()


// CEffectTool �޽��� ó�����Դϴ�.

BOOL CEffectTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	//Engine::CDynamicMesh* pPlayerMesh = (Engine::CDynamicMesh*)Engine::GetList(L"Layer_Player")->front()->FindComponent(Engine::CGameObject::NONUPDATE,L"Component_Mesh_Hagie");
	UpdateData(TRUE);
	
	m_AnimationList.AddString(L"Atk1");
	m_AnimationList.AddString(L"Atk2");
	m_AnimationList.AddString(L"Atk3");
	m_AnimationList.AddString(L"Atk4");

	m_fSlope = -0.01f;
	m_fScale = 50.f;
	m_fRadius = 2.f;

	m_fOffSetX = 0.f;
	m_fOffSetY = 0.f;
	m_fOffSetZ = 0.f;

	m_fRotX = 0.f;
	m_fRotY = 0.f;
	m_fRotZ = 0.f;

	m_fStartRatio = 0.f;
	m_fLifeTime = 1.f;

	m_ScaleX = 1.f;
	m_ScaleY = 1.f;
	m_ScaleZ = 1.f;

	UpdateData(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CEffectTool::OnBnClickedPlay()
{
	UpdateData(TRUE);
	int iIndex = m_AnimationList.GetCurSel();
	if(iIndex < 0)
		return;

	EFFECTDATA tEffectData;

	_matrix matScale,matLocalScale, matTrans,matRotX,matRotX2,matRotY,matRotZ;
	D3DXMatrixScaling(&matLocalScale,10.f,10.f,10.f);
	D3DXMatrixRotationX(&matRotX,D3DX_PI/2.f);
	D3DXMatrixRotationX(&matRotX2,D3DXToRadian(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_fRotX));
	D3DXMatrixRotationY(&matRotY,D3DXToRadian(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_fRotY));
	D3DXMatrixRotationZ(&matRotZ,D3DXToRadian(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_fRotZ));

	D3DXMatrixTranslation(&matTrans,((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_fOffSetX,((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_fOffSetY,((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_fOffSetZ);
	_matrix matRot = matRotX * matRotX2 * matRotY * matRotZ;

	tEffectData.matLocal = matLocalScale * matRot * matTrans;
	tEffectData.fLifeTime = ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_fLifeTime;
	tEffectData.fStartRatio = ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_fStartRatio;
	tEffectData.fScale = ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_fScale;
	tEffectData.fEffectRadius = ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_fRadius;
	tEffectData.fSlope = ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_fSlope;
	tEffectData.fAtkRange = _vec3(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_ScaleX,((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_ScaleY,((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_ScaleZ);

	if(iIndex == 0)
		((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_pEffectBack->m_pPlayerTool->m_pDynamicMeshCom->Set_AnimationSet(CPlayer_Tool::ATK1,false);
	else if(iIndex == 1)
		((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_pEffectBack->m_pPlayerTool->m_pDynamicMeshCom->Set_AnimationSet(CPlayer_Tool::ATK2,false);
	else if(iIndex == 2)
		((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_pEffectBack->m_pPlayerTool->m_pDynamicMeshCom->Set_AnimationSet(CPlayer_Tool::ATK3,false);
	else if(iIndex == 3)
		((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_pEffectBack->m_pPlayerTool->m_pDynamicMeshCom->Set_AnimationSet(CPlayer_Tool::ATK4,false);

	CPhantom_Trail_Tool* pEffect = CPhantom_Trail_Tool::Create(((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_pGraphicDev,((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_pEffectBack->m_pPlayerTool->m_pDynamicMeshCom,tEffectData);
	((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_pEffectBack->m_pPlayerTool->m_EffectList.push_back(pEffect);
	UpdateData(FALSE);

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CEffectTool::OnBnClickedPlayAniEffect()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);
	int iIndex = m_AnimationList.GetCurSel();
	if(iIndex < 0)
		return;

	if(iIndex == 0)
		((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_pEffectBack->m_pPlayerTool->m_pDynamicMeshCom->Set_AnimationSet(CPlayer_Tool::ATK1,false);
	else if(iIndex == 1)
		((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_pEffectBack->m_pPlayerTool->m_pDynamicMeshCom->Set_AnimationSet(CPlayer_Tool::ATK2,false);
	else if(iIndex == 2)
		((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_pEffectBack->m_pPlayerTool->m_pDynamicMeshCom->Set_AnimationSet(CPlayer_Tool::ATK3,false);
	else if(iIndex == 3)
		((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_pEffectBack->m_pPlayerTool->m_pDynamicMeshCom->Set_AnimationSet(CPlayer_Tool::ATK4,false);

	CString strName;
	m_AnimationList.GetText(iIndex,strName);

	map<const _tchar*,vector<EFFECTDATA>>::iterator iter_found = find_if(m_mapEffect.begin(),m_mapEffect.end(),Engine::CTagFinder(strName));
	if(iter_found == m_mapEffect.end())
		return;

	vector<EFFECTDATA>::iterator iter =	iter_found->second.begin();
	vector<EFFECTDATA>::iterator iter_end = iter_found->second.end();

	for(; iter != iter_end ; ++iter)
	{
		CPhantom_Trail_Tool* pEffect = CPhantom_Trail_Tool::Create(((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_pGraphicDev,((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_pEffectBack->m_pPlayerTool->m_pDynamicMeshCom,(*iter));
		((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_pEffectBack->m_pPlayerTool->m_EffectList.push_back(pEffect);
	}
	UpdateData(FALSE);
}

void CEffectTool::OnEnChangeSlop()
{
	UpdateData(TRUE);
	UpdateData(FALSE);
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CEffectTool::OnEnChangeScale()
{
	UpdateData(TRUE);
	UpdateData(FALSE);
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CEffectTool::OnEnChangeRadius()
{
	UpdateData(TRUE);
	UpdateData(FALSE);
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CEffectTool::OnBnSaveEffect()
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
	SetCurrentDirectory(szDirPath);	// ���� ���α׷��� ���� �۾� ���丮�� ������ ���丮�� �ٲ��ִ� �Լ�
	PathRemoveFileSpec(szDirPath);	// ���� �̸��� ����� ������ ��θ� ����
	lstrcat(szDirPath, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szDirPath;	

	DWORD	dwByte = 0;

	HANDLE	hFile  = CreateFile(Dlg.GetPathName(), 
		GENERIC_WRITE, 
		0, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);


	map<const _tchar*,vector<EFFECTDATA>>::iterator iter = m_mapEffect.begin();
	map<const _tchar*,vector<EFFECTDATA>>::iterator iter_end = m_mapEffect.end();

	for( ; iter != iter_end ; ++iter)
	{
		vector<EFFECTDATA>::iterator iter_vec = iter->second.begin();
		vector<EFFECTDATA>::iterator iter_vec_end = iter->second.end();


		TCHAR szName[256] = L"";
		lstrcpy(szName,iter->first);
		_uint iVecSize = iter->second.size();
		WriteFile(hFile,szName,sizeof(TCHAR)*256,&dwByte,NULL);
		WriteFile(hFile,&iVecSize,sizeof(_uint),&dwByte,NULL);
		EFFECTDATA tEffectData;
		for(; iter_vec != iter_vec_end ; ++iter_vec)
		{
			tEffectData = *iter_vec;
			WriteFile(hFile,&tEffectData,sizeof(EFFECTDATA),&dwByte,NULL);
		}
	}

	CloseHandle(hFile);
}

void CEffectTool::OnBnLoadEffect()
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

	UpdateData(TRUE);
	
	m_AnimationList.ResetContent();
	
	while(true)
	{
		_tchar* pName = new _tchar[256];
		ReadFile(hFile,pName, sizeof(_tchar)*256, &dwByte, NULL);

		if(dwByte == 0)
			break;

		map<const _tchar*,vector<EFFECTDATA>>::iterator iter = find_if(m_mapEffect.begin(),m_mapEffect.end(),Engine::CTagFinder(pName));
		if(iter == m_mapEffect.end())
		{
			m_mapEffect.insert(make_pair(pName,vector<EFFECTDATA>()));
			m_NameList.push_back(pName);
		}

		_uint iVecSize = 0;
		ReadFile(hFile, &iVecSize, sizeof(_uint), &dwByte, NULL);

		EFFECTDATA tEffectData;
		for(_uint i = 0; i<iVecSize ; ++i)
		{
			ReadFile(hFile, &tEffectData, sizeof(EFFECTDATA), &dwByte, NULL);
			m_mapEffect[pName].push_back(tEffectData);
		}
	
		CString strName = pName;
		m_AnimationList.AddString(strName);
	
	}
	((CMainFrame*)AfxGetMainWnd())->m_pToolView->Invalidate(FALSE);

	
	UpdateData(FALSE);
	CloseHandle(hFile);
}

void CEffectTool::OnBnClickedAdd()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	int iIndex = m_AnimationList.GetCurSel();
	if(iIndex < 0)
		return;

	m_EffectList.AddString(m_strEffectName);

	CString strName;
	m_AnimationList.GetText(iIndex,strName);

	map<const _tchar*,vector<EFFECTDATA>>::iterator iter = find_if(m_mapEffect.begin(),m_mapEffect.end(),Engine::CTagFinder(strName));

	if(iter == m_mapEffect.end())
	{
		_tchar* pName = new _tchar[128];
		lstrcpy(pName,strName);

		m_mapEffect.insert(make_pair(pName,vector<EFFECTDATA>()));
		//m_mapEffect[pName] = vector<EFFECTDATA>();
				
		m_NameList.push_back(pName);
	}

	iter = find_if(m_mapEffect.begin(),m_mapEffect.end(),Engine::CTagFinder(strName));

	EFFECTDATA tEffectData;

	_matrix matScale,matLocalScale, matTrans,matRotX,matRotX2,matRotY,matRotZ;
	D3DXMatrixScaling(&matLocalScale,10.f,10.f,10.f);
	D3DXMatrixRotationX(&matRotX,D3DX_PI/2.f);
	D3DXMatrixRotationX(&matRotX2,D3DXToRadian(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_fRotX));
	D3DXMatrixRotationY(&matRotY,D3DXToRadian(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_fRotY));
	D3DXMatrixRotationZ(&matRotZ,D3DXToRadian(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_fRotZ));

	D3DXMatrixTranslation(&matTrans,((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_fOffSetX,((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_fOffSetY,((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_fOffSetZ);
	_matrix matRot = matRotX * matRotX2 * matRotY * matRotZ;

	tEffectData.matLocal = matLocalScale * matRot * matTrans;
	tEffectData.fLifeTime = ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_fLifeTime;
	tEffectData.fStartRatio = ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_fStartRatio;
	tEffectData.fScale = ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_fScale;
	tEffectData.fEffectRadius = ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_fRadius;
	tEffectData.fSlope = ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_fSlope;
	tEffectData.fAtkRange = _vec3(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_ScaleX,((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_ScaleY,((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_ScaleZ);

	iter->second.push_back(tEffectData);

	UpdateData(FALSE);
}

void CEffectTool::OnBnClickedDelEffect()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	int iIndex = m_AnimationList.GetCurSel();
	if(iIndex < 0)
		return;

	CString strName;
	m_AnimationList.GetText(iIndex,strName);

	map<const _tchar*,vector<EFFECTDATA>>::iterator iter = find_if(m_mapEffect.begin(),m_mapEffect.end(),Engine::CTagFinder(strName));

	if(iter == m_mapEffect.end())
		return;

	int iVecIdx = m_EffectList.GetCurSel();
	if(iVecIdx < 0)
		return;

	m_EffectList.DeleteString(iVecIdx);

	vector<EFFECTDATA>::iterator iter_vec = iter->second.begin();
	
	for(_int i=0; i<iVecIdx; ++i)
		++iter_vec;

	if(iter_vec == iter->second.end())
		return;
	
	iter->second.erase(iter_vec);

	UpdateData(FALSE);
}



void CEffectTool::OnLbnSelchangeAnimationList()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	m_EffectList.ResetContent();

	int iIndex = m_AnimationList.GetCurSel();

	CString strAniName;
	m_AnimationList.GetText(iIndex,strAniName);

	map<const _tchar*,vector<EFFECTDATA>>::iterator iter = find_if(m_mapEffect.begin(),m_mapEffect.end(),Engine::CTagFinder(strAniName));

	if(iter == m_mapEffect.end())
		return;

	vector<EFFECTDATA>::iterator iter_vec = iter->second.begin();
	vector<EFFECTDATA>::iterator iter_vec_end = iter->second.end();

	int iIndex2 = 0;
	for( ; iter_vec != iter_vec_end ; ++iter_vec)
	{
		++iIndex2;
		TCHAR szEffectName[128] = L"";
		wsprintf(szEffectName,L"Effect%d",iIndex2);
		m_EffectList.AddString(szEffectName);
	}
}

void CEffectTool::OnLbnSelchangeEffectList()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	int iIndex = m_AnimationList.GetCurSel();
	if(iIndex < 0)
		return;

	CString strAniName;
	m_AnimationList.GetText(iIndex,strAniName);
	map<const _tchar*,vector<EFFECTDATA>>::iterator iter = find_if(m_mapEffect.begin(),m_mapEffect.end(),Engine::CTagFinder(strAniName));


	int iVecIndex = m_EffectList.GetCurSel();
	if(iVecIndex < 0)
		return;
	
	m_fRadius = iter->second[iVecIndex].fEffectRadius;
	m_fSlope = iter->second[iVecIndex].fSlope;
	m_fScale = iter->second[iVecIndex].fScale;

	m_fOffSetX = ((_vec3*)&iter->second[iVecIndex].matLocal.m[3][0])->x;
	m_fOffSetY = ((_vec3*)&iter->second[iVecIndex].matLocal.m[3][0])->y;
	m_fOffSetZ = ((_vec3*)&iter->second[iVecIndex].matLocal.m[3][0])->z;

	m_fStartRatio = iter->second[iVecIndex].fStartRatio;
	m_fLifeTime = iter->second[iVecIndex].fLifeTime;

	m_ScaleX = iter->second[iVecIndex].fAtkRange.x;
	m_ScaleY = iter->second[iVecIndex].fAtkRange.y;
	m_ScaleZ = iter->second[iVecIndex].fAtkRange.z;


	UpdateData(FALSE);


	
}

void CEffectTool::OnBnClickedAddAnimation()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	m_AnimationList.AddString(m_strAniName);	

	UpdateData(FALSE);
}
