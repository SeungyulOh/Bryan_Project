#ifndef Engine_DynamicMesh_h__
#define Engine_DynamicMesh_h__

#include "Engine_Defines.h"
#include "Mesh.h"

BEGIN(Engine)

class CAnimationCtrl;
class CHierarchyLoader;
class ENGINE_DLL CDynamicMesh : public CMesh
{
private:
	explicit CDynamicMesh(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName,_matrix matFirst);
	explicit CDynamicMesh(const CDynamicMesh& rhs);
	virtual ~CDynamicMesh(void);
public:
	LPD3DXFRAME GetRootFrame() {return m_pRootFrame;}
	inline _matrix* GetInitMatrix() {return &m_matParent;}
	inline CAnimationCtrl* GetAnimationCtrl() {return m_pAnimationCtrl;}

public:
	void Ready_DynamicMesh(LPDIRECT3DDEVICE9 pGraphicDev,const _tchar* pFilePath , const _tchar* pFileName,_matrix matFirst);
	void Update_MatrixFromRootFrame(D3DXFRAME_DERIVED* pRootFrame, _matrix matParent);
	void Fill_MatrixInfoOnMeshContainer(D3DXFRAME_DERIVED* pRootFrame);
	void Render_Mesh(void);
	void Render_MeshWithShader(LPD3DXEFFECT pEffect, _ulong dwPassIdx = 0);
	void Render_MeshOnlyForKadan(LPD3DXEFFECT pEffect, _ulong dwPassIdx = 0);
	void Compute_MinMax(_vec3* pMin, _vec3* pMax, _ulong lMeshContainerIdx);
	void SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	_matrix CalculateCombineMat(D3DXFRAME_DERIVED* pMatIn);


	//Animation
	void Set_AnimationSet(_ulong dwIndex, _bool bflexible = true, _float fSpeed = 1.f);
	void Play_Animation(const _float& fTimeDelta,_bool isUpdateCombine = true);
	bool Get_Occupied(void);
	_double Get_CurrentRatioFromAnimation();
	void SetCurrentTrackPosition(double _dPosition);

private:
	CHierarchyLoader* m_pLoader;
	CAnimationCtrl* m_pAnimationCtrl;
	LPD3DXFRAME m_pRootFrame;
	_matrix m_matParent;

	list<D3DXMESHCONTAINER_DERIVED*> m_MeshContainerList;

public:
	static CDynamicMesh* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName, _matrix matFirst = D3DXMATRIX(1.f,0.f,0.f,0.f,0.f,1.f,0.f,0.f,0.f,0.f,1.f,0.f,0.f,0.f,0.f,1.f));
	CComponent* Clone(void);
private:
	virtual void Free(void);
};

END

#endif // DynamicMesh_h__
