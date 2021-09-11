#include "MySocket.h"
#include <string>
#include <thread>
#pragma comment(lib, "ws2_32") // 윈도우 소켓 프로그래밍 기능 제공 라이브러리
#define SIZE 5
using namespace std;
using namespace MySocket;

ServerSocket* ms = NULL;
SocketData sktData_list[SIZE];
int skt_no_list[SIZE];
int current_skt_cnt = 0;

void recv_back(int uid) {
	cout << "uid " << uid << endl;
	while (true) {
		ms->set_socket(sktData_list[uid]);
		char msg[100] = { 0 };
		strcpy(msg, ms->recv_data());  
		for (int i = 0; i < current_skt_cnt; i++) {
			if (uid == i) {
				continue;
			}
			ms->set_socket(sktData_list[i]);
			ms->send_data(msg);
		}
	}
}

void accept_back() {
	for (int i = 0; i < SIZE; i++) {
		sktData_list[current_skt_cnt++] = ms->myAccept();
		//skt_no_list[i] = i;
		thread t1(recv_back, i);
		t1.detach();
	}
}

int main() {

	try {
		ms = new ServerSocket(9999, 5);
	}
	catch (int error_code) {
		cout << "소켓 실행 중 문제가 발생했습니다. 에러코드 : " << error_code << endl;
	}

	thread t2(accept_back);

	while (true) {
		cout << "server running.." << endl;
		this_thread::sleep_for(chrono::milliseconds(5000));
	}


	return 0;
}

