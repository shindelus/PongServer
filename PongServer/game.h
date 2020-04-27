#include <iostream>
#include <string>
#include "ball.h"
#include "paddle.h"


class Game
{
public:
    int player1Score;
    int player2Score;
    int angleChangeCountdown;
    int curveCountdown;
    float windowHeight;
    float windowWidth;
    float countDownToStart;
    float messageNum;
    float speedUpCountdown;
    float playing;
    float countdownToPause;
    bool curve;
    bool clockWiseCurve;
    bool p1Lost;
    bool p2Lost;
    bool p1Scored;
    bool p2Scored;
    bool p1paused;
    bool p2paused;
    bool p1waiting;
    bool p2waiting;
    bool resetting;
    bool inProcessOfPause;
    
    Game(float& wH, float& wW);
    ~Game(){};
    
    bool BallIsOutLeft(Ball& b);
    bool BallIsOutRight(Ball& b);
    bool isPaddleAtTop(Paddle& p);
    bool isPaddleAtBottom(Paddle& p);
    void CheckForCollisions(Paddle& p1, Paddle& p2, Ball& b);
    void CheckForLeftCollision(Paddle& p, Ball& b);
    void CheckForRightCollision(Paddle& p, Ball& b);
    void CheckForTopCollision(Paddle& p, Ball& b);
    void CheckForBottomCollision(Paddle& p, Ball& b);
    void CheckForBallYBounds(Ball& b);
    void ResetLevel(Paddle& p1, Paddle& p2, Ball& b);
    void ResetGame(Paddle& p1, Paddle& p2, Ball& b);
    void OnUpdate(Paddle& p1, Paddle& p2, Ball& b);
    void MovePaddleUp(Paddle& p);
    void MovePaddleDown(Paddle& p);
    void MoveBall(Paddle& p1, Paddle& p2, Ball& b);
    void CheckBallDirection(Ball& b);
    void CheckPaddleMovement(Paddle& p);
    void CheckMessage();
};
