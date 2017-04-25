
// ToolDoc.cpp : CToolDoc 클래스의 구현
//

#include "stdafx.h"
#include "Tool.h"

#include "ToolDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToolDoc

IMPLEMENT_DYNCREATE(CToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CToolDoc, CDocument)
END_MESSAGE_MAP()


// CToolDoc 생성/소멸

CToolDoc::CToolDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CToolDoc::~CToolDoc()
{
}

BOOL CToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CToolDoc serialization

void CToolDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}


// CToolDoc 진단

#ifdef _DEBUG
void CToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CToolDoc 명령
