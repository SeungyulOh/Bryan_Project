#ifndef Engine_Function_h__
#define Engine_Function_h__

namespace Engine
{
	template <typename T>
	void Safe_Delete(T& Pointer)
	{
		if(Pointer != NULL)
		{
			delete Pointer;
			Pointer = NULL;
		}
	}

	template <typename T>
	void Safe_Delete_Array(T& Pointer)
	{
		if(Pointer != NULL)
		{
			delete []Pointer;
			Pointer = NULL;
		}
	}

	template <typename T>
	unsigned long Safe_Release(T& Pointer)
	{
		unsigned long dwRefCnt = 0;
		if(Pointer != NULL)
		{
			dwRefCnt = Pointer->Release();

			if(dwRefCnt == 0)
				Pointer = NULL;
		}
		return dwRefCnt;
	}
}


#endif // Engine_Function_h__
