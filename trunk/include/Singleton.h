#ifndef _TAB_SIGLETON_H_
#define _TAB_SIGLETON_H_

#include "Guard.h"

template <typename T>
class CSingleton
{
	CLASS_UNCOPYABLE(CSingleton)
public:
	 static T* Instance()
	 {
		 if (NULL == ms_pInstance)
		 {
			 CGuard objGuard(ms_lock);
			 
			 if (NULL == ms_pInstance)
			 {
			      ms_pInstance = new T();
// 			      assert(ms_pInstance);
			      _onexit(OnExit);
			 }		
		 }

		 return ms_pInstance;
	 }

     VOID Release()
     {
         if (NULL != ms_pInstance)
         {
             CGuard objGuard(ms_lock);
             OnExit();
         }
     }
	 
protected:
	CSingleton(){};
	~CSingleton(){};
private:
	 static int __cdecl OnExit()
	 {
		 if (ms_pInstance != NULL)
		 {
			delete ms_pInstance;
			ms_pInstance = NULL;
		 }
		 
		 return 0;	 
	 }
private:
	static T* volatile ms_pInstance;
	static CMutex ms_lock;
};

template<typename T> CMutex CSingleton<T>::ms_lock;
template<typename T> T* volatile CSingleton<T>::ms_pInstance = NULL;

#endif //_TAB_SIGLETON_H_