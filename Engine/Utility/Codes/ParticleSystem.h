#ifndef Engine_ParticleSystem_h__
#define Engine_ParticleSystem_h__

#include "Engine_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CParticleSystem
	:public CGameObject
{
public:
	explicit CParticleSystem(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CParticleSystem();

public:
	virtual bool Init(_uint iMaxNumParticle); // Create Vertex  Buffer and Texture
	virtual void Reset();
	virtual void ResetParticle(PARICLEATT* pAttribute,_uint iIndex) PURE;
	virtual void AddParticle();
	virtual void AddParticle(_uint iIndex);
	virtual void AddParticle(PARICLEATT Attribute, _uint iIndex);
	virtual void MakeInvisible(PARICLEATT* pAttribute, _float fTimeDelta);

	virtual _int Update_GameObject(const _float& fTimeDelta){return 0;}

	virtual void preRender();
	virtual void Render_GameObject();
	virtual void Render_GameObject(LPD3DXEFFECT pEffect, LPDIRECT3DTEXTURE9 pTexture);
	virtual void postRender();

	bool isEmpty();
	bool isDead();

protected:
	virtual void Free(void);

protected:
	virtual void RemoveDeadParticles();

protected:
	D3DXVECTOR3 m_vOrigin; // Particle Starts at this point.
	float m_fEmitRate;	// Particle's create rate (particles per 1sec)
	float m_fSize;	// total particle's size
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	std::list<PARICLEATT> m_AttributeList;
	int m_iMaxParticles; // Maximum Particle Count in given time

	_vec3 m_vBoundingMinMax[2];

	DWORD m_dwVTXSize; // Num of Particles that VB can shave
	DWORD m_dwVTXOffSet; // Particle's Offset to Next Stage
	DWORD m_dwBatchSize; // Particle's Count per a stage;


	list<PARICLEATT>** m_pTrailList;
	_uint m_iMaxTrail;

	//Random
	inline float GetRandomFloat(float lowBound, float HighBound)
	{
		if(lowBound >= HighBound)
			return lowBound;

		float f = (rand() % 10000) * 0.0001f;

		return (f * (HighBound - lowBound) + lowBound);
	}

	inline void GetRandomVector(D3DXVECTOR3* out,D3DXVECTOR3* min,D3DXVECTOR3* max)
	{
		out->x = GetRandomFloat(min->x,max->x);
		out->y = GetRandomFloat(min->y,max->y);
		out->z = GetRandomFloat(min->z,max->z);
	}
	inline DWORD fToDW(float fValue)
	{
		return *(DWORD*)&fValue;
	}
};

static bool Compare(PARICLEATT Src,PARICLEATT Dest)
{
	return Src.fViewZ > Dest.fViewZ;
}

END

#endif // Engine_ParticleSystem_h__
