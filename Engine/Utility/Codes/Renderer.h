#ifndef Engine_Renderer_h__
#define Engine_Renderer_h__

#include "Component.h"

BEGIN(Engine)

class CShader;
class CGameObject;
class CRect_Texture;
class ENGINE_DLL CRenderer : public CComponent
{
public:
	enum TEXID{FOR_COPY,FOR_SHADOW,FOR_DISTORTION,ID_END};
private:
	explicit CRenderer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRenderer(void);

public:
	enum RENDERTYPE{RT_SHADOW,RT_PRIORITY,RT_NONALPHA,RT_ALPHA,RT_UI,RT_END};
public:
	inline _matrix* GetLightView(){return &m_matLightView;}
	inline _matrix* GetLightProj(){return &m_matLightProj;}
	inline void SetPlayerWorldMat(_matrix* pMat){m_pPlayerWorldMat = pMat;}
	inline void SetisBlur(_bool isBlur){m_isBlurOn = isBlur;}
	inline void SetisDistortion(_bool isDist){m_isDistortionOn = isDist;}
	inline void SetisGaussianBlur(_bool isBlur){m_isGaussianBlurOn = isBlur;}
	inline void SetisFadeOut(_bool isFade){m_isFadeOut = isFade;}
	inline void SetisFadeIn(_bool isFade){m_isFadeIn = isFade;}
	inline void SetisRedScreen(_bool isRed){m_isRedScreen = isRed;}
	inline void SetisWhiteScreen(_bool isWhite){m_isWhiteScreen = isWhite;}
	inline void SetBlurEffect(LPD3DXEFFECT pEffect){m_pBlurEffect = pEffect;}
public:
	HRESULT Ready_Renderer(void);
	void Add_RenderObject(CGameObject* pGameObject, RENDERTYPE eType);
	void Render_Object();
	void SetBlurOn(LPD3DXEFFECT pEffect, _uint iPassNum, _int iMaxBlurCnt = 6);
	void SetDistortionOn(LPD3DXEFFECT pEffect, _uint iPassNum);
	void SetGaussianBlurOn(LPD3DXEFFECT pEffect, _float fCoeff = -0.05f, _uint iGaussianBlurCnt = 13);
	void SetFadeOut(_float fFade);
	void SetFadeIn(_float fFade);
	void SetRedScreen(_float fFade);
	void SetWhiteScreen(_float fWhite);
public:
	void Render_Priority();
	void Render_Shadow();
	void Render_Deferred();
	void Render_Shade();
	void Render_Blend();


	void Render_Alpha();
	void Render_UI();

	virtual CComponent* Clone();


	//Distortion
	void Render_WithDistortion(LPD3DXEFFECT pEffect, _uint iPassNum, LPDIRECT3DBASETEXTURE9 pDistortionImg, _float* fRotMat);
	void Render_WithDistortionNormalRectBuff(LPD3DXEFFECT pEffect, _uint iPassNum, LPDIRECT3DBASETEXTURE9 pDistortionImg, _float* fRotMat, CRect_Texture* pBuffer);
	void Render_WithRadialBlur(LPD3DXEFFECT pEffect, _uint iPassNum, _int iMaxBlurCnt = 6);
	void Render_WithGaussianBlur(LPD3DXEFFECT pEffect, _float fCoeff, _uint iMaxCnt);
	void Render_FadeOut(_float fFadeCoeff);
	void Render_RedScreen(_float fFadeCoeff);
	void Render_WhiteScreen(_float fFadeCoeff);

private:
	list<CGameObject*> m_RenderList[RT_END];
	LPDIRECT3DDEVICE9 m_pGraphicDev;

	//For RenderTarget

	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;

	Engine::CShader* m_pShaderForShadow;
	Engine::CShader* m_pShaderForShade;
	Engine::CShader* m_pShaderForBlend;

	_matrix m_matLightView;
	_matrix m_matLightProj;
	_float m_fLightDist;
	_matrix* m_pPlayerWorldMat;

	//For Blur
	_bool m_isBlurOn;
	LPD3DXEFFECT m_pBlurEffect;
	_uint m_iBlurPassIdx;
	_uint m_iMaxBlurCnt;

	_bool m_isGaussianBlurOn;
	_float m_fCoeff;
	_uint m_iMaxGaussianBlurCnt;

	//For Distortion
	_bool m_isDistortionOn;
	LPD3DXEFFECT m_pDistortionEffect;
	_uint m_iDistortionPassIdx;

	//For Fade In
	_bool m_isFadeOut;
	_bool m_isFadeIn;
	_float m_fFadeCoeff;

	//For RedScreen
	_bool m_isRedScreen;
	_bool m_isWhiteScreen;

public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);



};


END

#endif // Renderer_h__
