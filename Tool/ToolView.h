
// ToolView.h : CToolView Ŭ������ �������̽�
//


#pragma once
#include "Engine_Defines.h"

class CToolDoc;
class CBossMap;
class CDefault_Camera;
class CCubeObj;
class CMeshObj;
class CEffectBack;
class CToolView : public CView
{
protected: // serialization������ ��������ϴ�.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

// Ư���Դϴ�.
public:
	CToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	CBossMap* m_pBossMap;
	CDefault_Camera* m_pCamera;
	CEffectBack* m_pEffectBack;


	vector<CMeshObj*> m_vecMeshObj;


	_bool bCamFocus;

	list<CCubeObj*>	m_CubeList;

	vector<D3DXVECTOR3*> m_SelectedPointVec;
	int m_iCellCnt;

public:
	void Ready_Component(void);
	void Ready_DefaultSetting(void);

	void AddCube(CPoint point);
	void PickCube(CPoint point);
	void AddObject(CPoint point);

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	//afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // ToolView.cpp�� ����� ����
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

