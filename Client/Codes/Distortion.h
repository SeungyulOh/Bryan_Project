#ifndef Distortion_h__
#define Distortion_h__

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

class CDistortion:public Engine::CGameObject
{
private:
	explicit CDistortion(LPDIRECT3DDEVICE9 pGraphicDev,_float m_fScaleSpeed,_float fDistortionPow,_int iImgIdx, _bool isBillBoard, _float fDeadTime,LPDIRECT3DBASETEXTURE9 pTexture);
	virtual ~CDistortion();
public:
	inline _float GetDeadTime(){return m_fDeadTime;}
	inline _float GetDistortionPow(){return m_fDistortionPow;}
public:
	virtual HRESULT Ready_GameObject(_vec3 vScale,_vec3 vRot,_vec3 vTrans);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);

private:
	Engine::CRenderer* m_pRendererCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CRect_Texture* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CShader* m_pShader;
	//Engine::CShader* m_pDistortionShader;

	_float m_fUV;
	_bool m_isOnce;
	_float m_fTimeAcc;
	_float m_fAlpha;
	_float m_fSpeed;
	_float m_fDir;
	_float m_fDeadTime;

	_matrix* m_pPlayerWorld;
	_uint m_iImgIdx;
	_matrix m_matWorld;

	//LPDIRECT3DTEXTURE9 m_pDistortionTex;
	//LPDIRECT3DSURFACE9 m_pSurface;
	_float m_fMat[4];
	_float m_fDistortionPow;
	_bool m_isBillBoard;

	//LPDIRECT3DBASETEXTURE9 m_pDistortionTexture;

public:
	static CDistortion* Create(LPDIRECT3DDEVICE9 pGraphicDev,_vec3 vScale = _vec3(5.f,5.f,5.f),_vec3 vRot = _vec3(0.f,0.f,0.f),_vec3 vTrans = _vec3(0.f,0.f,0.f),_float m_fScaleSpeed = 0.f,_float fDistortionPow = 0.05f, _int iImgIdx = 0, _bool isBillBoard = true, _float fDeadTime = 0.4f, LPDIRECT3DBASETEXTURE9 pTexture = NULL);

private:
	virtual void Free(void);

};

#endif // Distortion_h__
