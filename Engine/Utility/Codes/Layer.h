#ifndef Engine_Layer_h__
#define Engine_Layer_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CGameObject;
class ENGINE_DLL CLayer : public CBase
{
private:
	explicit CLayer(void);
	virtual ~CLayer(void);
public:
	inline void SetisAlive(_bool isAlive){m_isAliveOnEvent = isAlive;}
	inline _bool GetisAlive(){return m_isAliveOnEvent;}
public:
	HRESULT Ready_Layer(void);
	_int Update_Layer(const _float& fTimeDelta);
	_int LastUpdate_Layer(const _float& fTimeDelta);

	inline void Add_Object(CGameObject* pGameObject) {m_ObjList.push_back(pGameObject);}
	inline	list<CGameObject*>* GetList(void) {return &m_ObjList;}

private:
	list<CGameObject*> m_ObjList;
	typedef list<CGameObject*> OBJLIST;
	_bool m_isAliveOnEvent;


public:
	static CLayer* Create();
private:
	virtual void Free();
};

END

#endif // Layer_h__
