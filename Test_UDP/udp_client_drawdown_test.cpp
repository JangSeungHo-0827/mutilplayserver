#include <iostream>
#include <winsock2.h>
#include <chrono>

#pragma comment(lib, "ws2_32.lib")



int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    const int totalPackets = 10000;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < totalPackets; ++i) {
        sendto(sock, "ABC", 3, 0, (sockaddr*)&serverAddr, sizeof(serverAddr));

        // ⚠️ sleep 없이 매우 빠르게 전송함으로써 수신 버퍼를 초과시키고,
        // 자연스럽게 일부 패킷이 손실되도록 유도함
        // std::this_thread::sleep_for(std::chrono::milliseconds(0)); ← 일부러 제거함
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "📤 전송 완료 (" << totalPackets << "개), 소요 시간: " << duration.count() << "ms\n";

    closesocket(sock);
    WSACleanup();
    return 0;
}
