#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <Registration.h>

#define CLIENT_TAG "[CLIENT]: "

static int write_out(const void *buffer, size_t size, void *app_key);
void coder(int ac, char **av, Registration* registration);

int main(int ac, char **av) {
    const char *SERVER_IP = "127.0.0.1";
    constexpr int SERVER_PORT = 12343;

    Registration_t *registration = (Registration_t *)malloc(sizeof(Registration_t));
    if (!registration) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    OCTET_STRING_fromBuf(&registration->firstName, "Jack", std::strlen("Jack"));
    OCTET_STRING_fromBuf(&registration->lastName, "Jackson", std::strlen("Jackson"));
    OCTET_STRING_fromBuf(&registration->email, "jack@gmail.com", std::strlen("jack@gmail.com"));
    OCTET_STRING_fromBuf(&registration->phone, "78562541211", std::strlen("78562541211"));

    coder(ac, av, registration);

    int clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if (clientSocket == -1) {
        return 1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        close(clientSocket);
        return 1;
    }

    printf("%s%s\n", CLIENT_TAG, "Подключение к серверу выполнено успешно.\n");

    bool isRunning = true;
    while (isRunning) {
        if (!isRunning) {
            break;
        }

        struct sctp_sndrcvinfo sndrcvinfo;
        if (sctp_sendmsg(clientSocket, &registration, sizeof(Registration_t), (struct sockaddr *)&serverAddr, sizeof(serverAddr), 0, 0, 0, 0, 0) == -1) {
            isRunning = false;
        }

        printf("%s%s\n", CLIENT_TAG, "Данные успешно отправлены на сервер!\n");

        char buffer[1024];
        struct sockaddr_in from;
        int flags = 0;
        int bytesReceived = sctp_recvmsg(clientSocket, buffer, sizeof(buffer), (struct sockaddr *)&from, 0, 0, &flags);
        if (bytesReceived == -1) {
            isRunning = false;
            break;
        } else if (bytesReceived == 0) {
            isRunning = false;
            break;
        }
    }
    close(clientSocket);
    free(registration);
    return 0;
}

static int write_out(const void *buffer, size_t size, void *app_key) {
    FILE *out_fp = (FILE*)app_key;
    size_t wrote = fwrite(buffer, 1, size, out_fp);
    return (wrote == size) ? 0 : -1;
}

void coder(int ac, char **av, Registration* registration) {
    asn_enc_rval_t ec;

    if (ac < 2) {
        fprintf(stderr, "Specify filename for BER output\n");
    } else {
        const char *filename = av[1];
        FILE *fp = fopen(filename, "wb");
        if (!fp) {
            perror(filename);
            exit(1);
        }

        ec = der_encode(&asn_DEF_Registration, registration, write_out, fp);
        fclose(fp);
        if (ec.encoded == -1) {
            fprintf(stderr, "Could not encode registration(at % s)\n",
                    ec.failed_type ? ec.failed_type->name : "unknown");
            exit(1);
        } else {
            fprintf(stderr, "Created % s with BER encoded registration\n", filename);
        }
    }
    xer_fprint(stdout, &asn_DEF_Registration, registration);
}