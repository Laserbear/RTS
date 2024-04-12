#include <stdio.h> 
#include <string.h>

#include <sys/socket.h>
#include <netdb.h>

static void server(int port) {
    //create socket
    const int fd = socket(PF_INET, SOCK_STREAM, 0);

    //bind to open port
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(fd, (struct sockaddr*) &addr, sizeof(addr))) {
	perror("bind error:");
        return;
    }

    socklen_t addr_len = sizeof(addr);
    getsockname(fd, (struct sockaddr*) &addr, &addr_len);
    printf("server is running on port %d\n", (int) ntohs(addr.sin_port));
    
    if (listen(fd, 1)) {
	perror("listen error:");
	return;
    }
    
    //accept client connection
    struct sockaddr_storage caddr;
    socklen_t caddr_len = sizeof(caddr);
    const int cfd = accept(fd, (struct sockaddr*) &caddr, &caddr_len);
    
    //read from recv
    char buf[1024];
    recv(cfd, buf, sizeof(buf), 0);

    //leap without looking!
    printf("Received: \n%s\n", buf);
    
    close(cfd);
    close(fd);
}

int main(int argc, char *argv[]) {
    printf("hello, gamers!\n");
    if(argc < 2) {
	fprintf(stderr, "Please specify port\n");
	return -1;
    }
    int port;
    sscanf(argv[1], "%d", &port);
    printf("starting on port %d\n", port);
    server(port);
    return 0;
}
