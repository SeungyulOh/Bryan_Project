#ifndef ShapeFire_h__
#define ShapeFire_h__

#include "Defines.h"
#include "GameObject.h"
#include "Player.h"

namespace Engine
{
	class CRenderer;
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CShader;
}

class CShapeFire:public Engine::CGameObject
{
private:
	explicit CShapeFire(LPDIRECT3DDEVICE9 pGraphicDev, CPlayer::AttackState* pPlayerAtkState);
	virtual ~CShapeFire();

public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);

private:
	Engine::CRenderer* m_pRendererCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CRect_Texture* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CShader* m_pShader;

	_float m_fUV;
	_bool m_isOnce;
	_float m_fTimeAcc;
	_float m_fAlpha;
	_float m_fSpeed;
	_float m_fDir;

	_uint m_iImgIdx;
	CPlayer::AttackState* m_pPlayerAtkState;

public:
	static CShapeFire* Create(LPDIRECT3DDEVICE9 pGraphicDev, CPlayer::AttackState* pPlayerAtkState);
private:
	virtual void Free(void);

};

#endif // ShapeFire_h__
