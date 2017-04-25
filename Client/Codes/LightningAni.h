#ifndef LightningAni_h__
#define LightningAni_h__

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

class CLightningAni:public Engine::CGameObject
{
public:
	enum eType{LEFTHAND,RIGHTHAND,ATTACK};
private:
	explicit CLightningAni(LPDIRECT3DDEVICE9 pGraphicDev, eType MyType,_float fDeadTime);
	virtual ~CLightningAni();

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
	eType m_eMyType;

	_uint m_iImgIdx;
	_float m_fDeadTime;

	_matrix* m_pPlayerFrame;
	_matrix* m_pPlayerWorldMat;
	_matrix m_matMyLocal;

	_float m_fRandX;
	_float m_fRandZ;
	_float m_fChangePosTime;
	_float m_fChangeImgTime;

public:
	static CLightningAni* Create(LPDIRECT3DDEVICE9 pGraphicDev,eType MyType,_float fDeadTime=2.f);
private:
	virtual void Free(void);

};

#endif // LightningAni_h__
