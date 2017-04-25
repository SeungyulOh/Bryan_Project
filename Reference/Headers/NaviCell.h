#ifndef Engine_NaviCell_h__
#define Engine_NaviCell_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CNaviCell : public CBase
{
private:
	explicit CNaviCell(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3* pPointA,D3DXVECTOR3* pPointB,D3DXVECTOR3* pPointC,bool _ToolMode=false);
	virtual ~CNaviCell();

public:
	void Initialize();
	void Render();

	void SetNeighbor(CNaviCell* pNeighbor);
	D3DXVECTOR3* GetPoint();

	void SetMyIndex(DWORD _Index);
	DWORD GetIndex();

	vector<CNaviCell*>* GetNeighbor();

private:
	LPDIRECT3DDEVICE9 m_pDevice;
	LPD3DXLINE m_pLine;

	D3DXVECTOR3* m_pPoint[3];

	D3DXVECTOR3 m_vPoint[3];
	D3DXVECTOR3 m_vConPoint[4];
	DWORD m_myIndex;

	vector<CNaviCell*> m_NeighborVec;
	bool bToolMode;

public:
	static CNaviCell* Create(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3* pPointA,D3DXVECTOR3* pPointB,D3DXVECTOR3* pPointC,bool _ToolMode=false);
private:
	virtual void Free(void);
};

END
#endif
