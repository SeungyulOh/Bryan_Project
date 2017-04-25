#ifndef Engine_Texture_h__
#define Engine_Texture_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture : public CComponent
{
public:
	enum TEXTURETYPE {TYPE_NORMAL, TYPE_CUBE, TYPE_END};
protected:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture(void);
public:
	void SetUp_OnGraphicDev(const _uint& iIndex = 0);	
	inline LPDIRECT3DBASETEXTURE9 GetTexture(const _uint& iIndex = 0){return m_vecTexture[iIndex];}
public:
	virtual HRESULT Ready_Texture(const _tchar* pPath, TEXTURETYPE eType, const _uint& iCnt);
private:
	LPDIRECT3DDEVICE9							m_pGraphicDev;
private:
	vector<IDirect3DBaseTexture9*>				m_vecTexture;
	typedef vector<IDirect3DBaseTexture9*>		VECTEXTURE;

public:
	virtual CComponent* Clone(void);
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, TEXTURETYPE eType, const _uint& iCnt = 1);
public:
	virtual void Free(void);
	
};


END

#endif // Engine_Texture_h__
