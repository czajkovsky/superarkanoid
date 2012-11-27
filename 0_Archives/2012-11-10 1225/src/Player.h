#ifndef PLAYER_H
#define PLAYER_H

#include "Ball.h"
#include "Platform.h"
#include "geo.h"
#include <vector>

class Player : public Platform{
	public:
		Player(int lives, rectangle r);
		int getLives();
		void addLives(int k);
		bool reduceBalls();
		void addBalls(int k);
		bool reduceLives(); //zwraca true jeśli nie zostało żadne życie
		std::vector<Ball> balls;
		int getBallsAlive();
		void addBall(Ball ball);
	private:
		int lives;
		int power;
		int balls_alive;
		rectangle plat;
};

#endif
