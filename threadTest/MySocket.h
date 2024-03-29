#ifndef MY_SOCKET
#define MY_SOCKET
#include <WinSock2.h>
#include <iostream>
#include <thread>
#include <vector>
#define MAX_BUFF_SIZE 100

using namespace std;

namespace MySocket {
	typedef struct SocketData {
		SOCKET target_sock; // 서버 소켓
		sockaddr_in target_addr; // 서버 주소(IP)
	} SocketData;

	class Socket {
	protected:
		SocketData curr_socket_data;
		WSADATA wsaData;
		char buffer_out[MAX_BUFF_SIZE];
		char buffer_in[MAX_BUFF_SIZE];

		void set_addr(sockaddr_in& addr, int port, ULONG ip);
		SOCKET make_socket();
	public:
		Socket(int port);
		Socket(int port, const char* ip);
		char* recv_data();
		void send_data(const char* msg);
		void print_data();
		void set_socket(SocketData socket);
		SocketData get_socket();
	};

	class ServerSocket : public Socket {
	private:
		int skt_size;
		SocketData* skt_list;
		SOCKET serv_sock;
		sockaddr_in serv_addr;
	public:
		ServerSocket(int port, int size);
		SocketData myAccept();
	};
}
#endif
