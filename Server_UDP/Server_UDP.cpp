#include <iostream>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")
using namespace std;

void main()
{
    //Start Winsock
    WSADATA data;
    WORD version = MAKEWORD(2, 2);
    int wsOK = WSAStartup(version, &data);
    if (wsOK != 0) {
        cout << "Can't Start Winsock!\n" << wsOK;
        return;
    }

    //Binding socket ---> ip_addr & port
    SOCKET in = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in serverHint;
    serverHint.sin_addr.S_un.S_addr = ADDR_ANY;
    serverHint.sin_family = AF_INET;
    serverHint.sin_port = htons(54000);

    if (bind(in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR)
    {
        cout << "Can't bind socket! " << WSAGetLastError() << endl;
        return;
    }

    sockaddr_in client;
    int clientLength = sizeof(client);
    ZeroMemory(&client, clientLength);
    char buffer[1024];

    //listen client for message
    //print message and client info
    while (true)
    {
        ZeroMemory(buffer, 1024);
        int bytesIn = recvfrom(in, buffer, 1024, 0, (sockaddr*)&client, &clientLength);
        if (bytesIn == SOCKET_ERROR) {
            cout << "Can't receiving from client " << WSAGetLastError() << endl;
            continue;
        }

        char clientIp[256];
        ZeroMemory(clientIp, 256);

        inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);

        cout << "Message recieved from " << clientIp << " : " << buffer << endl;
    }

    //close socket
    closesocket(in);

    //shutdown ws
    WSACleanup();
}
