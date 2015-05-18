#include "StudentWorld.h"
#include "Level.h"
#include <algorithm>
#include <string>
#include <iomanip>
#include <sstream>
#include "GraphObject.h"
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

int StudentWorld::init()
{
	bonusScore = 1000;
	return loadLevel(m_list);
}

int StudentWorld::loadLevel(std::list<Actor*>& holder)
{
	Level lev(assetDirectory());
	if (getLevel() > 99)
		return GWSTATUS_PLAYER_WON;
	ostringstream oss;
	oss.fill('0');
	oss << "level" << setw(2) << getLevel() << ".dat";
	Level::LoadResult result = lev.loadLevel(oss.str());
	if (result == Level::load_fail_file_not_found)
		return GWSTATUS_PLAYER_WON;
	else if (result == Level::load_fail_bad_format)
		return GWSTATUS_LEVEL_ERROR;
	else if (result == Level::load_success)
	{
		for (int x = 0; x <= 14; x++)
		{
			for (int y = 0; y <= 14; y++)
			{
				Level::MazeEntry ge = lev.getContentsOf(x, y);
				switch (ge)
				{
				case Level::player:
				{
					player = new Player(x, y, this);
					holder.push_back(player);
					break;
				}
				case Level::wall:
				{
					holder.push_back(new Wall(x, y, this));
					break;
				}
				case Level::boulder:
				{
					holder.push_back(new Boulder(x, y, this));
					break;
				}
				case Level::hole:
				{
					holder.push_back(new Hole(x, y, this));
					break;
				}
				case Level::jewel:
				{
					holder.push_back(new Jewel(x, y, this));
					break;
				}
				case Level::exit:
				{
					holder.push_back(new Exit(x, y, this));
					break;
				}
				case Level::ammo:
				{
					holder.push_back(new ammoBox(x, y, this));
					break;
				}
				case Level::restore_health:
				{
					holder.push_back(new healthBox(x, y, this));
					break;
				}
				case Level::extra_life:
				{
					holder.push_back(new lifeGiver(x, y, this));
					break;
				}
				case Level::horiz_snarlbot:
				{
					holder.push_back(new snarlBot(x, y, this, true));
					break;
				}
				case Level::vert_snarlbot:
				{
					holder.push_back(new snarlBot(x, y, this, false));
					break;
				}
				case Level::kleptobot_factory:
				{
					holder.push_back(new KleptobotFactory(x, y, this, false));
					break;
				}
				case Level::angry_kleptobot_factory:
				{
					holder.push_back(new KleptobotFactory(x, y, this, true));
				}
				}
			}
		}
	}

	return GWSTATUS_CONTINUE_GAME;
}

list<Actor*> StudentWorld::actorsAtCoord(int x, int y, Actor* notWanted)
{
	list<Actor*> coordActors;
	for (Actor* currActor : m_list)
	{
		if (currActor != notWanted && currActor->getX() == x && currActor->getY() == y)
			coordActors.push_back(currActor);
	}
	return coordActors;
}

Actor* StudentWorld::actorAtCoord(int x, int y, Actor* notWanted)
{
	Actor* returner = nullptr;
	for (Actor* currActor : m_list)
	{
		if (currActor != notWanted && currActor->getX() == x && currActor->getY() == y)
		{
			if (returner == nullptr)
			{
				returner = currActor;
			}
			else
			{
				if (returner->getID() == "ammo" || returner->getID() == "health" || returner->getID() == "life")
				{
					returner = currActor;
				}
			}
		}
	}
	return returner;
}

void StudentWorld::add(Actor* newActor)
{
	if (newActor->getID() == "bullet")
		m_list.push_front(newActor);
	else
		m_list.push_back(newActor);
}

bool StudentWorld::checkForType(string type)
{
	for (Actor* currActor : m_list)
	{
		if (currActor->getID() == type)
			return true;
	}
	return false;
}

void StudentWorld::updateTitleText()
{
	ostringstream oss;
	int currLevel = getLevel(), currLives = getLives(), currScore = getScore();
	oss << "Score: ";
	oss.fill('0');
	oss << setw(7) << getScore();
	oss << "  ";
	oss << "Level: " << setw(2) << currLevel << "  ";
	oss.fill(' ');
	oss << "Lives: " << setw(2) << currLives << "  ";
	int health = 5* player->getHealth();
	oss << "Health: " << setw(3) << health << "%  ";
	oss << "Ammo: " << setw(3) << player->getAmmo() << "  ";
	oss << "Bonus: " << setw(4) << bonusScore;
	setGameStatText(oss.str());

}

int StudentWorld::move()
{
	updateTitleText();

	for (list<Actor*>::iterator it = m_list.begin(); it != m_list.end(); it++)
	{
		if (!(*it)->getLifeStatus())
			(*it)->doSomething();
		if (player->getLifeStatus())
		{
			decLives();
			return GWSTATUS_PLAYER_DIED;
		}
		if (!checkForType("exit"))
		{
			increaseScore(bonusScore);
			return GWSTATUS_FINISHED_LEVEL;
		}
	}

	removeDeadObjects();

	if (bonusScore > 0)
		bonusScore--;

	if (player->getLifeStatus())
	{
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}

	if (!checkForType("exit"))
	{
		increaseScore(bonusScore);
		return GWSTATUS_FINISHED_LEVEL;
	}

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	for (list<Actor*>::iterator it = m_list.begin(); it != m_list.end();)
	{
		delete *it;
		it = m_list.erase(it);
	}
	player = nullptr;
}

void StudentWorld::removeDeadObjects()
{
	for (list<Actor*>::iterator it = m_list.begin(); it != m_list.end(); )
	{
		if ((*it)->getLifeStatus())
		{
			delete *it;
			it = m_list.erase(it);
		}
		else
			it++;
	}
}

StudentWorld::~StudentWorld()
{
	cleanUp();
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp