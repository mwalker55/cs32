#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
#include <string>

using std::string;

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject
{
	public:
		Actor(int imID, int sX, int sY, StudentWorld* world);
		virtual void doSomething() = 0;
		StudentWorld* getWorld() const;
		virtual ~Actor() = default;
		void setID(string ID);
		string getID() const;
		bool getLifeStatus() const;
		void setDead(); 
		int* directionDeltas(GraphObject::Direction dir);
	private:
		StudentWorld* m_world;
		string m_id;
		bool isDead;
};

inline string Actor::getID() const
{
	return m_id;
}

inline void Actor::setID(string ID)
{
	m_id = ID;
}

inline bool Actor::getLifeStatus() const
{
	return isDead;
}

inline void Actor::setDead()
{
	isDead = true;
}


inline StudentWorld* Actor::getWorld() const
{
	return m_world;
}

class Wall : public Actor
{
	public:
		Wall(int sX, int sY, StudentWorld* world);
		virtual void doSomething();
		virtual ~Wall() = default;
};

class movingActor : public Actor
{
	public:
		movingActor(int imID, int sX, int sY, StudentWorld* world, int health);
		virtual ~movingActor() = default;
		virtual void doSomething() = 0;
		void changeHealth(int x);
		virtual void playDeathSound();
		virtual void playDamagedSound();
		bool moveMe(GraphObject::Direction x);
		void spawnBullet();
		int getHealth();
	private:
		bool canMoveOnto(int x, int y);
		virtual void givePointsOnDeath();
		int m_health;
};

class Player : public movingActor
{
	public:
		Player(int sX, int sY, StudentWorld* world);
		virtual void doSomething();
		virtual ~Player() = default;
		int getAmmo();
		void giveAmmo(int amount);
		virtual void playDamagedSound();
		virtual void playDeathSound();
	private:
		void handleBoulder(GraphObject::Direction dir);
		int m_ammo;
};

class Boulder : public movingActor
{
	public:
		Boulder(int sX, int sY, StudentWorld* world);
		virtual void doSomething();
		virtual ~Boulder() = default;
		void checkMoveAndExecute(GraphObject::Direction x);
};

class Bullet : public Actor
{
	public:
		Bullet(int sX, int sY, StudentWorld* world, GraphObject::Direction startingDir);
		virtual void doSomething();
		virtual ~Bullet() = default;
	private:
		void checkDeathConditions();
};

class Hole : public Actor
{
	public:
		Hole(int sX, int sY, StudentWorld* world);
		virtual void doSomething();
		virtual ~Hole() = default;
};

class Exit : public Actor
{
	public:
		Exit(int sX, int sY, StudentWorld* world);
		virtual void doSomething();
		virtual ~Exit() = default;
	private:
		bool revealed;
};

class Goodie : public Actor
{
	public:
		Goodie(int IID, int sX, int sY, StudentWorld* world);
		virtual void doSomething();
		virtual ~Goodie() = default;
		virtual void giveReward() = 0;
		Actor* getActor();
	protected:
		bool checkIfPlayer(Actor*& ptr);
	private:
		Actor* m_actor;
};

class Jewel : public Goodie
{
	public:
		Jewel(int sX, int sY, StudentWorld* world);
		virtual ~Jewel() = default;
		virtual void giveReward();
};

class ammoBox : public Goodie
{
	public:
		ammoBox(int sX, int sY, StudentWorld* world);
		virtual ~ammoBox() = default;
		virtual void giveReward();

};

class healthBox : public Goodie
{
	public:
		healthBox(int sX, int sY, StudentWorld* world);
		virtual ~healthBox() = default;
		virtual void giveReward();
};

class lifeGiver : public Goodie
{
	public:
		lifeGiver(int sX, int sY, StudentWorld* world);
		virtual ~lifeGiver() = default;
		virtual void giveReward();
};

class offensiveBot : public movingActor
{
	public:
		offensiveBot(int IID, int sX, int sY, StudentWorld* world, int health = 10);
		virtual void doSomething() = 0;
		virtual ~offensiveBot() = default;
		virtual void playDamagedSound();
		virtual void playDeathSound();
	protected:
		bool checkLOS(int* a, GraphObject::Direction curDir);
		bool doAttack();
		bool checkTick();
		bool checkTickNoChange();
	private:
		int currTick;
		int tickCount;
		int checkConditions(int x, int y);
};

class snarlBot : public offensiveBot
{
	public:
		snarlBot(int sX, int sY, StudentWorld* world, bool horiz);
		virtual void doSomething();
		virtual ~snarlBot();
	private:
		virtual void givePointsOnDeath();
		void changeDirection();
};

class Kleptobot : public offensiveBot
{
	public:
		Kleptobot(int sX, int sY, StudentWorld* world, int imDir = IID_KLEPTOBOT, int health = 5);
		virtual void doSomething();
		virtual ~Kleptobot();
	private:
		virtual void givePointsOnDeath();
		int distanceBeforeTurning;
		int m_moves;
		string idofGoodie;
		bool munch();
};

class KleptobotFactory : public Actor
{
	public:
		KleptobotFactory(int sX, int sY, StudentWorld* world, bool if_angry);
		virtual void doSomething();
		virtual ~KleptobotFactory() = default;
	private:
		bool m_type;
		void updateCount(int x, int y, int& count);
		
};

class AngryKleptobot : public Kleptobot
{
	public:
		AngryKleptobot(int sX, int sY, StudentWorld* world);
		virtual void doSomething();
		virtual ~AngryKleptobot() = default;
	private:
		virtual void givePointsOnDeath();
};

#endif // ACTOR_H_