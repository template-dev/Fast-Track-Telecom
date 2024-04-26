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

void decode(int ac, char **av, Registration_t *registration);
void handleClient(int ac, char **av, int clientSocket, const sockaddr_in& clientAddr);

int main(int ac, char **av) {
    constexpr int PORT = 12343;

    int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if (serverSocket == -1) {
        return 1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        close(serverSocket);
        return 1;
    }

    if (listen(serverSocket, 10) == -1) {
        close(serverSocket);
        return 1;
    }

    while (true) {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (clientSocket == -1) {
            continue;
        }
        std::thread clientThread(handleClient, ac, av, clientSocket, std::ref(clientAddr));
        clientThread.detach();
    }
    close(serverSocket);
    return 0;
}

void handleClient(int ac, char **av, int clientSocket, const sockaddr_in& clientAddr) {
    while (true) {
        Registration_t *registration = nullptr;
        char buffer[1024];
        struct sockaddr_in from;
        int flags = 0;
        struct sctp_sndrcvinfo sndrcvinfo;

        int bytesReceived = sctp_recvmsg(clientSocket, buffer, sizeof(buffer), (struct sockaddr*)&from, 0, &sndrcvinfo, &flags);
        if (bytesReceived == -1) {
            close(clientSocket);
            break;
        }

        if (bytesReceived == 0) {
            close(clientSocket);
            break;
        }

        size_t registration_size = sizeof(Registration_t);
        registration = (Registration_t *)malloc(registration_size);
        if (!registration) {
            close(clientSocket);
            break;
        }
        
        decode(ac, av, registration);
        free(registration);

        if (bytesReceived == 0) {
            close(clientSocket);
            break;
        }
    }
}

void decode(int ac, char **av, Registration_t *registration) {
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
