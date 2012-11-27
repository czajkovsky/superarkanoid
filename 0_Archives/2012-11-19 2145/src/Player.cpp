#include "Player.h"
#include <vector>
using namespace std;

typedef vector<Ball> vba;

Player::Player(const int &lives, const int &level, const rectangle &r) : Platform(r) {
	this->lives = this->base_lives = lives;
	this->level = level;
	this->power = 1;
	this->balls_alive = 0;
	this->points = 0;
	(this->balls).reserve(16);
}

int Player::getLives() {
	return this->lives;
}

void Player::moreBalls(const int &n) {
	this->balls_alive+=n;
}

void Player::addLives(const int &k) {
	this->lives += k;
}

bool Player::reduceBalls() {
	if (--(this->balls_alive) < 1) return true;
	else return false;
}

void Player::addBalls(const int &k) {
	this->balls_alive += k;
}

bool Player::reduceLives() {
	if (--lives < 1) return true;
	else return false;
}

int Player::getBallsAlive() {
	return this->balls_alive;
}

void Player::addBall(Ball &ball) {
	(this->balls).push_back(ball);
}

void Player::removeBalls() {
	this->balls_alive = 0;
	(this->balls).clear();
}

void Player::setLevel(const int &level) {
	this->level = level;
}

int Player::getLevel() {
	return this->level;
}

void Player::setLives(const int &lives) {
	this->lives = lives;
}

void Player::setPoints(const int &points) {
	this->points = points;
}

int Player::getPoints() {
	return this->points;
}

void Player::addPoints(const int &points) {
	this->points += points;
}

void Player::clear() {
	this->points = 0;
	this->lives = base_lives;
	removeBalls();
}

