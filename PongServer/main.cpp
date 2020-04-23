
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
#define BUFSIZE 40

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
    
    
    float hostIP1 = 0.0f;
    float hostIP2 = 0.0f;
    
//    float buf2[BUFSIZE];     /* receive buffer */

    /* main loop */
    for (;;)
    {
        // Wait for Client
        printf("waiting on port %d\n", PORT);
        recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
        
        if (hostIP1 == 0.0f && buf[0] > 9.0f && buf[1] == 0)
        {
            hostIP1 = buf[0];
            printf("ADDRESS ONE ASSIGNED!!\n");
            buf[0] = 3.0f;

            if (sendto(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, addrlen) < 0)
            perror("sendto");

        }
        else if (hostIP2 == 0.0f && buf[0] != hostIP1 && buf[0] > 9.0f)
        {
            hostIP2 = buf[0];
            printf("ADDRESS TWO ASSIGNED!!\n");
            buf[0] = 4.0f;  // Waiting for opponent


            if (sendto(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, addrlen) < 0)
            perror("sendto");

        }
        else if (hostIP2 == 0.0f && hostIP1 != 0.0f)
        {
            buf[0] = 0;  // Waiting for opponent

            if (sendto(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, addrlen) < 0)
            perror("sendto");
        } else if (hostIP1 != 0.0f && hostIP2 != 0.0f)
        {
            printf("Play Game!!\n");
            
            if (buf[1] == 1.0f && hostIP1 == buf[0])
                paddle1.Yposition = buf[2];
            else if (buf[1] == 1.0f && hostIP2 == buf[0])
                paddle2.Yposition = buf[2];

            // Update game state
            game.OnUpdate(paddle1, paddle2, ball);

            if (buf[1] == 1.0f)
            {
                buf[0] = 1.0f;
                buf[1] = ball.Xposition;
                buf[2] = ball.Yposition;
                buf[3] = paddle1.Yposition;
                buf[4] = paddle2.Yposition;
            } else if (buf[1] == 2.0f)
            {
                buf[0] = 2.0f;
                buf[1] = game.player1Score;
                buf[2] = game.player2Score;
                buf[3] = game.messageNum;
                game.needLevelScoreUpdate = false;
            }
            
            if (sendto(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, addrlen) < 0)
            perror("sendto");
        }
    }
}
