#include "MySocket.h"

using namespace MySocket;
Socket::Socket(int port) {

	//memset(buffer_in, 0, sizeof(buffer_in)); // buff의 모든 영역을 0으로 초기화
	//memset(buffer_out, 0, sizeof(buffer_out)); // buff의 모든 영역을 0으로 초기화
	WSAStartup(MAKEWORD(2, 2), &wsaData); // 윈도우의 socket관련 기능 사용 시작

	// socket(AF_INET, SOCK_STREAM, 0) ip v4, TCP/UDP, 0
	curr_socket_data.target_sock = make_socket();

	// 소켓 얻는 과정에서 에러가 나면 에러처리
	if (curr_socket_data.target_sock == SOCKET_ERROR) {
		cout << "Socket Error!!" << endl;
		throw SOCKET_ERROR;
	}
}

Socket::Socket(int port, const char* ip) : MySocket::Socket(port) {
	set_addr(curr_socket_data.target_addr, port, inet_addr(ip));
	connect(curr_socket_data.target_sock, (sockaddr*)&curr_socket_data.target_addr, 0);
}

SOCKET Socket::make_socket() {
	return socket(AF_INET, SOCK_STREAM, 0);
}

void Socket::set_socket(SocketData socketData) {
	this->curr_socket_data = socketData;
}

SocketData Socket::get_socket() {
	return this->curr_socket_data;
}

void Socket::set_addr(sockaddr_in& addr, int port, ULONG ip) {
	addr.sin_family = AF_INET; // 주소체계
	addr.sin_port = htons(port); // 접속할 서버소켓 포트번호(9000~65535)
	addr.sin_addr.S_un.S_addr = ip;
}

void Socket::send_data(const char* msg) {
	memset(buffer_out, 0, sizeof(buffer_out));
	strcpy(buffer_out, msg);
	send(curr_socket_data.target_sock, buffer_out, MAX_BUFF_SIZE, 0);
}

char* Socket::recv_data() {
	memset(buffer_in, 0, sizeof(buffer_in));
	recv(curr_socket_data.target_sock, buffer_in, MAX_BUFF_SIZE, 0);
	return buffer_in;
}

void Socket::print_data() {
	char* data = recv_data();
	cout << data << endl;
}

ServerSocket::ServerSocket(int port, int size) : Socket(port) {
	skt_list = new SocketData[size];
	skt_size = size;
	serv_sock = make_socket();
	set_addr(serv_addr, port, ADDR_ANY);
	cout << "서버 소켓 생성 및 주소 생성 완료" << endl;
	cout << "소켓 번호 : " << serv_sock << endl;
	cout << "소켓 주소 : " << inet_ntoa(serv_addr.sin_addr) << endl;
	//memset(&socketData, 0, sizeof(socketData));
	bind(serv_sock, (sockaddr*)&serv_addr, sizeof(serv_addr));
	cout << "소켓 바인딩 완료" << endl;
	listen(serv_sock, 5);
	cout << "리슨 시작" << endl;
}

SocketData ServerSocket::myAccept() {
	SocketData new_socket_data = {0};
	int len = sizeof(new_socket_data.target_addr);
	cout << "수신 대기.." << endl;
	cout << "수신자 소켓 정보 : " << serv_sock << endl;
	cout << "수신자 주소 정보 : " << inet_ntoa(serv_addr.sin_addr) << endl;
	new_socket_data.target_sock = accept(serv_sock, (sockaddr*)&new_socket_data.target_addr, &len);
	cout << "수신 완료" << endl;
	if (new_socket_data.target_sock == SOCKET_ERROR) {
		cout << "accept error!!" << endl;
		throw SOCKET_ERROR;
	}
	this->curr_socket_data = new_socket_data;
	cout << "접속자 : " << inet_ntoa(new_socket_data.target_addr.sin_addr) << endl;
	return new_socket_data;
}

//void ServerSocket::MyAccept() {
//	SOCKET cli_sock = 0;
//	sockaddr_in cli_addr = { 0 };
//
//	char buff[MAX_BUFF_SIZE] = { 0 };
//	int len = sizeof(cli_addr);
//	strcpy(buff, "welcome to my server");
//
//	for (int i = 0; i < list_size; i++) {
//		cout << "대기.." << endl;
//		sock_list[i] = accept(sock, (sockaddr*)&cli_addr, &len);
//		cout << "create : " << i << endl;
//		if (sock_list[i] == SOCKET_ERROR) {
//			cout << "accept error!!" << endl;
//			return;
//		}
//		send(sock_list[i], buff, sizeof(buff), 0);
//		char* rcv_msg[MAX_BUFF_SIZE] = { 0 };
//		thread t2(recv_data, sock_list[i]);
//		t2.detach();
//	}
//}