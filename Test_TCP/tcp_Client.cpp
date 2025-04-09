#include <iostream>
#include <winsock2.h>
#include <thread>
#include <chrono>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        std::cerr << "WSAStartup 실패\n";
        return 1;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "소켓 생성 실패: " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(12345);

    std::cout << "서버에 연결 시도 중...\n";

    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "서버 연결 실패: " << WSAGetLastError() << "\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    std::cout << "서버에 연결 성공!\n";

    for (int i = 0; i < 5; ++i) {
        send(sock, "ABC", 3, 0);
        std::cout << "데이터 전송: ABC\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    closesocket(sock);
    WSACleanup();
    std::cout << "연결 종료.\n";
    return 0;
}
