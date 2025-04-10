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

    SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "소켓 생성 실패: " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(12345);

    std::cout << "UDP 서버에 데이터 전송 시작...\n";

    for (int i = 0; i < 20; ++i) {
        sendto(clientSocket, "ABC", 3, 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
        std::cout << "📤 데이터 전송: ABC\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    closesocket(clientSocket);
    WSACleanup();
    std::cout << "전송 완료.\n";
    return 0;
}

