#ifndef Engine_GameObject_h__
#define Engine_GameObject_h__

#include "Engine_Defines.h"
#include "Base.h"
#include "Component.h"

BEGIN(Engine)


class ENGINE_DLL CGameObject : public CBase
{
public:
	enum COMTYPE { UPDATE, NONUPDATE, TYPE_END };
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGameObject(void);
public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta){return 0;}
	virtual void Render_GameObject(void);
	virtual void Render_ShadowDepth(void);

public:
	inline void SetisCol(_bool isCol){m_isCol = isCol;}

public:
	void ComputeViewZ(_matrix* pWorldMat);
public:
	inline _bool GetDestroy(){return m_isDestroy;}
	inline void SetDestroy(_bool bDestroy) {m_isDestroy = bDestroy;}
	inline _float GetViewZ(void){return m_fViewZ;}
public:
	void Push_CloneComponent(COMTYPE eType, const _tchar* pTag, CComponent* pComponent);
	CComponent* FindComponent(COMTYPE eType, const _tchar* pTag);

protected:
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	map<const _tchar*, CComponent*> m_MapComponent[TYPE_END];
	typedef map<const _tchar*, CComponent*> MAPCOMPONENT;
	_bool m_isCol;
	_bool m_isDestroy;
	_float m_fViewZ;

protected:
	virtual void Free(void);
};



END


#endif // GameObject_h__
