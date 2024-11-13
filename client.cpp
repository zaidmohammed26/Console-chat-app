#include "common.h"
using namespace std;
void startClient(const string& personalName,string SERVER_IP,int SERVER_PORT) {
    
    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_socket == INVALID_SOCKET) {
        cerr << "Failed to create socket" << endl;
        return;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP.c_str(), &server_addr.sin_addr);
    server_addr.sin_port = htons(SERVER_PORT);

    if (connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        cerr << "Connection to server failed." << endl;
        closesocket(client_socket);
        return;
    }

    cout << "Connected to group chat!" << endl;
    send(client_socket, personalName.c_str(), personalName.size(), 0);

    thread receiveThread([client_socket]() {
        char buffer[1024];
        while (true) {
            memset(buffer, 0, sizeof(buffer));
            int bytesReceived = recv(client_socket, buffer, sizeof(buffer), 0);
            if (bytesReceived <= 0) {
                cerr << "Disconnected from server." << endl;
                closesocket(client_socket);
                return;
            }
            cout << buffer << endl;
            cout << "---------------" << endl;
        }
        });

    string message;
    while (true) {
        getline(cin, message);
        if (send(client_socket, message.c_str(), message.size(), 0) == SOCKET_ERROR) {
            cerr << "Message sending failed." << endl;
            break;
        }
        cout << "---------------" << endl;
    }

    receiveThread.join();
    closesocket(client_socket);
}
