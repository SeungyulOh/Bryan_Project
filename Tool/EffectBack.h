#ifndef EffectBack_h__
#define EffectBack_h__

#include "Engine_Defines.h"
#include "GameObject.h"
#include "Player_Tool.h"

class CEffectBack : public Engine::CGameObject
{
private:
	explicit CEffectBack(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffectBack(void);

public:
	void Ready_EffectBack(void);
	void Render_GameObject();
	void Render_AxisLine();

public:
	CPlayer_Tool* m_pPlayerTool;

private:
	_vec2 m_AxisX[2];
	_vec2 m_AxisY[2];
	_vec2 m_AxisZ[2];

public:
	static CEffectBack* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void);
};

#endif // EffectBack_h__
