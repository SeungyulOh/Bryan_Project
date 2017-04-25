#ifndef Base_h__
#define Base_h__

class _declspec(dllexport) CBase
{
protected:
	explicit CBase():m_dwRefCnt(0) {}
	virtual ~CBase(){}

public:
	inline unsigned long AddRefCnt();
	inline unsigned long Release();

protected:
	unsigned long m_dwRefCnt;

private:
	virtual void Free()PURE;

};

#include "Base.inl"

#endif // Base_h__
