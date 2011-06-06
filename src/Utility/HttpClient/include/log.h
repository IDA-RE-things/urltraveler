/// @file log.h
/// @brief ��־ģ��

#ifndef _LOG_H_
#define _LOG_H_
#undef _LOG_H_

#include <stdio.h>
#include <time.h>

namespace utils
{

    #ifndef MAX_PATH
    /// ֧�ֵ�����ļ�·������
    #define MAX_PATH    1024
    #endif

    /// ��־����
    enum LogLevel
    {
	    L_FATAL = 0,    ///< �������󣬳��򲻵ò�ֹͣ
	    L_ERROR,        ///< ���ϣ����������Ӵ���
	    L_WARN,         ///< ���棬��Ӱ��ҵ��Ĺ���
	    L_INFO,         ///< ҵ����Ϣ��¼
	    L_TRACE,        ///< �������̸���
	    L_DEBUG,        ///< ������Ϣ
        L_LEVEL_MAX
    };

    /**
        ��־��

        
        ��־�ļ�����ʽ��
            - xxx-20081105-090000.log

        ��־�ļ��ָ
            - �µ�һ�쿪ʼ�������ļ���С�����趨ֵ�������ļ�д��

        �÷���
            <CODE>
            #include <log.h>

            LOG_INIT("xxx", 400000, L_TRACE);

            LOG_OPEN();

            LOG(3)("warn info %d", 3);

            LOG_CLOSE();

            </CODE>
        */
    class Log
    {
    public:
        /**
            ����Log����
        */
	    Log();

        /**
            ����Log����
            
            ����ǰ���ر���־�ļ�
            @see close()
        */
	    ~Log();

    public:
        /**
            ������־�ļ���
            
            ����Ѿ����˾Ͳ�����������
            @param filename �µ���־�ļ���
            @return 0 �ɹ� -1 ���ִ���
        */
	    int set_file_name(const char *filename);

        /**
            ������־�ļ���ģ����
            
            ��������Ч�����´������־ʱ�ż��
            @param maxsize �µĹ�ģ����
            @return 0 �ɹ� -1 ���ִ���
        */
	    int set_max_size(int maxsize);

        /**
            ���������־�ȼ�         
              
            ֻ�Ժ����������־��Ч
            @param level �µļ�������
            @return 0 �ɹ� -1 ���ִ���
        */
	    int set_max_level(LogLevel level);

        /**
            ��ȡ��־�ļ���ģ����
            
            @return ��־�ļ���ģ����
        */
	    size_t get_max_size() {return max_size_;}

        /**
            ��ȡ�����־�ȼ�           
            
            @return �����־�ȼ�
        */
        LogLevel get_max_level() {return max_level_;}

        /**
            ����־�ļ�

            ��ǰ���������ļ���
            @return 0 �ɹ� -1 ���ִ���
            @see set_file_name()
        */
	    int open();

        /**
            �ر���־�ļ�

            �رպ���������־�ļ�
            @return 0 �ɹ� -1 ���ִ���
            @see set_file_name()
        */
	    int close();

    #ifdef WIN32	// for windows
    #define CHECK_FORMAT(i, j)
    #else			// for linux(gcc)
    #define CHECK_FORMAT(i, j) __attribute__((format(printf, i, j)))
    #endif

        /**
            ���һ����־��¼

            @param level ��־�ȼ�
            @param fmt ��ʽ���ַ���
            @return 0 �ɹ� -1 ���ִ���
        */
        int log(LogLevel level, const char * fmt, ...) CHECK_FORMAT(3, 4);

        /// ���һ��FATAL��־��¼
	    int log_fatal(const char * fmt, ...) CHECK_FORMAT(2, 3);

        /// ���һ��ERROR��־��¼
        int log_error(const char * fmt, ...) CHECK_FORMAT(2, 3);

        /// ���һ��WARN��־��¼
        int log_warn(const char * fmt, ...) CHECK_FORMAT(2, 3);

        /// ���һ��INFO��־��¼
        int log_info(const char * fmt, ...) CHECK_FORMAT(2, 3);

        /// ���һ��TRACE��־��¼
        int log_trace(const char * fmt, ...) CHECK_FORMAT(2, 3);

        /// ���һ��DEBUG��־��¼
        int log_debug(const char * fmt, ...) CHECK_FORMAT(2, 3);

    #undef CHECK_FORMAT

        /**
            ��ʮ������dumpһ������

            @param data �����׵�ַ
            @param len ���ݳ���
            @param level ��־�ȼ�
            @return 0 �ɹ� -1 ���ִ���
        */
        int log_hex(unsigned char * data, size_t len, LogLevel level);

    public:
        /// ȫ����־����
        static Log global_log;

    private:
        class Lock;

        /**
            ���һ����־��¼

            @param level ��־�ȼ�
            @param fmt ��ʽ���ַ���
            @param ap ��ʽ������
            @return 0 �ɹ� -1 ���ִ���
            @see log()
        */
	    int vlog(int level, const char* fmt, va_list ap);

    private:
	    /// ��ͬ��־�������ɫ�Լ��ؼ�������
	    static char level_str_[L_LEVEL_MAX][64];

    private:
	    /// ��־�ļ���
	    char file_name_[MAX_PATH];

	    /// ������־�ļ�����ļ���С
	    size_t max_size_;

        /// ��־����
        LogLevel max_level_;

	    /// ��־�ļ��ļ�������
	    FILE * file_;

	    /// �߳���
	    Lock * lock_;

	    /// ���쿪ʼʱ��
        time_t mid_night_;

    };

}

#define LOG(level) LOG_##level

#define LOG_0 LOG_FATAL
#define LOG_1 LOG_ERROR
#define LOG_2 LOG_WARN
#define LOG_3 LOG_INFO
#define LOG_4 LOG_TRACE
#define LOG_5 LOG_DEBUG

#define LOG_FATAL 
#define LOG_ERROR 
#define LOG_WARN
#define LOG_INFO
#define LOG_TRACE 
#define LOG_DEBUG 

#define LOG_HEX(data, len, level) 
#define LOG_INIT(name, size, level) 
#define LOG_OPEN() 
#define LOG_CLOSE() 
#define LOG_SET_LEVEL(level) 
#define LOG_GET_LEVEL() 

#endif
