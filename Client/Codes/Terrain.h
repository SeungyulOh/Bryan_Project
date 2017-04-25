#ifndef Terrain_h__
#define Terrain_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CTransform;
	class CTerrain_Texture;
	class CTexture;
	class CMaterial;

}

class CTerrain
	:public Engine::CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTerrain();

public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);

private:
	Engine::CRenderer* m_pRendererCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTerrain_Texture* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CMaterial* m_pMaterialCom;


public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void);
};

#endif // Terrain_h__
