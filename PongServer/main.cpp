
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <array>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netdb.h>

#define PORT 1153
#define BUFSIZE 2048

#include "ball.h"
#include "paddle.h"
#include "game.h"


int main(void)
{
    
    float windowHeight = 800.0f;
    float windowWidth = 1300.0f;

    Game game(windowHeight, windowWidth);
    Paddle paddle1(50.0f, (windowHeight/2.0f) - 70.0f, true);
    Paddle paddle2(windowWidth - 70.0f, (windowHeight/2.0f) - 70.0f, false);
    Ball ball;
    
    struct sockaddr_in myaddr;      /* our address */
    struct sockaddr_in remaddr;     /* remote address */
    socklen_t addrlen = sizeof(remaddr);            /* length of addresses */
    long recvlen;                    /* # bytes received */
    int fd;                         /* our socket */
    float buf[BUFSIZE];     /* receive buffer */

    /* create a UDP socket */

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            perror("cannot create socket\n");
            return 1;
    }

    /* bind the socket to any valid IP address and a specific port */

    memset((char *)&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port = htons(PORT);

    if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
            perror("bind failed");
            return 1;
    }

    /* main loop */
    for (;;)
    {
        game.OnUpdate(paddle1, paddle2, ball);
        printf("ball X - %f\n", ball.Xposition);
        printf("ball Y - %f\n", ball.Yposition);
        printf("countdown - %f\n", game.countDownToStart);

        
        printf("waiting on port %d\n", PORT);
        recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
        printf("received %ld bytes\n", recvlen);
        if (recvlen > 0) {
                buf[recvlen] = 0;
            printf("%f\n", buf[0]);
        }
        if (sendto(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, addrlen) < 0)
        perror("sendto");
    }
}
