#ifndef PLAYER_H
#define PLAYER_H

#include "Ball.h"
#include "Platform.h"
#include "geo.h"
#include <vector>

class Player : public Platform{
	public:
		Player(int lives, int level, rectangle r);
		int getLives();
		void addLives(int k);
		void moreBalls(int k);
		bool reduceBalls();
		void addBalls(int k);		
		std::vector<Ball> balls;
		int getBallsAlive();
		void addBall(Ball ball);
		void removeBalls();
		void setLevel(int level);
		int getLevel();
		void setLives(int lives);
		bool reduceLives(); //zwraca true jeśli nie zostało żadne życie
		int getPoints();
		void setPoints(int points);
		void addPoints(int points);
		void clear();
	private:
		int level;
		int points;
		int lives, base_lives;
		int power;
		int balls_alive;
		rectangle plat;
};

#endif
