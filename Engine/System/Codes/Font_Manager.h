#ifndef Font_Manager_h__
#define Font_Manager_h__

#include "Engine_Defines.h"
#include "Base.h"
#include "Font.h"

BEGIN(Engine)

class ENGINE_DLL CFont_Manager : public CBase
{
	DECLARE_SINGLETON(CFont_Manager)
private:
	explicit CFont_Manager(void);
	virtual ~CFont_Manager(void);
public:
	HRESULT Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFontTag
		, const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight);
	void Render_Font(const _tchar* pFontTag, const _tchar* pString, const _vec2* pPosition, D3DXCOLOR Color);
private:
	map<const _tchar*, CFont*>			m_mapFont;
	typedef map<const _tchar*, CFont*>	MAPFONT;
private:
	CFont* Find_Font(const _tchar* pFontTag);
public:
	virtual void Free(void);
};

END

#endif // Font_Manager_h__
