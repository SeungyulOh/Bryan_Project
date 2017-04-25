#ifndef Atk2Throw_h__
#define Atk2Throw_h__

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

class CAtk2Throw:public Engine::CGameObject
{
private:
	explicit CAtk2Throw(LPDIRECT3DDEVICE9 pGraphicDev, _int iLeftLife,_vec3 MyFirstPos,_vec3 vFirstMove,_float fOffSet,_float fLifeTime,_float fOffSetAngleX,_int iImgIdx);
	virtual ~CAtk2Throw();

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
	_matrix m_PlayerMat;
	_matrix m_LocalMat;
	_uint m_iShowTime;
	_float m_fOffSet;
	_int m_iLeftLife;
	_float m_fLifeTime;
	_float m_fOffSetAngleX;
	_vec3 m_MyFirstDir;
	_vec3 m_MyFirstPos;


public:
	static CAtk2Throw* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iLeftLife,_vec3 MyFirstPos,_vec3 vFirstMove,_float fOffSet = 1.f,_float fLifeTime = 0.01f,_float fOffSetAngleX = 0.f,_int iImgIdx = 0);
private:
	virtual void Free(void);

};

#endif // Atk2Throw_h__
