#pragma once
#include "afxcmn.h"



// CMyFormView 폼 뷰입니다.
#include "MapTool.h"
#include "EffectTool.h"

class CMyFormView : public CFormView
{
	DECLARE_DYNCREATE(CMyFormView)

protected:
	CMyFormView();           // 동적 만들기에 사용되는 protected 생성자입니다.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_TabCtrl;
	afx_msg void OnTabControl(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnInitialUpdate();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};


