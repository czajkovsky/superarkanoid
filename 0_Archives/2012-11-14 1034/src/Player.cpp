#include "Player.h"
#include <vector>
using namespace std;

typedef vector<Ball> vba;

Player::Player(int lives, int level, rectangle r) : Platform(r) {
	this->lives = this->base_lives = lives;
	this->level = level;
	this->power = 1;
	this->balls_alive = 0;
	(this->balls).reserve(16);
}

int Player::getLives() {
	return this->lives;
}

void Player::moreBalls(int n) {
	this->balls_alive+=n;
}

void Player::addLives(int k) {
	this->lives += k;
}

bool Player::reduceBalls() {
	if (--(this->balls_alive) < 1) return true;
	else return false;
}

void Player::addBalls(int k) {
	this->balls_alive += k;
}

bool Player::reduceLives() {
	if (--lives < 1) return true;
	else return false;
}

int Player::getBallsAlive() {
	return this->balls_alive;
}

void Player::addBall(Ball ball) {
	(this->balls).push_back(ball);
}

void Player::removeBalls() {
	this->balls_alive = 0;
	(this->balls).clear();
}

void Player::setLevel(int level) {
	this->level = level;
}

int Player::getLevel() {
	return this->level;
}

void Player::setLives(int lives) {
	this->lives = lives;
}

void Player::setPoints(int points) {
	this->points = points;
}

int Player::getPoints() {
	return this->points;
}

void Player::addPoints(int points) {
	this->points += points;
}

void Player::clear() {
	this->points = 0;
	this->lives = base_lives;
	removeBalls();
}

