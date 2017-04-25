#ifndef Engine_Shader_h__
#define Engine_Shader_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader:public CComponent
{
public:
	enum SHADERPATH{DEFAULT, WATER, ALPHATEST, CULLNONE,ALPHABLEND};
private:
	explicit CShader(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CShader(const CShader& rhs);
	virtual ~CShader(void);
public:
	inline LPD3DXEFFECT GetEffect(){return m_pEffect;}
public:
	HRESULT Ready_Shader(const _tchar* szShaderFilePath);
	void SetMatrixOnShader();
	void SetMatrixOnShader(_matrix matWorld);

private:
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	LPD3DXEFFECT m_pEffect;
	LPD3DXBUFFER m_pErrBuffer;

public:
	static CShader* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* szShaderFilePath);
	CComponent* Clone(void);
private:
	virtual void Free(void);
};

END

#endif // Engine_Shader_h__
