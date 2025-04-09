#include <iostream>
#include <winsock2.h>
#include <windows.h> // SetConsoleOutputCP

#pragma comment(lib, "ws2_32.lib")

int main() {
    SetConsoleOutputCP(CP_UTF8); // 콘솔 한글 출력 깨짐 방지

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);

    SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));

    std::cout << "✅ UDP 서버 대기 중... (포트 12345)\n";

    char buffer[1024];
    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);

    const int expectedPackets = 10000;
    int receivedCount = 0;

    while (receivedCount < expectedPackets) {
        int len = recvfrom(serverSocket, buffer, sizeof(buffer) - 1, 0,
                           (sockaddr*)&clientAddr, &clientAddrSize);
        if (len == SOCKET_ERROR) break;

        buffer[len] = '\0';
        ++receivedCount;
    }

    std::cout << "📥 총 받은 메시지 수: " << receivedCount << "\n";
    std::cout << "❌ 손실된 메시지 수: " << (expectedPackets - receivedCount) << "\n";
    std::cout << "📊 손실률: " << 100.0 * (expectedPackets - receivedCount) / expectedPackets << "%\n";

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
