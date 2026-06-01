#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>

#define PORT 9000
#define FILE_PATH "/var/tmp/aesdsocketdata"

int main(void)
{
    int server_fd, client_fd;
    struct sockaddr_in serv_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    char buffer[1024];
    ssize_t bytes_read;

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Allow reuse
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Bind
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, 5) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Accept connection
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("accept");
            continue;
        }

        // Open file for append
        FILE *fp = fopen(FILE_PATH, "a+");
        if (!fp) {
            perror("fopen");
            close(client_fd);
            continue;
        }

        // Read until newline EXACTLY ONCE
        size_t total = 0;
        memset(buffer, 0, sizeof(buffer));

        while ((bytes_read = recv(client_fd, buffer + total, sizeof(buffer) - total - 1, 0)) > 0) {
            total += bytes_read;
            if (memchr(buffer, '\n', total)) {
                break; // stop at newline
            }
        }

        // Write to file ONCE
        fwrite(buffer, 1, total, fp);
        fflush(fp);

        // Rewind and send entire file back ONCE
        fseek(fp, 0, SEEK_SET);
        while ((bytes_read = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
            send(client_fd, buffer, bytes_read, 0);
        }

        fclose(fp);
        close(client_fd);
    }

    close(server_fd);
    return 0;
}
