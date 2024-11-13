#include "common.h"
using namespace std;

void startAdmin(const string& groupName, const string& personalName,string SERVER_IP,int SERVER_PORT);
void startClient(const string& personalName,string SERVER_IP,int SERVER_PORT);

 int SERVER_PORT ;
 string SERVER_IP ;

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed." << endl;
        return 1;
    }

    cout << "Enter 1 to be Admin or 2 to be User: ";
    int choice;
    cin >> choice;

    string personalName;
    cout << "Enter your name: ";
    cin >> personalName;

    if (choice == 1) {
        cout << "Enter your group name: " ;
        string groupName;
        cin >> groupName;
        getline(cin, groupName);
        cout << "Enter server IP: ";
        cin >> SERVER_IP;
        cout << "Enter server port: ";
        cin >> SERVER_PORT;
        startAdmin(groupName, personalName,SERVER_IP,SERVER_PORT);
    }
    else if (choice == 2) {
        cout << "Enter server IP: ";
        cin >> SERVER_IP;
        cout << "Enter server port: ";
        cin >> SERVER_PORT;
        startClient(personalName,SERVER_IP,SERVER_PORT);
    }

    WSACleanup();
    return 0;
}
