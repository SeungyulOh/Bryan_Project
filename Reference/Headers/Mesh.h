#ifndef Engine_Mesh_h__
#define Engine_Mesh_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CMesh
	:public CComponent
{
protected:
	explicit CMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMesh(const CMesh& rhs);
	virtual ~CMesh(void);

protected:
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	_bool m_isClone;

protected:
	virtual void Free(void);

};


END

#endif // Engine_Mesh_h__
