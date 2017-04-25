#ifndef Typedef_h__
#define Typedef_h__

typedef signed char		_byte;
typedef unsigned char	_ubyte;

typedef signed short	_short;
typedef unsigned short	_ushort;

typedef signed int		_int;
typedef unsigned int	_uint;

typedef signed long		_long;
typedef unsigned long	_ulong;

typedef float			_float;
typedef double			_double;

typedef bool			_bool;
typedef wchar_t			_tchar;

typedef D3DXVECTOR2 _vec2;
typedef D3DXVECTOR3 _vec3;
typedef D3DXVECTOR4 _vec4;

typedef D3DXMATRIX _matrix;

typedef struct tagEffect
{
	_matrix matLocal;
	float fSlope;
	float fScale;
	float fEffectRadius;
	float fStartRatio;
	float fLifeTime;
	_vec3 fAtkRange;
}EFFECTDATA;

typedef struct tagExpEffect
{
	_bool isDistortion;
	_int iImgIdx;
	_float fEmitInterval;
	_float fEmitDuration;
	_int iParticleCount;
	_vec3 vPositionOffSet;
	_vec3 vParticleCount[2];
	_vec3 vColorMin;
	_vec3 vColorMax;
	_vec2 vDieTime;
	_vec2 vSpeed;
	_vec3 vSpeedDelta[10];
	_vec2 vSpreadAngle;
	_vec2 vSpreadRadius;
	_vec2 vSpreadRadiusAngle;
	_vec2 vStartSize;
	_vec3 vSizeDelta[10];
	_vec3 vAlphaDelta[10];
	_vec2 vStartRoll;
	_vec3 vRollDelta[10];
	_vec2 vSpreadCylinderRadius[10];
	_vec2 vSpreadCylinderHeight[10];
	_vec3 vOffSetAngle;
	_vec3 vMyOffSetPos;
	_bool isBillBoard;
	_bool isDecrease;
	_vec2 vLength;
	_vec2 vWidth;
	_vec2 vLengthDelta[2];
	_vec2 vWidthDelta[2];
	_float fLookDist;
	_vec2 vLookDistRand;
	_bool isAttached;
	_bool isLoop;
	_uint iProbabilityForLoop;
	_bool isDestAlphaOne;
	_vec2 vGravity;
	_bool isStartedFromLand;
	_vec2 vUV;
	_float fLoopTime;
	_float fOffSetAngleY;
	_vec3 vWorldOffSetPos;
	_bool isDealing;
}EFXDATA;



#endif // Typedef_h__
