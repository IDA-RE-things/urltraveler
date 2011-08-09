#ifndef __SOCKETOBJECT_I_H__
#define __SOCKETOBJECT_I_H__

#include "SocketObjectDefine.h"

namespace NetModule
{
	class ISocketEvent;
	/**
	 * @ingroup network
	 * @brief ����ͨѶģ��
	 *
	 * <p>
	 *���� ��ģ���ڲ�ʹ���˶��̡߳�
	 * </p>
	 *
	 * <p>
	 *���� ISocketObject �Ǹ���ͨѶ����ĸ��࣬ ISocketObject �ж�����࣬�ֱ����ͬ���͵�
	 *     ͨѶ������� ITCPSocketObject , IUDPSocketObject ���ֱ����ʹ�� TCP Э��� UDP Э
	 *     ��ͨѶ���������ֱ�Ӵ��� ISocketObject ���͵������ֻ�ܴ�����������ͨѶ�����
	 * </p>
	 *
	 * <p>
	 * ISocketObject �������Ͱ����ˣ�
	 * <ol>
	 *   <li>ITCPSocketObject �� ʹ��TCPЭ���ͨѶģ��</li>
	 *   <li>IUDPSocketObject �� ʹ��UDPЭ���ͨѶģ��</li>
	 *   <li>ISock5TCPProxyObject �� ʹ��Sock5����TCPЭ���ͨѶģ��</li>
	 *   <li>IHttpProxyObject �� ʹ��Http����TCPЭ���ͨѶģ��</li>
	 * </ol>
	 * </p>
	 *
	 * <p>
	 * ʹ������
	 * <ol>
	 *   <li>ѡ��һ�������͵� SocketObject ���д���</li>
	 *   <li>ʹ�� Create() ��������, ������������Ϣ��ͬʱ�����¼��ص��ӿ�</li>
	 *   <li>ʹ�� Connect() �������ӷ�����</li>
	 *   <li>���¼��ص��ӿ����յ� ISocketEvent::OnConnectSuccess() ��Ϣ����ʾ���ӳɹ�</li>
	 *   <li>ʹ�� Send() ������������</li>
	 *   <li>�����������Ҫ����, �¼��ص��ӿڵ� ISocketEvent::OnReceived() �ᱻ����, �� ISocketEvent::OnReceived()
	 *      ����ȡ����</li>
	 *   <li>ʹ�� Disconnect() �����Ͽ�����</li>
	 * </ol>
	 * </p>
	 *
	 * @sa ITCPSocketObject , IUDPSocketObject , ISock5TCPProxyObject , IHttpProxyObject
	 */
    class ISocketObject
	{
	public:
		/**
		 *  @brief ��������ʼ��
		 *  @param lpParam ָ��һ�� SOCKETPARAM ���ͽṹ��
		 *  @param pEventHandler �¼��ص�ָ��
		 *  @return ��������ɹ�, ���� TRUE
		 *  @sa �ṹ�嶨��:SOCKETPARAM ����: tagProxyParam ��
		 */
		virtual BOOL Create(LPVOID lpParam, ISocketEvent* pEventHandler) = 0;
		
		/**
		 *  @brief ����Զ�̷�����
		 *  @param lpszHost ��������ַ������������"www.qq.com"��Ҳ������IP "202.10.22.11"
		 *  @param ushPort ��Ҫ���ӵĶ˿�
		 *  @return ����ɹ��ط�������������, ���� TRUE
		 */
		virtual BOOL Connect(LPTSTR lpszHost, USHORT ushPort) = 0;
		
		/**
		 *  @brief ����Զ�̷�����
		 *  @param ulHostIP ������IP��Ϊ�����ֽ���
		 *  @param ushPort ��Ҫ���ӵĶ˿�
		 *  @return ����ɹ��ط�������������, ���� TRUE
		 */
		virtual BOOL Connect(ULONG ulHostIP, USHORT ushPort) = 0;
		
		/**
		 *  @brief �Ͽ�����
		 *  @return ����ɹ��ضϿ�������, ���� TRUE
		 */
		virtual BOOL Disconnect() = 0;
		
		/**
		 *  @brief ��������
		 *  @return ���Ͷ����ɹ����򷵻� TRUE
		 */
		virtual BOOL Send(LPBYTE lpBytes, int nLen) = 0;

		/**
		 * @ingroup
		 * @brief ��������connect()֮ǰ���ò���Ч
		 * @param dwMode : socketģʽ��SOCKETMODE_DELAYRECEIVE : �ӳٶ�����
		 * @return
		 * @see
		 */
 		//virtual void SetMode(DWORD dwMode) = 0;
 
		/**
		 * @brief ��ģʽΪ SOCKETMODE_DELAYRECEIVE ʱ���������������������ӳ�ʱ��(���ݴ������)
		 * @param  dwBandWidth [in] : ������λ��KB/S
		 * @see
		 */
		virtual void SetBandWidth(DWORD dwBandWidth) = 0;	

		/**
		* @brief ��ģʽΪ SOCKETOPT_CACHEPACKET ʱ��������������������Cache Packet����С
		*
		* @param byPacketSize [in] : Packet Size, ��λ��KB
		*
		* @return
		*
		* <p>
		* </p>
		*
		* @see
		*/
		virtual void SetMaxPacketSize(BYTE byPacketSize) = 0;

		/**
		 * @brief  ����Socket Option.����Ϊ����ѡ������
		 *			  SOCKETOPT_CACHEPACKET : Cache Packet ģʽ
		 *			  SOCKETOPT_DELAYRECEIVE : �ӳٶ�ģʽ
		 * 
		 * @param  pSocketOption [in] : Socketģʽ
		 * @see PSOCKETOPTION
		 */
		virtual void SetOption(PSOCKETOPTION pSocketOption) = 0;
        /**
        * @brief �ͷ�Socket����
        * @param
        * @param
        * @return 
        */
        virtual void Release() = 0;

	};
	
	/**
	 * @ingroup network
	 * @brief ʹ��TCPЭ���ͨѶģ��
	 *
	 * ʹ�÷�����ο� ISocketObject
	 */
	class ITCPSocketObject : public ISocketObject
	{
	};
		
	/**
	 * @ingroup network
	 * @brief ISocketObject ���¼��ص��ӿ�
	 * @sa ISocketObject
	 */
	class ISocketEvent
	{
	public:
		/**
		 * @brief ���ӳɹ�
		 */
		virtual void OnConnectSuccess() = 0;

		/**
		 * @brief ����ʧ��
		 * @param nErrorCode �������
		 */
		virtual void OnConnectFail(int nErrorCode) = 0;

		/**
		 * @brief ��������
		 * @param lpBytes ָ���յ�������
		 * @param nLen    ���յ������ݵĳ���
		 */
		virtual void OnReceived(LPBYTE lpBytes, int nLen) = 0;

		/**
		 * @brief ���ӱ��ر�
		 * @param nErrorCode �������
		 */
		virtual void OnClosed(int nErrorCode) = 0;

	};
	
#define SOCKETOBJECT_SUBFOLDER					_T("Socket\\")
        UTILITY_API ITCPSocketObject* CreateSocketObject();
};
#endif
