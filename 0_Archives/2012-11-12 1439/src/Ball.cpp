#include "Ball.h"
#include "Block.h"
#include "Platform.h"
#include "Border.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <stdlib.h>
using namespace std;

#define INF 1e9

typedef vector<Block> vbl;
typedef vector<Border> vbo;
typedef vector<Bonus> vbn;

Ball::Ball(int x, int y, int r, vec v, double speed, vbo::iterator it, vbl::iterator it2) {
	point p;
	p.x = x;
	p.y = y;
	circle c;
	c.center = p;
	c.r = r;
	this->cir = c;
	this->base_speed = speed;
	this->speed = speed;
	this->time_remaining = 0;
	this->mv = v;
	this->moze_odbic_sie_od_platformy = false;
	this->lastBorderReflection = it;
	this->lastBlockReflection = it2;
	srand(time(0));

	this->bon_bl.upperleft.x = 0;
	this->bon_bl.upperleft.y = 603;
	this->bon_bl.bottomright.x = 960;
	this->bon_bl.bottomright.y = 593;
}

void Ball::change_speed(double speed) {
	this->speed = speed;
}

void Ball::change_pos(point p) {
	this->cir.center = p;
}

void Ball::print() {
	//cout<<"Ball: "<<this->cir.center.x<<" "<<this->cir.center.y<<"\n";
}

point Ball::collision(rectangle rec) {
	return intersects(cir, rec);
}

void Ball::reflect(point w) {
	vec rrr;
	rrr.x = w.x - cir.center.x;
	rrr.y = w.y - cir.center.y;
	
	this->mv = reflection(mv, rrr);
	
}

const double wsp = 54.0;
const double wsp2 = 16.0;
const double dif = 1.8;

void Ball::change_vec(double pl_speed, double dist) {
	vec nowy;
	double d = sqrt(mv.x*mv.x + mv.y*mv.y);
	nowy.x = mv.x - pl_speed * wsp + dist/wsp2;

	if (nowy.x >= d-dif) nowy.x = d-dif;//max(mv.x, d-dif);
	else if (nowy.x <= dif-d) nowy.x = dif-d;//min(mv.x, dif-d);

	if (mv.y >= 0) nowy.y = sqrt(d*d - nowy.x*nowy.x);
	else nowy.y = -sqrt(d*d - nowy.x*nowy.x);

	mv = nowy;
	//cout<<mv.x<<" "<<mv.y<<endl;
}

//co zwraca opisane jest w Ball.h
int Ball::move(vbl &blocks, vbo &borders, vbn &bonuses, Platform plat, int blocksuntouched[]) {
	bool col = false;
	bool usun = false;

	rectangle rr;
	point w;

	int cc = 0;

	//zderzenie z obramowaniem
	for (vbo::iterator it = borders.begin(); it != borders.end()/* && !col*/ && !usun; it++) {
		rr = (*it).return_rectangle();
		w = collision(rr);
		if (w.x != INF && lastBorderReflection != it) {			
			if (!(*it).isMurderous()) {
				col = true;
				cc++;
				reflect(w);
				this->moze_odbic_sie_od_platformy = true;
			}
			else {
				usun = true;
				break;
			}
			change_vec(0.0, 0.0);
			lastBorderReflection = it;
			lastBlockReflection = blocks.end();
		}
	}

	bool chanceforbonus=false; //zmienna spamiętująca czy usunięto klocek
	point bonuscords;


	//zderzenie z blokiem
	for (vbl::iterator it = blocks.begin(); it != blocks.end()/* && !col*/ && !usun; it++) {
		rr = (*it).return_rectangle();
		w = collision(rr);
		if (w.x != INF && lastBlockReflection != it) {
			lastBlockReflection = it;
			lastBorderReflection = borders.end();
			col = true;
			cc++;
			reflect(w);
			if ((*it).get_lives()==(*it).get_maxlives()) {
				blocksuntouched[(*it).get_type()]--;
				int yellowblock=3;
	
				if(blocksuntouched[yellowblock]==0) {
					for (vbl::iterator it = blocks.begin(); it != blocks.end(); it++)	{	//to nie powinno być w części rysowania!
						if((*it).get_type()==yellowblock) {
							it--;
							blocks.erase(it+1);
						}
					}
					blocksuntouched[yellowblock]=-1;
				}
			}
			if ((*it).hit(1)) { // uderzenie z siłą 1 - do zmiany !!!
				
				chanceforbonus=true;
				bonuscords.x=((*it).return_rectangle().bottomright.x+(*it).return_rectangle().upperleft.x)/2;
				bonuscords.y=(*it).return_rectangle().bottomright.y;

				it--;
				blocks.erase(it+1);
								
			}
			lastBlockReflection = blocks.end();
				
			
			//tworzenie bonusu

			
			int bonustype=rand()%3;
			bonustype++;
			if(rand()%3==0&&chanceforbonus) chanceforbonus=true;
			else chanceforbonus=false;

			if(chanceforbonus) {
				circle cir;
				//cir.r=16;
				cir.center=bonuscords;
				Bonus bonus(cir, bonustype); 
				bonuses.push_back(bonus);
			}
			this->moze_odbic_sie_od_platformy = true;
			break;
		}
		else if (w.x != INF && lastBlockReflection == it) cout<<"err 2*lBR\n";
	}

	
	
	

	

	bool plat_col = false;
	//zderzenie z platformą będzie tu ↓
//	if (this->moze_odbic_sie_od_platformy == true) {
		rr = plat.return_rectangle();
		w = collision(rr);
		if (w.x != INF) {
			plat_col = true;
			cc++;
			if (this->moze_odbic_sie_od_platformy) {
				double dist = (w.x-(rr.upperleft.x + (rr.bottomright.x-rr.upperleft.x)/2));
				reflect(w);
				change_vec(plat.getSpeed(), dist); //sprawdź czy nie krawędź
				lastBorderReflection = borders.end();
				lastBlockReflection = blocks.end();
			}
			this->moze_odbic_sie_od_platformy = false;
		}
//	}


	if (!plat_col && blocksuntouched[0]) {
		w=collision(bon_bl);
		if (w.x != INF) {
			cc++;
			reflect(w);
			lastBorderReflection = borders.end();
			lastBlockReflection = blocks.end();
			this->moze_odbic_sie_od_platformy = false;
			change_vec(0.0, 0.0);
			blocksuntouched[0]=0;
		}
	}
	
	//if (cc > 1) cout<<"errrrr\n";
	if (cc > 1 && plat_col) usun = true;
	
	(this->cir).center.x += speed*mv.x;
	(this->cir).center.y += speed*mv.y;	

	if (usun && !plat_col) return 1;
	else if (usun && plat_col) return 2;
	else if (plat_col) return 3;
	else return 4;
}

point Ball::middle() {
	return this->cir.center;
}

double Ball::r() {
	return this->cir.r;
}
