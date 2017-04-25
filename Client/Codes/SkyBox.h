#ifndef SkyBox_h__
#define SkyBox_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CTransform;
	class CCube_Tex;
	class CTexture;
	class CShader;

}

class CSkyBox : public Engine::CGameObject
{
private:
	explicit CSkyBox(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSkyBox(void);

public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);


private:
	Engine::CRenderer* m_pRendererCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CCube_Tex* m_pCubeBufCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CShader* m_pShaderCom;
public:
	static CSkyBox* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);
};

#endif // SkyBox_h__
