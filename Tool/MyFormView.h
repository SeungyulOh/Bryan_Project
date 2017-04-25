#pragma once
#include "afxcmn.h"



// CMyFormView �� ���Դϴ�.
#include "MapTool.h"
#include "EffectTool.h"

class CMyFormView : public CFormView
{
	DECLARE_DYNCREATE(CMyFormView)

protected:
	CMyFormView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMyFormView();

public:
	enum { IDD = IDD_MYFORMVIEW };

public:
	CMapTool m_MapTool;
	CEffectTool m_EffectTool;

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_TabCtrl;
	afx_msg void OnTabControl(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnInitialUpdate();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};


