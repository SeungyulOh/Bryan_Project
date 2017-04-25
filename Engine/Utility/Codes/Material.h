#ifndef Engine_Material_h__
#define Engine_Material_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CMaterial 
	:public CComponent
{
public:
	enum MATERIAL{DIFFUSE, AMBIENT, SPECULAR, EMISSIVE, POWER};
private:
	explicit CMaterial(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMaterial(const CMaterial& rhs);
	virtual ~CMaterial();

public:
	HRESULT Ready_Material(D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, D3DXCOLOR Emissive, _float Power);
	void Change_Material(MATERIAL eType, void* pInfo);

	inline void SetUp_OnGraphicDev(void)
	{ m_pGraphicDev->SetMaterial(&m_tMtrl);}

private:
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	D3DMATERIAL9 m_tMtrl;

public:
	static CMaterial* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, D3DXCOLOR Emissive, _float Power);
	virtual CComponent* Clone(void);
private:
	virtual void Free(void);
};


END

#endif // Engine_Material_h__
