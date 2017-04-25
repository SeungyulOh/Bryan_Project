#ifndef Engine_HierarchyLoader_h__
#define Engine_HierarchyLoader_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CHierarchyLoader : public ID3DXAllocateHierarchy
{
private:
	explicit CHierarchyLoader(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* szPath);
	virtual ~CHierarchyLoader(void);
public:
	inline void AddRefCnt(void){++m_dwRefCnt;}

public:
	void Ready_HierarchyLoader(void);
	void Allocate_Name(char** ppDestName, const char* pSrcName);
	_ulong Release(void);

	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);
	STDMETHOD(CreateMeshContainer)(THIS_ 
		LPCSTR Name, 
		CONST D3DXMESHDATA *pMeshData, 
		CONST D3DXMATERIAL *pMaterials, 
		CONST D3DXEFFECTINSTANCE *pEffectInstances, 
		DWORD NumMaterials, 
		CONST DWORD *pAdjacency, 
		LPD3DXSKININFO pSkinInfo, 
		LPD3DXMESHCONTAINER *ppNewMeshContainer);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree);

private:
	_ulong m_dwRefCnt;
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	const _tchar* m_szPath;

public:
	static CHierarchyLoader* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* szPath);
};

END

#endif // Engine_HierarchyLoader_h__
