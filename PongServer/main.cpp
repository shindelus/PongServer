
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
    
    
    struct sockaddr * addr1 = nullptr;
    struct sockaddr * addr2 = nullptr;
    
    float buf2[BUFSIZE];     /* receive buffer */

    /* main loop */
    for (;;)
    {
        // Wait for Client
        printf("waiting on port %d\n", PORT);
        recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
//        printf("received %ld bytes\n", recvlen);
        
        if (addr1 == nullptr)
        {
            addr1 = (struct sockaddr *)&remaddr;
            printf("ADDRESS ONE ASSIGNED!!");
        }
        else if (addr2 == nullptr)
        {
            addr2 = (struct sockaddr *)&remaddr;
            printf("ADDRESS TWO ASSIGNED!!");
        }
        
//        if (addr1 != nullptr && addr2 != nullptr)
        if (true)
        {
            if (recvlen > 0) {
                buf[recvlen] = 0;
    //            printf("%f\n", buf[0]);
            }
            if (buf[0] == 1.0f)
                game.needLevelScoreUpdate = true;
            paddle1.Yposition = buf[1];
            paddle2.Yposition = buf[2];
                    
            // Update game state
            game.OnUpdate(paddle1, paddle2, ball);
            
            if (game.needLevelScoreUpdate)
            {
                buf[0] = 1;
                buf[1] = game.player1Score;
                buf[2] = game.player2Score;
                buf[3] = game.level;
                buf[4] = game.messageNum;
                game.needLevelScoreUpdate = false;
            } else {
                buf[0] = 0;
                buf[1] = ball.Xposition;
                buf[2] = ball.Yposition;
            }
            
            
//            buf2[0] = 2;
//            buf2[1] = paddle1.Yposition;
//            buf2[2] = paddle2.Yposition;
            
            // Send new state to clients
            
            if (addr1 == (struct sockaddr *)&remaddr)
            {
                if (sendto(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, addrlen) < 0)
                perror("sendto");
                
//                if (sendto(fd, buf2, BUFSIZE, 0, addr2, addrlen) < 0)
//                perror("sendto");
                
            } else if (addr2 == (struct sockaddr *)&remaddr)
            {
                if (sendto(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, addrlen) < 0)
                perror("sendto");
                
//                if (sendto(fd, buf2, BUFSIZE, 0, addr1, addrlen) < 0)
//                perror("sendto");
            }
        }
    }
}
