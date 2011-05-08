#pragma  once

namespace Auth51{

#if defined(_MSC_VER)
	#if defined(_M_IX86)
		typedef char				Int8;
		typedef unsigned char		UInt8;
		typedef short				Int16;
		typedef unsigned short		UInt16;
		typedef int					Int32;
		typedef unsigned int		UInt32;
		typedef long long			Int64;
		typedef unsigned long long	UInt64;
	#else if defined(_M_IA64)
		typedef char				Int8;
		typedef unsigned char		UInt8;
		typedef short				Int16;
		typedef unsigned short		UInt16;
		typedef int					Int32;
		typedef unsigned int		UInt32;
		typedef long long			Int64;
		typedef unsigned long long	UInt64;
	#endif
#endif	//_MSC_VER

#ifdef __GNUC__
	#ifdef __LP64__
		typedef char	            Int8;
                typedef unsigned char		UInt8;
		typedef short			    Int16;
		typedef unsigned short		UInt16;
		typedef int			        Int32;
		typedef unsigned int		uint32_t;
		typedef long			    Int64;
		typedef unsigned long	    UInt64;
        #endif
#endif // __GNUC__

}


