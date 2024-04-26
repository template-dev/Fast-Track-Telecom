#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <arpa/inet.h>
#include <unistd.h>

#define CLIENT_TAG "[CLIENT]: "

static int write_out(const void *buffer, size_t size, void *app_key) {
    FILE *out_fp = app_key;
    size_t wrote = fwrite(buffer, 1, size, out_fp);
    return (wrote == size) ? 0 : -1;
}

int main(int ac, char **av) {
    const char *SERVER_IP = "127.0.0.1";
    constexpr int SERVER_PORT = 12343;

    int clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
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
        if (!isRunning) {
            break;
        }

        std::string message = "";
        std::cout << CLIENT_TAG << "Введите сообщение: ";
        std::getline(std::cin, message);
        struct sctp_sndrcvinfo sndrcvinfo;
        if (sctp_sendmsg(clientSocket, message.data(), message.size(), (struct sockaddr *)&serverAddr, sizeof(serverAddr), 0, 0, 0, 0, 0) == -1) {
            std::cerr << CLIENT_TAG << "Ошибка отправки данных на сервер\n";
            isRunning = false;
        }

        std::cout << CLIENT_TAG << "Данные успешно отправлены на сервер!" << std::endl;

        char buffer[1024];
        struct sockaddr_in from;
        int flags = 0;
        int bytesReceived = sctp_recvmsg(clientSocket, buffer, sizeof(buffer), (struct sockaddr *)&from, 0, 0, &flags);
        if (bytesReceived == -1) {
            std::cerr << CLIENT_TAG << "Ошибка при получении данных от сервера\n";
            isRunning = false;
            break;
        } else if (bytesReceived == 0) {
            std::cout << CLIENT_TAG << "Сервер отключился\n";
            isRunning = false;
            break;
        }

        std::cout << std::string(buffer, bytesReceived) << std::endl;
    }
    close(clientSocket);

    return 0;
}
