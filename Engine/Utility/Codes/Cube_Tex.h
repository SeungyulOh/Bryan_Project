#ifndef Engine_Cube_Tex_h__
#define Engine_Cube_Tex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCube_Tex
	:public CVIBuffer
{
private:
	explicit CCube_Tex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCube_Tex(const CCube_Tex& rhs);
	virtual ~CCube_Tex(void);

public:
	virtual HRESULT Ready_Buffer(void);
	virtual void Render_Buffer(void);

	void GetVtxInfo(VTXCUBETEX* pVertex);
	void GetIdxInfo(INDEX32* pIndex);

public:
	virtual CComponent* Clone(void);
	static CCube_Tex* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void);
};

END

#endif // Engine_Cube_Tex_h__
