#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <vector>

#define SERVER_TAG "[SERVER]: "

void handleClient(int clientSocket, const sockaddr_in& clientAddr) {
    while (true) {
        char buffer[1024];
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived == -1) {
            std::cerr << SERVER_TAG << "Ошибка при приеме данных\n";
            close(clientSocket);
            break;
        }

        if (bytesReceived == 0) {
            std::cout << SERVER_TAG << "Клиент отключился [" << inet_ntoa(clientAddr.sin_addr) << "]:[" << ntohs(clientAddr.sin_port) << "]";
            close(clientSocket);
            break;
        }

        std::cout << SERVER_TAG << "Получено сообщение от клиента [" << inet_ntoa(clientAddr.sin_addr) << "]:[" << ntohs(clientAddr.sin_port) << "]: " << std::string(buffer, bytesReceived) << std::endl;

        std::string connectionLostMessage = std::string(SERVER_TAG) + "Соединение потеряно!";
        if (std::string(buffer, bytesReceived) == "/close") {
            if (send(clientSocket, connectionLostMessage.data(), connectionLostMessage.size(), 0) == -1) {
                std::cerr << SERVER_TAG << "Ошибка при отправке данных\n";
                close(clientSocket);
                break;
            }
            std::cout << SERVER_TAG << "Соединение с клиентом [" << inet_ntoa(clientAddr.sin_addr) << "][" << ntohs(clientAddr.sin_port) << "] потеряно." << std::endl;
            close(clientSocket);
            break;
        }

        std::string answerFromServer = std::string(SERVER_TAG) + "Данные получены!";
        if (send(clientSocket, answerFromServer.data(), answerFromServer.size(), 0) == -1) {
            std::cerr << SERVER_TAG << "Ошибка при отправке данных\n";
            close(clientSocket);
            break;
        }
    }
}

int main() {
    constexpr int PORT = 12343;

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << SERVER_TAG << "Ошибка создания сокета\n";
        return 1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << SERVER_TAG << "Ошибка привязки сокета\n";
        close(serverSocket);
        return 1;
    }

    if (listen(serverSocket, 10) == -1) {
        std::cerr << SERVER_TAG << "Ошибка прослушивания сокета\n";
        close(serverSocket);
        return 1;
    }

    std::cout << SERVER_TAG << "Сервер запущен. Ожидание клиентов...\n";

    while (true) {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (clientSocket == -1) {
            std::cerr << SERVER_TAG << "Ошибка при принятии соединения\n";
            continue;
        }

        std::cout << SERVER_TAG << "Подключился клиент с IP: [" << inet_ntoa(clientAddr.sin_addr) << "], порт: [" << ntohs(clientAddr.sin_port) << "]" << std::endl;

        std::thread clientThread(handleClient, clientSocket, clientAddr);
        clientThread.detach();
    }

    close(serverSocket);

    return 0;
}
