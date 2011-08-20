#ifndef _AB_PUBMACRO_H_
#define _AB_PUBMACRO_H_
    
    #define SAFE_DELETE(x) if(x) \
    { \
        delete (x);\
        (x) = NULL;\
}

 	#define SAFE_DELETE_ARRAY(x)  if(x) \
 	{\
 	    delete [](x);\
        (x) = NULL;\
 	}

    #define SAFE_CLOSE_FILE(x)  if(x != INVALID_HANDLE_VALUE) \
    {\
        CloseHandle(x);\
        (x) = INVALID_HANDLE_VALUE;\
    }

    #define SAFE_CLOSE_HANDLE(x) if(x) \
    {\
        CloseHandle(x);\
        (x) = NULL;\
    }

    #define BEGIN_DO_WHILE do{

    #define END_DO_WHILE  }while(FALSE)

    #define BREAK break

#define Z_PROCESS_SUCCESS(x)    \
    do                          \
    {                           \
        if (x)                  \
            goto Exit1;         \
    } while(false)

#define Z_PROCESS_ERROR(x)  \
    do                      \
    {                       \
        if (!(x))           \
            goto Exit0;     \
    } while(false)

#define Z_DELETE_ARRAY(p)   \
    if (p)                  \
    {                       \
        delete [](p);       \
        (p) = NULL;         \
    }

#define Z_DELETE(p) \
    if (p)          \
    {               \
        delete(p);  \
        (p) = NULL; \
    }

    #define Z_PROCESS_ERROR_EX(x, y)  if(!(x))\
    {\
       y;\
       goto Exit0;\
}

#ifdef _DEBUG
    #define BEGIN_CATCH_EXCEPTION
    #define END_CATCH_EXCEPTION(x)
#else
    #define BEGIN_CATCH_EXCEPTION   __try\
    {

#define END_CATCH_EXCEPTION(x)  }\
    __except(x)\
    {\
        goto Exit0;\
    }
#endif
#endif