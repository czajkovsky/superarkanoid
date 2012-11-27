#ifndef PLAYER_H
#define PLAYER_H

#include "Ball.h"
#include "Platform.h"
#include "geo.h"
#include <vector>
using namespace std;

class Player : public Platform{
	public:
		Player(const int &lives, const int &level, const rectangle &r);
		int getLives();
		void addLives(const int &k);
		void moreBalls(const int &k);
		bool reduceBalls();
		void addBalls(const int &k);		
		vector<Ball> balls;
		int getBallsAlive();
		void addBall(Ball &ball);
		void removeBalls();
		void setLevel(const int &level);
		int getLevel();
		void setLives(const int &lives);
		bool reduceLives(); //zwraca true jeśli nie zostało żadne życie
		int getPoints();
		void setPoints(const int &points);
		void addPoints(const int &points);
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
