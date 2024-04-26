#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <Registration.h>

#define SERVER_TAG "[SERVER]: "

void deserialize(int ac, char **av, Registration_t *registration);
void handleClient(int ac, char **av, int clientSocket, const sockaddr_in& clientAddr);

int main(int ac, char **av) {
    constexpr int PORT = 12343;

    int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if (serverSocket == -1) {
        //std::cerr << SERVER_TAG << "Ошибка создания сокета\n";
        return 1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        //std::cerr << SERVER_TAG << "Ошибка привязки сокета\n";
        close(serverSocket);
        return 1;
    }

    if (listen(serverSocket, 10) == -1) {
        //std::cerr << SERVER_TAG << "Ошибка прослушивания сокета\n";
        close(serverSocket);
        return 1;
    }

    //std::cout << SERVER_TAG << "Сервер запущен. Ожидание клиентов...\n";

    while (true) {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (clientSocket == -1) {
            //std::cerr << SERVER_TAG << "Ошибка при принятии соединения\n";
            continue;
        }

        //std::cout << SERVER_TAG << "Подключился клиент с IP: [" << inet_ntoa(clientAddr.sin_addr) << "], порт: [" << ntohs(clientAddr.sin_port) << "]" << std::endl;

        std::thread clientThread(handleClient, ac, av, clientSocket, std::ref(clientAddr));
        clientThread.detach();
    }

    close(serverSocket);

    return 0;
}

void handleClient(int ac, char **av, int clientSocket, const sockaddr_in& clientAddr) {
    while (true) {
        char buffer[1024];
        Registration_t registration;
        struct sockaddr_in from;
        int flags = 0;
        struct sctp_sndrcvinfo sndrcvinfo;
        int bytesReceived = sctp_recvmsg(clientSocket, &registration, sizeof(Registration_t), (struct sockaddr*)&from, 0, &sndrcvinfo, &flags);
        if (bytesReceived == -1) {
            //std::cerr << SERVER_TAG << "Ошибка при приеме данных\n";
            close(clientSocket);
            break;
        }
        deserialize(ac, av, &registration);

        if (bytesReceived == 0) {
            //std::cout << SERVER_TAG << "Клиент отключился [" << inet_ntoa(clientAddr.sin_addr) << "]:[" << ntohs(clientAddr.sin_port) << "]";
            close(clientSocket);
            break;
        }

        //std::cout << SERVER_TAG << "Получено сообщение от клиента [" << inet_ntoa(clientAddr.sin_addr) << "]:[" << ntohs(clientAddr.sin_port) << "]: " << std::string(buffer, bytesReceived) << std::endl;

        std::string connectionLostMessage = std::string(SERVER_TAG) + "Соединение потеряно!";
        if (std::string(buffer, bytesReceived) == "/close") {
            if (sctp_sendmsg(clientSocket, connectionLostMessage.data(), connectionLostMessage.size(), (struct sockaddr*)&from, 0, 0, 0, 0, 0, 0) == -1) {
                //std::cerr << SERVER_TAG << "Ошибка при отправке данных\n";
                close(clientSocket);
                break;
            }
            //std::cout << SERVER_TAG << "Соединение с клиентом [" << inet_ntoa(clientAddr.sin_addr) << "][" << ntohs(clientAddr.sin_port) << "] потеряно." << std::endl;
            close(clientSocket);
            break;
        }

        std::string answerFromServer = std::string(SERVER_TAG) + "Данные получены!";
        if (sctp_sendmsg(clientSocket, answerFromServer.data(), answerFromServer.size(), (struct sockaddr*)&from, 0, 0, 0, 0, 0, 0) == -1) {
            //std::cerr << SERVER_TAG << "Ошибка при отправке данных\n";
            close(clientSocket);
            break;
        }
    }
}

void deserialize(int ac, char **av, Registration_t *registration) {
    char buf[1024];
    asn_dec_rval_t rval;
    FILE *fp;
    size_t size;
    char *filename;

    if (ac != 2) {
        fprintf(stderr, "Usage: %s <file.ber>\n", av[0]);
        exit(1);
    } else {
        filename = av[1];
    }

    fp = fopen(filename, "rb");
    if (!fp) {
        perror(filename);
        exit(1);
    }

    size = fread(buf, 1, sizeof(buf), fp);
    fclose(fp);
    if (!size) {
        fprintf(stderr, "%s: Empty or broken\n", filename);
        exit(1);
    }

    rval = ber_decode(0, &asn_DEF_Registration, (void **)&registration, buf, size);
    if (rval.code != RC_OK) {
        fprintf(stderr, "%s: Broken Rectangle encoding at byte %ld\n", filename,
                (long)rval.consumed);
        exit(1);
    }

    xer_fprint(stdout, &asn_DEF_Registration, registration);
}
