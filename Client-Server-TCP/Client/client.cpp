#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define CLIENT_TAG "[CLIENT]: "

int main() {
    const char *SERVER_IP = "127.0.0.1";
    constexpr int SERVER_PORT = 12343;

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << CLIENT_TAG << "Ошибка создания сокета\n";
        return 1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << CLIENT_TAG << "Ошибка подключения к серверу\n";
        close(clientSocket);
        return 1;
    }

    std::cout << CLIENT_TAG << "Подключение к серверу выполнено успешно.\n";

    bool isRunning = true;
    while (isRunning) {
        std::string message;
        std::cout << CLIENT_TAG << "Введите сообщение: ";
        std::getline(std::cin, message);

        if (message == "/close") {
            isRunning = false;
            continue;
        }

        if (send(clientSocket, message.c_str(), message.size(), 0) == -1) {
            std::cerr << CLIENT_TAG << "Ошибка отправки данных на сервер\n";
            isRunning = false;
            break;
        }

        std::cout << CLIENT_TAG << "Данные успешно отправлены на сервер!" << std::endl;

        char buffer[1024];
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived == -1) {
            std::cerr << CLIENT_TAG << "Ошибка при получении данных от сервера\n";
            isRunning = false;
            break;
        } else if (bytesReceived == 0) {
            std::cout << CLIENT_TAG << "Сервер отключился\n";
            isRunning = false;
            break;
        }

        std::cout << CLIENT_TAG << "Получено подтверждение от сервера: " << std::string(buffer, bytesReceived) << std::endl;
    }

    close(clientSocket);

    return 0;
}
