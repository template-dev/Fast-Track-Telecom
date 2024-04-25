#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    const int PORT = 12346;

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Ошибка создания сокета\n";
        return 1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Ошибка привязки сокета\n";
        close(serverSocket);
        return 1;
    }

    if (listen(serverSocket, 10) == -1) {
        std::cerr << "Ошибка прослушивания сокета\n";
        close(serverSocket);
        return 1;
    }

    std::cout << "Сервер запущен. Ожидание клиентов...\n";

    while (true) {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (clientSocket == -1) {
            std::cerr << "Ошибка при принятии соединения\n";
            continue;
        }

        std::cout << "Подключился клиент с IP: " << inet_ntoa(clientAddr.sin_addr) << ", порт: " << ntohs(clientAddr.sin_port) << std::endl;

        while (true) {
            char buffer[1024];
            int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
            if (bytesReceived == -1) {
                std::cerr << "Ошибка при приеме данных\n";
                close(clientSocket);
                break;
            }

            if (bytesReceived == 0) {
                std::cout << "Клиент отключился\n";
                close(clientSocket);
                break;
            }

            std::cout << "Получено от клиента [" << inet_ntoa(clientAddr.sin_addr) << "]: " << std::string(buffer, bytesReceived) << std::endl;

            if (std::string(buffer, bytesReceived) == "/close") {
                if (send(clientSocket, "Соединение потеряно.", std::strlen("Соединение потеряно."), 0) == -1) {
                    std::cerr << "Ошибка при отправке данных\n";
                    close(clientSocket);
                    break;
                }
                std::cout << "Соединение с клиентом " << inet_ntoa(clientAddr.sin_addr) << " потеряно." << std::endl;
                close(clientSocket);
                break;
            }

            const char* answerFromServer = "Данные получены!";
            if (send(clientSocket, answerFromServer, std::strlen(answerFromServer), 0) == -1) {
                std::cerr << "Ошибка при отправке данных\n";
                close(clientSocket);
                break;
            }
        }
    }

    close(serverSocket);

    return 0;
}
