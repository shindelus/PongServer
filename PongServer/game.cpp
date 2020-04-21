#include <iostream>
#include <math.h>
#include "game.h"

void Game::CheckForLeftCollision(Paddle& p, Ball& b)
{
    bool ballWithinVertWindow = b.Yposition + b.height > p.Yposition && b.Yposition < p.Yposition + p.height;
    bool ballWithinHorizWindow = b.Xposition + b.width < p.Xposition && b.Xposition + b.speed + b.width > p.Xposition;
    if (ballWithinVertWindow && ballWithinHorizWindow && b.Xspeed > 0)
    {
            
        if (p.isMovingUp && !curve)
        {
            curve = true;
            clockWiseCurve = false;
            curveCountdown = 2;
            
        } else if (p.isMovingDown && !curve)
        {
            curve = true;
            clockWiseCurve = true;
            curveCountdown = 2;
        }
        b.Xposition = p.Xposition - b.width;
        b.Xspeed = - b.Xspeed;
        clockWiseCurve = !clockWiseCurve;
    
    }
};

void Game::CheckForRightCollision(Paddle& p, Ball& b)
{
    bool ballWithinVertWindow = b.Yposition + b.height > p.Yposition && b.Yposition < p.Yposition + p.height;
    bool ballWithinHorizWindow = b.Xposition > p.Xposition + p.width && b.Xposition + b.Xspeed < p.Xposition + p.width;
    if (ballWithinVertWindow && ballWithinHorizWindow && b.Xspeed < 0)
    {
        b.Xposition = p.Xposition + p.width;
        b.Xspeed = -b.Xspeed;
        if (curve)
        {
            clockWiseCurve = !clockWiseCurve;
        }

    }


};

void Game::CheckForTopCollision(Paddle& p, Ball& b)
{
    bool ballWithinVertWindow = b.Yposition > p.Yposition + p.height && b.Yposition + b.speed < p.Yposition + p.height;
    bool ballWithinHorizWindow = b.Xposition + b.width > p.Xposition && b.Xposition < p.Xposition + p.width;
    if (ballWithinVertWindow && ballWithinHorizWindow && b.Yspeed < 0)
    {
        b.Yposition = p.Yposition + p.height;
        b.Yspeed = -b.Yspeed;
    }
};

void Game::CheckForBottomCollision(Paddle& p, Ball& b)
{
    bool ballWithinVertWindow = b.Yposition + b.height < p.Yposition && b.Yposition + b.speed + b.height > p.Yposition;
    bool ballWithinHorizWindow = b.Xposition + b.width > p.Xposition && b.Xposition < p.Xposition + p.width;
    if (ballWithinVertWindow && ballWithinHorizWindow && b.Yspeed > 0)
    {
        b.Yposition = p.Yposition - b.height;
        b.Yspeed = -b.Yspeed;
    }
};

void Game::CheckForCollisions(Paddle& p1, Paddle& p2, Ball& b)
{
    CheckForTopCollision(p1, b);
    CheckForTopCollision(p2, b);
    CheckForLeftCollision(p2, b);
    CheckForRightCollision(p1, b);
};

bool Game::BallIsOutRight(Ball& b)
{
    if (b.Xposition + b.width + b.Xspeed > windowWidth)
    {
        return true;
    }
    return false;
};

bool Game::BallIsOutLeft(Ball& b)
{
    if (b.Xposition + b.Xspeed < 0.0f)
    {
        return true;
    }
    return false;
};

void Game::CheckForBallYBounds(Ball& b)
{
    if (b.Yposition + b.height + b.Yspeed > windowHeight - 60.0f && b.Yspeed > 0)
    {
        b.Yposition = windowHeight - b.height - 60.0f;
        b.Yspeed = - b.Yspeed;
        b.direction = 26 - b.direction;

    } else if (b.Yposition + b.Yspeed < 0.0f && b.Yspeed < 0)
    {
        b.Yposition = 0.0f;
        b.Yspeed = - b.Yspeed;
        b.direction = 26 - b.direction;

    }
};

bool Game::isPaddleAtTop(Paddle& p)
{
    if (p.Yposition + p.height + p.Yspeed > windowHeight - 60.0f)
    {
        p.Yposition = windowHeight - p.height - 60.0f;
        return true;
    }
    return false;
};

bool Game::isPaddleAtBottom(Paddle& p)
{
    if (p.Yposition - p.Yspeed < 0.0f)
    {
        p.Yposition = 0.0f;
        return true;
    }
    return false;
};

void Game::MovePaddleUp(Paddle& p)
{
    if (!isPaddleAtTop(p))
    {
        p.Yposition = p.Yposition + p.Yspeed;
    }
};

void Game::MovePaddleDown(Paddle& p)
{
    if (!isPaddleAtBottom(p))
    {
        p.Yposition = p.Yposition - p.Yspeed;
    }
};

void Game::CheckBallDirection(Ball& b)
{
    
    if (curve && curveCountdown > 0)
    {
        curveCountdown--;
        return;
    }
    
    if (curve && curveCountdown == 0)
    {
        
        if (b.Xspeed > 0 && clockWiseCurve && b.direction < 26)
        {
            b.direction++;
            
        } else if (b.Xspeed > 0 && !clockWiseCurve && b.direction > 0)
        {
            b.direction--;
            
        } else if (b.Xspeed < 0 && clockWiseCurve && b.direction < 26)
        {
            b.direction++;
            
        } else if (b.Xspeed < 0 && !clockWiseCurve && b.direction > 0)
        {
            b.direction--;
        }
                
        if (b.direction > 13)
        {
            b.Yspeed = - b.speed * b.Yangle[b.direction];
            
        } else {
            b.Yspeed = b.speed * b.Yangle[b.direction];
        }
        
        
        
        if (b.Xspeed > 0 && b.speed > 0)
        {
            b.Xspeed = b.speed * b.Xangle[b.direction];
            
        } else if (b.speed > 0)
        {
            b.Xspeed = - b.speed * b.Xangle[b.direction];
            
        } else if (b.speed < 0 && b.speed < 0)
        {
            b.Xspeed = b.speed * b.Xangle[b.direction];
            
        } else {
            
            b.Xspeed = - b.speed * b.Xangle[b.direction];
        }
        
        curveCountdown = 6;
    }
};

void Game::MoveBall(Paddle& p1, Paddle& p2, Ball& b)
{
    CheckBallDirection(b);
    CheckForBallYBounds(b);
        
    if (BallIsOutLeft(b))
    {
        player2Score++;
        p2Scored = true;
        if (player2Score == 7)
        {
            level++;
            player1Score = 0;
            player2Score = 0;
            levelUp = true;
            b.speed = b.speed + 2.0f;
            player2Score = 0;
        }
        ResetLevel(p1, p2, b);
        
    } else if (BallIsOutRight(b))
    {
        player1Score++;
        p1Scored = true;
        if (player1Score == 7)
        {
            level = 1;
            lost = true;
            ResetGame(p1, p2, b);
        } else {
            ResetLevel(p1, p2, b);
        }
    } else {
        b.Xposition = b.Xposition + b.Xspeed;
        b.Yposition = b.Yposition + b.Yspeed;
    }
};

void Game::ResetLevel(Paddle& p1, Paddle& p2, Ball& b)
{
    p1.Yposition = (windowHeight/2.0f) - 70.0f;
    p2.Yposition = (windowHeight/2.0f) - 70.0f;
    b.Xposition = 100.0f;
    b.Yposition = 387.5f;
    b.direction = 10;
    b.Xspeed = b.speed * b.Xangle[b.direction];
    b.Yspeed = b.speed * b.Yangle[b.direction];
    curve = false;
    clockWiseCurve = false;
};

void Game::ResetGame(Paddle& p1, Paddle& p2, Ball& b)
{
    player1Score = 0;
    player2Score = 0;
    level = 1;
    ResetLevel(p1, p2, b);
    countDownToStart = 200.0f;
};

void Game::CheckPaddleMovement(Paddle& p)
{
    if (p.isMovingUp)
    {
        MovePaddleUp(p);
    } else if (p.isMovingDown)
    {
        MovePaddleDown(p);
    }
}

void Game::OnUpdate(Paddle& p1, Paddle& p2, Ball& b)
{
    CheckPaddleMovement(p2);
    if (countDownToStart != 0)
    {
        countDownToStart--;
    } else {
        CheckForCollisions(p1, p2, b);
        MoveBall(p1, p2, b);
    }
};

Game::Game(float& wH, float& wW)
{
    player1Score = 0;
    player2Score = 0;
    level = 1;
    windowHeight = wH;
    windowWidth = wW;
    countDownToStart = 200.0f;
    curve = false;
    clockWiseCurve = false;
    levelUp = false;
    lost = false;
    p1Scored = false;
    p2Scored = false;
}
