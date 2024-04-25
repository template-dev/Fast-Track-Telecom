#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    const char *SERVER_IP = "127.0.0.1";
    const int SERVER_PORT = 12344;

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Ошибка создания сокета\n";
        return 1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Ошибка подключения к серверу\n";
        close(clientSocket);
        return 1;
    }

    std::cout << "Подключение к серверу выполнено успешно.\n";

    bool isRunning = true;
    while(isRunning) {
        if (!isRunning) {
            break;
        }

        std::string message = "";
        std::cout << "Введите сообщение: ";
        std::getline(std::cin, message);
        if (send(clientSocket, message.data(), message.size(), 0) == -1) {
            std::cerr << "Ошибка отправки данных на сервер\n";
            isRunning = false;
        }

        std::cout << "Данные отправлены на сервер: " << message << std::endl;

        char buffer[1024];
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived == -1) {
            std::cerr << "Ошибка при получении данных от сервера\n";
            isRunning = false;
            break;
        } else if (bytesReceived == 0) {
            std::cout << "Сервер отключился\n";
            isRunning = false;
            break;
        }

        std::cout << "[SERVER]: " << std::string(buffer, bytesReceived) << std::endl;
    }
    close(clientSocket);

    return 0;
}
