#ifndef Engine_Transform_h__
#define Engine_Transform_h__

#include "Engine_Defines.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
public:
	enum MATINFO {SCALE,ANGLE,POSITION,MAT_END};
private:
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform(void);
public:
	void SetLocalMat(_matrix matLocal) {m_matLocal = matLocal;}
	void SetWorldmat(_matrix matWorld) {m_matWorld = matWorld;}
public:
	virtual void Update_Component(const _float& fTimeDelta);
	HRESULT Ready_Transform(void);
	void Adjust_ParentMat(const _matrix* pParent);
	bool Align_TargetDirection(_vec3 vTarget, _float fTimeDelta);
	bool Align_TargetDirection(CTransform* pTransform,_ulong dwCurIndex, _vec3 vTarget, _float fTimeDelta);
public:
	void SetUp_OnGraphicDev(void);
	void SetInfomation(MATINFO eInfo,const _vec3 vInfo);
	_vec3* GetInfomation(MATINFO eInfo);
	void SetAngleWithLook(_vec3 vNormalizedLook);
	inline void SetBillBoardMat(_matrix matBillboard){m_matBilboard = matBillboard;}
	inline void SetOffSetRotMat(_matrix matRotMat){m_matOffSetRot = matRotMat;}
	void SetBillBoardAfterUpdate();

private:
	void MakeWorldMat(void);
	
public:
	inline _matrix* GetWorldMatrix(void) {return &m_matWorld;}

private:
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	_matrix m_matWorld;
	_vec3 m_vInfomation[MAT_END];
	_matrix m_matLocal;
	_matrix m_matBilboard;
	_matrix m_matOffSetRot;

public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(void);
private:
	virtual void Free(void);
};

END
#endif // Engine_Transform_h__
