#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <list>
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir) {}

	virtual int init();

	virtual int move();

	virtual void cleanUp();

	~StudentWorld();

	Actor* actorAtCoord(int x, int y, Actor* notWanted);
	void add(Actor* newActor);
	bool checkForType(std::string type);
	std::list<Actor*> actorsAtCoord(int x, int y, Actor* notWanted);
private:
	void updateTitleText();
	std::list<Actor*> m_list;
	int loadLevel(std::list<Actor*>& holder);
	int bonusScore;
	Player* player;
	void removeDeadObjects();
};

#endif // STUDENTWORLD_H_
