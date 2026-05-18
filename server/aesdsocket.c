
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <syslog.h>
#include <fcntl.h>
#include <errno.h>

#define PORT 9000
#define DATAFILE "/var/tmp/aesdsocketdata"

static int server_fd = -1;
static int client_fd = -1;
static volatile sig_atomic_t exit_flag = 0;

void signal_handler(int sig)
{
    syslog(LOG_INFO, "Caught signal, exiting");
    exit_flag = 1;

    if (server_fd != -1)
        close(server_fd);
    if (client_fd != -1)
        close(client_fd);

    remove(DATAFILE);
}

void daemonize()
{
    pid_t pid = fork();
    if (pid < 0)
        exit(EXIT_FAILURE);

    if (pid > 0)
        exit(EXIT_SUCCESS);

    if (setsid() < 0)
        exit(EXIT_FAILURE);

    pid = fork();
    if (pid < 0)
        exit(EXIT_FAILURE);

    if (pid > 0)
        exit(EXIT_SUCCESS);

    umask(0);
    chdir("/");

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

int main(int argc, char *argv[])
{
    int daemon_mode = 0;

    if (argc == 2 && strcmp(argv[1], "-d") == 0)
        daemon_mode = 1;

    openlog("aesdsocket", LOG_PID, LOG_USER);

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        syslog(LOG_ERR, "socket failed: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        syslog(LOG_ERR, "bind failed: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (daemon_mode)
        daemonize();

    if (listen(server_fd, 5) < 0) {
        syslog(LOG_ERR, "listen failed: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    while (!exit_flag) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);

        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
        if (client_fd < 0) {
            if (exit_flag) break;
            syslog(LOG_ERR, "accept failed: %s", strerror(errno));
            continue;
        }

        syslog(LOG_INFO, "Accepted connection from %s", inet_ntoa(client_addr.sin_addr));

        FILE *fp = fopen(DATAFILE, "a+");
        if (!fp) {
            syslog(LOG_ERR, "fopen failed: %s", strerror(errno));
            close(client_fd);
            continue;
        }

        char buf[1024];
        ssize_t bytes;
        int newline_found = 0;

        while (!newline_found && (bytes = recv(client_fd, buf, sizeof(buf), 0)) > 0) {
            fwrite(buf, 1, bytes, fp);
            if (memchr(buf, '\n', bytes))
                newline_found = 1;
        }

        fflush(fp);

        fseek(fp, 0, SEEK_SET);
        while ((bytes = fread(buf, 1, sizeof(buf), fp)) > 0) {
            send(client_fd, buf, bytes, 0);
        }

        fclose(fp);
        close(client_fd);
        client_fd = -1;
    }

    if (server_fd != -1)
        close(server_fd);

    remove(DATAFILE);
    closelog();

    return 0;
}
