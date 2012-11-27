#include "Level.h"
#include <vector>
#include <iostream>
using namespace std;

#define INF 1e9

typedef vector<Block> vbl;
typedef vector<Border> vbo;
typedef vector<Bonus> vbn;

Level::Level() {

}

void Level::createLevel() {
	rectangle rec;
	point pt;
	
	//wczytywanie obramowania
	int k;
	cin>>k;
	for (int i=0; i<k; i++) {
		cin>>pt.x>>pt.y;		
		rec.upperleft = pt;
		cin>>pt.x>>pt.y;		
		rec.bottomright = pt;
		int mur;
		cin>>mur;
		Border border(rec, (bool)mur);
		borders.push_back(border);		
	}
	
	
	//inicjalizacja bloków
	int strength,type;
	int n,blocksuntouched[7];
	for(int i=0; i<7; i++) blocksuntouched[i]=0;
	cin>>n;
	for (int i=0; i<n; i++) {
		cin>>pt.x>>pt.y;
		pt.x += 3;
		pt.y -= 3;
		rec.upperleft = pt;
		cin>>pt.x>>pt.y;
		pt.x -= 3;
		pt.y += 3;
		rec.bottomright = pt;
		cin>>type;

		if(type==1) strength=1;
		else if(type==2) strength=2;
		else if (type==3) strength=INF;
		else strength=0;
		blocksuntouched[type]++;
		Block block(rec, strength, type); 
		blocks.push_back(block);
	}
	
	//platforma - player
	pt.x = 380;
	pt.y = 600;
	rec.upperleft = pt;
	pt.x = 580;
	pt.y = 590;
	rec.bottomright = pt;

	Player player(INF, rec);
	
	
	
	
	
}
