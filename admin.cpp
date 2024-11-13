#include "common.h"
using namespace std;
vector<SOCKET> clientSockets;
mutex clientsMutex;

void handleClient(SOCKET clientSocket) {
    char buffer[1024];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived <= 0) {
        cerr << "Failed to receive client name." << endl;
        closesocket(clientSocket);
        return;
    }
    string personalName(buffer, bytesReceived); 

    cout << personalName << " has joined the group." << endl;
    for (SOCKET sock : clientSockets) {
        if(sock!=clientSocket)
        send(sock, ( personalName + " has joined the group").c_str(), (personalName + " has joined the group").size(), 0);
    }
    while (true) {
        memset(buffer, 0, sizeof(buffer));
         bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            closesocket(clientSocket);
            lock_guard<mutex> lock(clientsMutex);
            clientSockets.erase(remove(clientSockets.begin(), clientSockets.end(), clientSocket), clientSockets.end());
            cout << personalName<<" has left" << endl;
            for (SOCKET sock : clientSockets) {
                if (sock != clientSocket)
                    send(sock, (personalName + " has left the group").c_str(), (personalName + " has left the group").size(), 0);
            }
            break;
        }
        cout <<personalName<<": "<< buffer << endl;
        string msg = personalName + ":" + string(buffer);
        lock_guard<mutex> lock(clientsMutex);
        for (SOCKET sock : clientSockets) {
            if (sock != clientSocket) {
                send(sock, msg.c_str(), msg.length(), 0);
            }
        }
    }
}

void startAdmin(const string& groupName, const string& personalName,string SERVER_IP,int SERVER_PORT) {
    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == INVALID_SOCKET) {
        cerr << "Failed to create socket" << endl;
        return;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP.c_str(), &server_addr.sin_addr);
    server_addr.sin_port = htons(SERVER_PORT);

    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        cerr << "Binding failed." << endl;
        closesocket(server_socket);
        return;
    }

    if (listen(server_socket, 5) == SOCKET_ERROR) {
        cerr << "Failed to listen." << endl;
        closesocket(server_socket);
        return;
    }

    cout << "Admin " << personalName << " created group: " << groupName << " on IP " << SERVER_IP << endl;
    cout << "Waiting for users to join..." << endl;

    while (true) {
        SOCKET clientSocket = accept(server_socket, nullptr, nullptr);
        if (clientSocket != INVALID_SOCKET) {
            {
                lock_guard<mutex> lock(clientsMutex);
                clientSockets.push_back(clientSocket);
            }
            thread(handleClient, clientSocket).detach();
        }
    }
}
