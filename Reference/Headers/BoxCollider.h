#ifndef Engine_BoxCollider_h__
#define Engine_BoxCollider_h__

#include "Component.h"

BEGIN(Engine)

class CStaticMesh;
class CDynamicMesh;
class ENGINE_DLL CBoxCollider : public CComponent
{
public:
	enum BOX_TYPE { BOX_AABB, BOX_OBB, BOX_END };
	enum COL_TYPE { COL_X, COL_O, COL_END };
	enum CALC_TYPE { CAL_MIN, CAL_MAX, CAL_END };
private:
	explicit CBoxCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBoxCollider(const CBoxCollider& rhs);
	virtual ~CBoxCollider(void);
public:
	void SetMinMaxDirect(_vec3 vMin,_vec3 vMax);
public:
	void Ready_Collider(void);
	virtual void Update_Component(const _float& fTimeDelta);
	void Compute_MinMax(Engine::CStaticMesh* pStaticMesh);
	void Compute_MinMax(Engine::CDynamicMesh* pDynamicMesh,_matrix matInit, _ulong lMeshContainerIdx = 0, _vec3 vScale = _vec3(1.f,1.f,1.f));
	void Render(BOX_TYPE eType, _matrix matPreWorld, _bool isCol, _bool isRender = true);
	_bool CollisionCheck_AABB(CBoxCollider* pTargetCollider);
	_bool CollisionCheck_OBB(CBoxCollider* pTargetCollider);

	_vec3 GetMinMaxPos(CBoxCollider* pMyCollider, CBoxCollider* pTargetCollider, CALC_TYPE eType);
	_matrix GetNewMatExcScale(_matrix matIn);
	
private:
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;
	LPDIRECT3DTEXTURE9 m_pTexture[COL_END];

	_vec3 m_vMin, m_vMax;
	_matrix m_matWorld;
	_matrix m_LocalMat;

	_vec3 m_MyLength[2];
	_vec3 m_TargetLength[2];

public:
	static CBoxCollider* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	CComponent* Clone(void);

private:
	virtual void Free(void);
};

END


#endif // Engine_BoxCollider_h__
