#ifndef Engine_Functor_h__
#define Engine_Functor_h__

namespace Engine
{

	class CRelease_Single
	{
	public:
		explicit CRelease_Single(){}
		virtual ~CRelease_Single(){}

	public:
		template<typename T>
		void operator() (T& pointer)
		{
			if(pointer != NULL)
			{
				Engine::Safe_Release(pointer);
			}
		}
	};


	class CRelease_Pair
	{
	public:
		explicit CRelease_Pair(){}
		virtual ~CRelease_Pair(){}

	public:
		template<typename T>
		void operator() (T& pair)
		{
			if(pair.second != NULL)
			{
				Engine::Safe_Release(pair.second);
			}
			
		}
	};



	class CTagFinder
	{
	public:
		explicit CTagFinder(const TCHAR* pString) :m_pString(pString) {}
		virtual ~CTagFinder() {}

	public:
		template<typename T>
		bool operator() (T& pair)
		{
			bool bCheck = !lstrcmp(m_pString,pair.first);
			if(true == bCheck)
				return true;

			return false;
		}

	private:
		const TCHAR* m_pString;
	};
}



#endif // Engine_Functor_h__
