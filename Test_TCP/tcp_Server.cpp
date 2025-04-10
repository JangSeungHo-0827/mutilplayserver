#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    
    SetConsoleOutputCP(CP_UTF8); // 콘솔 출력 문자셋 UTF-8 설정
    SetConsoleCP(CP_UTF8);       // 콘솔 입력 문자셋도 (필요시)
    
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        std::cerr << "WSAStartup 실패\n";
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
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

    if (listen(serverSocket, 1) == SOCKET_ERROR) {
        std::cerr << "listen 실패: " << WSAGetLastError() << "\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "✅ TCP 서버 대기 중... (127.0.0.1:12345)\n";

    SOCKET clientSocket = accept(serverSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "accept 실패: " << WSAGetLastError() << "\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "🎉 클라이언트 연결됨!\n";

    char buffer[1024];
    int received;
    while ((received = recv(clientSocket, buffer, sizeof(buffer)-1, 0)) > 0) {
        buffer[received] = '\0';
        std::cout << "📨 받은 데이터: " << buffer << std::endl;
    }

    std::cout << "❌ 클라이언트 연결 종료됨\n";

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
