
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
#include <sys/timeb.h>


#define PORT 1153
#define BUFSIZE 128

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
    timeb tb;
    ftime(&tb);
    int beginInterval = tb.millitm + (tb.time & 0xfffff) * 1000;

    /* main loop */
    for (;;)
    {
        
        // Wait for Client
        printf("waiting on port %d\n", PORT);
        recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
        
        if (hostIP1 == 0.0f && buf[1] == 1000.0f)
        {
            hostIP1 = buf[0];
            printf("ADDRESS ONE ASSIGNED!!\n");
            
            buf[0] = 0.0f;

            if (sendto(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, addrlen) < 0)
            perror("sendto");

        }
        else if (hostIP2 == 0.0f && buf[0] != hostIP1 && buf[1] == 1000.0f)
        {
            hostIP2 = buf[0];
            printf("ADDRESS TWO ASSIGNED!!\n");
            
            buf[0] = 1.0f;
            
            if (sendto(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, addrlen) < 0)
            perror("sendto");

        }
        else if (hostIP2 == 0.0f && hostIP1 != 0.0f && hostIP1 == buf[0])
        {
            buf[0] = 0;  // Waiting for opponent

            if (sendto(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, addrlen) < 0)
            perror("sendto");
        
        } else if (hostIP1 != 0.0f && hostIP2 != 0.0f)
        {
            if (buf[0] == hostIP1 && buf[1] == 2000.0f)
                game.p1paused = false;
            
            if (buf[0] == hostIP2 && buf[1] == 2000.0f)
                game.p2paused = false;
            
            if ((hostIP1 == buf[0] && game.p1paused == false && game.p2paused == true) ||
                (hostIP2 == buf[0] && game.p2paused == false && game.p1paused == true))
            {
                buf[0] = 0;
            }
            else if (!game.p1paused && !game.p2paused)
            {
                if (buf[1] != 1000.0f && hostIP1 == buf[0])
                {
                    paddle1.Yposition = buf[1];
                }
                else if (buf[1] != 1000.0f && hostIP2 == buf[0])
                {
                    paddle2.Yposition = buf[1];
                }

                // Update game state
                
                timeb tb;
                ftime(&tb);
                int current = tb.millitm + (tb.time & 0xfffff) * 1000;
                
                if (current - beginInterval > 10)
                {
                    beginInterval = current;
                    game.OnUpdate(paddle1, paddle2, ball);
                }

                if (buf[1] != 1000.0f)
                {
                    
                    if (buf[0] == hostIP2)
                    {
                        printf("Updating Player 2 State ---------------\n");
                        buf[3] = paddle1.Yposition;
                    }
                    else if (buf[0] == hostIP1)
                    {
                        printf("Updating Player 1 State ---------------\n");
                        buf[3] = paddle2.Yposition;
                    }
                    
                    if ((hostIP1 == buf[0] && game.p1paused) ||
                        (hostIP2 == buf[0] && game.p2paused))
                    {
                        buf[10] = 1.0f;
                    } else {
                        buf[10] = 0.0f;
                    }
                    
                    buf[0] = 2.0f;
                    buf[1] = game.player1Score;
                    buf[2] = game.player2Score;
                    buf[4] = ball.Xposition;
                    buf[5] = ball.Yposition;
                    buf[6] = ball.speed;
                    buf[7] = ball.direction;
                    buf[8] = game.messageNum;
                    buf[9] = game.playing;
                }
            } else if ((hostIP1 == buf[0] && game.p1paused) ||
                (hostIP2 == buf[0] && game.p2paused))
            {
                buf[0] = 3.0f;
            }

            if (sendto(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, addrlen) < 0)
            perror("sendto");
        }
    }
}
