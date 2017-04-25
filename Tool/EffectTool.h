#pragma once
#include "afxwin.h"

#include "Phantom_Trail_Tool.h"
// CEffectTool 대화 상자입니다.

class CEffectTool : public CDialog
{
	DECLARE_DYNAMIC(CEffectTool)

public:
	CEffectTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEffectTool();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_EFFECTTOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListBox m_AnimationList;
	afx_msg void OnBnClickedPlay();
	float m_fSlope;
	float m_fRadius;
	float m_fScale;
	afx_msg void OnEnChangeSlop();
	afx_msg void OnEnChangeScale();
	afx_msg void OnEnChangeRadius();
	float m_fOffSetX;
	float m_fOffSetY;
	float m_fOffSetZ;
	afx_msg void OnBnSaveEffect();
	afx_msg void OnBnLoadEffect();
	float m_fRotX;
	float m_fRotY;
	float m_fRotZ;
	float m_fStartRatio;
	CString m_strEffectName;
	float m_fLifeTime;
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedDelEffect();
	CListBox m_EffectList;

	map<const _tchar*,vector<EFFECTDATA>> m_mapEffect;

	afx_msg void OnBnClickedPlayAniEffect();

	list<_tchar*> m_NameList;
	afx_msg void OnLbnSelchangeAnimationList();
	afx_msg void OnLbnSelchangeEffectList();
	afx_msg void OnBnClickedAddAnimation();
	CString m_strAniName;
	BOOL m_isJustTrail;
	float m_ScaleX;
	float m_ScaleY;
	float m_ScaleZ;
};
