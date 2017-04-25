#ifndef Engine_StaticMesh_h__
#define Engine_StaticMesh_h__

#include "Mesh.h"

BEGIN(Engine)

class CShader;
class ENGINE_DLL CStaticMesh
	:public CMesh
{
private:
	explicit CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStaticMesh(const CStaticMesh& rhs);
	virtual ~CStaticMesh(void);

public:
	inline _ulong GetSubSetCnt(void){return m_dwSubSetCnt;}
	inline LPDIRECT3DTEXTURE9 GetTexture(_ulong dwSubSetIdx){return m_ppTexture[dwSubSetIdx];}
	inline D3DMATERIAL9* GetMaterial(_ulong dwSubSetIdx){return &m_pMtrl[dwSubSetIdx];}
	inline LPD3DXMESH GetMesh() {return m_pMesh;}
public:
	HRESULT Ready_Component(const _tchar* szPath, const _tchar* szFileName);
	void Render_SubSets(_ulong lStage = 0);
	void Render_SubSetsWithShader(LPD3DXEFFECT pEffect, _ulong dwSubSetCnt,_ulong dwPassIdx = 0);
	void Render_SubSetsWithHWInstancing(LPD3DXEFFECT pEffect, _ulong dwSubSetCnt, _matrix matWorld, _ulong dwPassIdx = 0);
	void Compute_MinMax(_vec3* pMin, _vec3* pMax);
	void SetUp_ConstantTable(LPD3DXEFFECT pEffect);

private:
	LPD3DXBUFFER m_pAdjacency;
	LPD3DXBUFFER m_pSubset;
	LPD3DXMESH m_pMesh;
	LPD3DXMESH m_pOriMesh;
	_ulong m_dwSubSetCnt;

	D3DMATERIAL9*		m_pMtrl;
	LPDIRECT3DTEXTURE9*  m_ppTexture;
	
public:
	static CStaticMesh* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* szPath, const _tchar* szFileName);
	virtual CComponent* Clone(void);
private:
	virtual void Free(void);
};

END

#endif // Engine_StaticMesh_h__
