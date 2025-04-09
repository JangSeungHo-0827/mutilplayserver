#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        std::cerr << "WSAStartup 실패\n";
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "소켓 생성 실패: " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(12345);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "bind 실패: " << WSAGetLastError() << "\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "✅ UDP 서버 대기 중... (포트 12345)\n";

    char buffer[1024];
    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    int received;

    while (true) {
        received = recvfrom(serverSocket, buffer, sizeof(buffer)-1, 0,
                            (sockaddr*)&clientAddr, &clientAddrSize);
        if (received == SOCKET_ERROR) {
            std::cerr << "recvfrom 실패: " << WSAGetLastError() << "\n";
            break;
        }

        buffer[received] = '\0';
        std::cout << "📨 받은 데이터: " << buffer << std::endl;
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
