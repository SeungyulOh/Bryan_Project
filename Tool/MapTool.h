#pragma once
#include "afxwin.h"

// CMapTool 대화 상자입니다.
#include "Engine_Defines.h"

class CMeshObj;
class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)
private:
	typedef struct tagSaveFile
	{
		TCHAR szName[128];
		_vec3 vScale;
		_vec3 vAngle;
		_vec3 vPos;
	}SaveMeshObj;

public:
	CMapTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMapTool();

public:
	CMeshObj* m_pSelectedMesh;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MAPTOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	float PosX;
	float PosY;
	float PosZ;
	float fTargetY;
	CButton m_CubeControl[3];
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedPickCube();
	afx_msg void OnBnClickedMakeCell();
	float fCubeX;
	float fCubeY;
	float fCubeZ;
	int m_iCellCnt;
	CButton m_AxisControl[3];
	afx_msg void OnBnClickedX();
	afx_msg void OnBnClickedY();
	afx_msg void OnBnClickedZ();
	afx_msg void OnSave();
	afx_msg void OnLoad();
	CListBox m_ListBoxCtrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeList1();
	CButton m_ModiControl[3];
	CButton m_DirControl[3];
	afx_msg void OnBnClickedRadio7();
	afx_msg void OnBnClickedRadio8();
	afx_msg void OnBnClickedRadio9();
	afx_msg void OnBnClickedRadio10();
	afx_msg void OnBnClickedRadio11();
	afx_msg void OnBnClickedRadio12();
	CListBox m_ObjListFromFile;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	BOOL m_bAddObj;


	list<_tchar*> m_NameList;
	afx_msg void OnBnSaveForObject();
	afx_msg void OnBnLoadForObject();
	afx_msg void OnBnDeleteMeshObj();
};
