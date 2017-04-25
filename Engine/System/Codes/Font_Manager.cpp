#include "Font_Manager.h"

using namespace Engine;
IMPLEMENT_SINGLETON(CFont_Manager)

CFont_Manager::CFont_Manager(void)
{

}
CFont_Manager::~CFont_Manager(void)
{

}

HRESULT CFont_Manager::Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFontTag, const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	CFont*			pFont = NULL;

	pFont = Find_Font(pFontTag);
	if(NULL != pFont)
		return E_FAIL;

	pFont = CFont::Create(pGraphicDev, pFontType, iWidth, iHeight, iWeight);

	if(NULL == pFont)
		return E_FAIL;

	m_mapFont.insert(MAPFONT::value_type(pFontTag, pFont));

	return S_OK;	
}

void CFont_Manager::Render_Font(const _tchar* pFontTag, const _tchar* pString, const _vec2* pPosition, D3DXCOLOR Color)
{
	CFont*	pFont = Find_Font(pFontTag);

	if(NULL == pFont)
		return;

	pFont->Render_Font(pString, pPosition, Color);
}

CFont* CFont_Manager::Find_Font(const _tchar* pFontTag)
{
	MAPFONT::iterator	iter = find_if(m_mapFont.begin(), m_mapFont.end(), CTagFinder(pFontTag));
	if(iter == m_mapFont.end())
		return NULL;

	return iter->second;
}

void CFont_Manager::Free(void)
{
	for_each(m_mapFont.begin(), m_mapFont.end(), CRelease_Pair());
	m_mapFont.clear();
}