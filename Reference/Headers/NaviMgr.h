#ifndef Engine_NaviMgr_h__
#define Engine_NaviMgr_h__

#include "NaviCell.h"
#include "Base.h"

BEGIN(Engine)

class CTransform;
class ENGINE_DLL CNaviMgr
	:public CBase
{
	DECLARE_SINGLETON(CNaviMgr)

private:
	explicit CNaviMgr();
	virtual ~CNaviMgr();

public:
	enum MOVETYPE {MOVE_FORWARD, MOVE_BACKWARD};

public:
	void AddNaviCell(D3DXVECTOR3* pPointA,D3DXVECTOR3* pPointB, D3DXVECTOR3* pPointC,bool _ToolMode=false);
	void Initialize(LPDIRECT3DDEVICE9 pDevice, DWORD dwTotalCell);
	void Render();

	void SetNeighbor();
	DWORD ComparePoint(D3DXVECTOR3* pFirstArr, D3DXVECTOR3* pSecondArr);

	void MoveOnNavi(CTransform* pTransform, DWORD& CurIndex,_float fTimeDelta, MOVETYPE eMove = MOVE_FORWARD);
	vector<CNaviCell*>* GetNaviVec();

private:
	virtual void Free(void);

private:
	vector<CNaviCell*> m_vecCell;
	LPDIRECT3DDEVICE9 m_pDevice;
	
	
};


END

#endif
