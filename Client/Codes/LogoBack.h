#ifndef LogoBack_h__
#define LogoBack_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CShader;
}

class CLogoBack:public Engine::CGameObject
{
private:
	explicit CLogoBack(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLogoBack();

public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);
	void Render_Blur(void);

private:
	Engine::CRenderer* m_pRendererCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CRect_Texture* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTexture* m_pTextureDistortion;
	Engine::CShader* m_pShader;
	Engine::CShader* m_pShaderForRenderTarget;

	LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DSURFACE9 m_pSurface;

	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;

	_float m_fUV;
	_float m_fTimeAcc;
	_float m_fRotMat[4];

public:
	static CLogoBack* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);

};

#endif // LogoBack_h__
