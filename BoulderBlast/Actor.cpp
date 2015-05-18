#include "Actor.h"
#include "StudentWorld.h"
#include <cstdlib>
#include <iostream>

Actor::Actor(int imID, int sX, int sY, StudentWorld* world)
	:GraphObject(imID, sX, sY) {
	m_world = world;
	isDead = false;
}

Wall::Wall(int sX, int sY, StudentWorld* world)
	: Actor(IID_WALL, sX, sY, world)
{
	setVisible(true);
	setID("building");
}

void Wall::doSomething() {}

movingActor::movingActor(int imID, int sX, int sY, StudentWorld* world, int h)
	:Actor(imID, sX, sY, world) 
{
	m_health = h;
};

bool movingActor::canMoveOnto(int x, int y)
{
	std::list<Actor*> charsAt = getWorld()->actorsAtCoord(x, y, this);
	if (charsAt.empty())
		return true;
	for (Actor* nextSpace : charsAt)
	{
		if (nextSpace->getID() == "building" || nextSpace->getID() == "boulder" || nextSpace->getID() == "player" || nextSpace->getID() == "bot" || nextSpace->getID() == "kbot" || (nextSpace->getID() == "hole" && getID() != "boulder"))
		{
			return false;
		}
	}
	return true;
}

bool movingActor::moveMe(GraphObject::Direction dir)
{
	int* a = directionDeltas(dir);
	if (0 <= a[0] && VIEW_WIDTH-1 >= a[0] && 0 <= a[1] && VIEW_HEIGHT-1 >= a[1] && canMoveOnto(a[0], a[1]))
	{
		moveTo(a[0], a[1]);
		setDirection(dir);
		delete[] a;
		return true;
	}
	delete [] a;
	setDirection(dir);
	return false;
}

int* Actor::directionDeltas(GraphObject::Direction dir)
{
	int* a = new int[2];
	switch (dir)
	{
		case up:
			a[0] = getX(), a[1] = getY() + 1;
			break;
		case down:
			a[0] = getX(), a[1] = getY() - 1;
			break;
		case right:
			a[0] = getX() + 1, a[1] = getY();
			break;
		case left:
			a[0] = getX() - 1, a[1] = getY();
			break;
	}
	return a;
}

void movingActor::spawnBullet()
{
	Bullet* bull = new Bullet(getX(), getY(), getWorld(), getDirection());
	getWorld()->add(bull);
	int* a = directionDeltas(getDirection());
	bull->moveTo(a[0], a[1]);
	delete[] a;
}

void movingActor::changeHealth(int x)
{
	m_health += x;
	if (m_health <= 0)
	{
		setVisible(false);
		setDead();
		playDeathSound();
	}
	else if (x < 0)
	{
		givePointsOnDeath();
		playDamagedSound();
	}
}

void movingActor::givePointsOnDeath() {}
void movingActor::playDamagedSound() {}
void movingActor::playDeathSound() {}

int movingActor::getHealth()
{
	return m_health;
}

Player::Player(int sX, int sY, StudentWorld* world)
	:movingActor(IID_PLAYER, sX, sY, world, 20)
{
	setVisible(true);
	setID("player");
	setDirection(right);
	m_ammo = 20;
}

void Player::playDamagedSound()
{
	getWorld()->playSound(SOUND_PLAYER_IMPACT);
}

void Player::playDeathSound()
{
	getWorld()->playSound(SOUND_PLAYER_DIE);
}

int Player::getAmmo()
{
	return m_ammo;
}

void Player::giveAmmo(int amount)
{
	m_ammo += amount;
}

void Player::doSomething()
{
	int ch;
	if (getWorld()->getKey(ch))
	{
		switch (ch)
		{
			case KEY_PRESS_DOWN:
			{
				handleBoulder(down);
				moveMe(down);
				break;
			}
			case KEY_PRESS_UP:
			{
				handleBoulder(up);
				moveMe(up);
				break;
			}
			case KEY_PRESS_LEFT:
			{
				handleBoulder(left);
				moveMe(left);
				break;
			}
			case KEY_PRESS_RIGHT:
			{
				handleBoulder(right);
				moveMe(right);
				break;
			}
			case KEY_PRESS_SPACE:
			{
				if (m_ammo > 0)
				{
					spawnBullet();
					getWorld()->playSound(SOUND_PLAYER_FIRE);
					m_ammo--;
				}
				break;
			}
			case KEY_PRESS_ESCAPE:
			{
				setDead();
				break;
			}
		}
	}
}

void Player::handleBoulder(GraphObject::Direction dir)
{
	int* a = directionDeltas(dir);
	if (0 <= a[0] && VIEW_WIDTH-1 >= a[0] && 0 <= a[1] && VIEW_HEIGHT-1 >= a[1] && getWorld()->actorAtCoord(a[0], a[1], this) != nullptr && getWorld()->actorAtCoord(a[0], a[1], this)->getID() == "boulder")
	{
		Boulder& boulder = dynamic_cast<Boulder&>(*getWorld()->actorAtCoord(a[0], a[1], this));
		boulder.checkMoveAndExecute(dir);
	}
	delete[] a;
}

Boulder::Boulder(int sX, int sY, StudentWorld* world)
	:movingActor(IID_BOULDER, sX, sY, world, 10)
{
	setID("boulder");
	setVisible(true);
}

void Boulder::checkMoveAndExecute(GraphObject::Direction x)
{
	int* a = directionDeltas(x);
	if (0 <= a[0] && VIEW_WIDTH-1 >= a[0] && 0 <= a[1] && VIEW_HEIGHT-1 >= a[1] && (getWorld()->actorAtCoord(a[0], a[1], this) == nullptr || getWorld()->actorAtCoord(a[0], a[1], this)->getID() == "hole"))
	{
		moveMe(x);
	}
	delete[] a;
}

void Boulder::doSomething() {}

Bullet::Bullet(int sX, int sY, StudentWorld* world, GraphObject::Direction startingDir)
	:Actor(IID_BULLET, sX, sY, world)
{
	setID("bullet");
	setDirection(startingDir);
	setVisible(true);
}

void Bullet::doSomething()
{
	checkDeathConditions();
	if (getLifeStatus()) return;
	int* a = directionDeltas(getDirection());
	if (a[0] >= 0 && a[0] <= VIEW_WIDTH - 1 && a[1] >= 0 && a[1] <= VIEW_HEIGHT - 1)
		moveTo(a[0], a[1]);
	delete[] a;
	checkDeathConditions();
}

void Bullet::checkDeathConditions()
{
	std::list<Actor*> listOf = getWorld()->actorsAtCoord(getX(), getY(), this);
	for (Actor* currSquare : listOf)
	{
		if (currSquare != nullptr && currSquare->getID() == "building")
		{
			setDead();
		}
		else if (currSquare != nullptr && (currSquare->getID() == "boulder" || currSquare->getID() == "player" || currSquare->getID() == "bot" || currSquare->getID() == "kbot"))
		{
			movingActor& temp = dynamic_cast<movingActor&>(*currSquare);
			temp.changeHealth(-2);
			setDead();
		}
	}
}

Hole::Hole(int sX, int sY, StudentWorld* world)
	: Actor(IID_HOLE, sX, sY, world) 
{
	setVisible(true);
	setID("hole");
}

void Hole::doSomething()
{
	Actor* charAt = getWorld()->actorAtCoord(getX(), getY(), this);
	if (charAt != nullptr && charAt->getID() == "boulder")
	{
		setDead();
		charAt->setDead();
	}
}

Jewel::Jewel(int sX, int sY, StudentWorld* world)
	:Goodie(IID_JEWEL, sX, sY, world)
{
	setID("jewel");
	setVisible(true);
}

void Jewel::giveReward()
{
	getWorld()->increaseScore(50);
}

Exit::Exit(int sX, int sY, StudentWorld* world)
	:Actor(IID_EXIT, sX, sY, world)
{
	setVisible(false);
	setID("exit");
	revealed = false;
}

void Exit::doSomething()
{
	if (!revealed && !getWorld()->checkForType("jewel"))
	{
		setVisible(true);
		getWorld()->playSound(SOUND_REVEAL_EXIT);
		revealed = true;
		return;
	}
	else if (!revealed)
	{
		return;
	}
	else
	{
		Actor* charAt = getWorld()->actorAtCoord(getX(), getY(), this);
		if (charAt != nullptr && charAt->getID() == "player")
		{
			getWorld()->playSound(SOUND_FINISHED_LEVEL);
			setDead();
			getWorld()->increaseScore(2000);
		}
	}
}

Goodie::Goodie(int IID, int sX, int sY, StudentWorld* world)
	:Actor(IID, sX, sY, world)
{
	m_actor = nullptr;
	setVisible(true);
}

void Goodie::doSomething()
{
	if (checkIfPlayer(m_actor))
	{
		setDead();
		giveReward();
		getWorld()->playSound(SOUND_GOT_GOODIE);
	}
}

bool Goodie::checkIfPlayer(Actor*& act)
{
	act = getWorld()->actorAtCoord(getX(), getY(), this);
	return (act != nullptr && act->getID() == "player") ? true : false;
}

Actor* Goodie::getActor()
{
	return m_actor;
}

ammoBox::ammoBox(int sX, int sY, StudentWorld* world)
	:Goodie(IID_AMMO, sX, sY, world)
{
	setID("ammo");
}

void ammoBox::giveReward()
{
	if (getActor() != nullptr)
	{
		dynamic_cast<Player&>(*getActor()).giveAmmo(20);
		getWorld()->increaseScore(100);
	}
}

healthBox::healthBox(int sX, int sY, StudentWorld* world)
	:Goodie(IID_RESTORE_HEALTH, sX, sY, world)
{
	setID("health");
}

void healthBox::giveReward()
{
	if (getActor() != nullptr)
	{
		Player& playa = dynamic_cast<Player&>(*getActor());
		playa.changeHealth(20 - playa.getHealth());
		getWorld()->increaseScore(500);
	}
}

lifeGiver::lifeGiver(int sX, int sY, StudentWorld* world)
	:Goodie(IID_EXTRA_LIFE, sX, sY, world)
{
	setVisible(true);
	setID("life");
}

void lifeGiver::giveReward()
{
	getWorld()->incLives();
	getWorld()->increaseScore(1000);
}

offensiveBot::offensiveBot(const int IID, int sX, int sY, StudentWorld* world, int health)
	:movingActor(IID, sX, sY, world, health)
{
	setVisible(true);
	setID("bot");
	currTick = 0;
	tickCount = ((28 - getWorld()->getLevel()) / 4 <= 3) ? 3 : (28 - getWorld()->getLevel()) / 4;
}

void offensiveBot::playDamagedSound()
{
	getWorld()->playSound(SOUND_ROBOT_IMPACT);
}

void offensiveBot::playDeathSound()
{
	getWorld()->playSound(SOUND_ROBOT_DIE);
}

bool offensiveBot::checkLOS(int* a, GraphObject::Direction curDir)
{
	int dx, dy;
	switch (curDir)
	{
	case up:
		dy = 1, dx = 0;
		break;
	case down:
		dy = -1, dx = 0;
		break;
	case left:
		dy = 0, dx = -1;
		break;
	case right:
		dy = 0, dx = 1;
		break;
	}
	while (a[0] >= 0 && a[0] <= VIEW_WIDTH - 1 && a[1] >= 0 && a[1] <= VIEW_HEIGHT - 1)
	{
		int x = checkConditions(a[0], a[1]);
		if (x == 2)
			return true;
		if (x == 1)
			return false;
		a[0] += dx, a[1] += dy;
	}
	return false;
}

int offensiveBot::checkConditions(int x, int y)
{
	Actor* charAt = getWorld()->actorAtCoord(x, y, this);
	if (charAt != nullptr && charAt->getID() == "player")
		return 2;
	else if (charAt != nullptr && (charAt->getID() == "building" || charAt->getID() == "boulder" || charAt->getID() == "bot" || charAt->getID() == "kbot"))
		return 1;
	return 0;
}

bool offensiveBot::doAttack()
{
	int* a = directionDeltas(getDirection());
	if (checkLOS(a, getDirection()))
	{
		spawnBullet();
		getWorld()->playSound(SOUND_ENEMY_FIRE);
		delete[] a;
		return true;
	}
	delete[] a;
	return false;
}

bool offensiveBot::checkTick()
{
	if (currTick % tickCount == 0)
	{
		currTick++;
		return true;
	}
	currTick++;
	return false;
}

bool offensiveBot::checkTickNoChange()
{
	if (currTick % tickCount == 0)
	{
		return true;
	}
	return false;
}

snarlBot::snarlBot(int sX, int sY, StudentWorld* world, bool horiz)
	:offensiveBot(IID_SNARLBOT, sX, sY, world)
{
	setVisible(true);
	(horiz) ? setDirection(right) : setDirection(down);
}

void snarlBot::doSomething()
{
	if (!checkTick())
		return;
	if (doAttack())
	{
		return;
	}
	if (!moveMe(getDirection()))
	{
		changeDirection();
		return;
	}
}

void snarlBot::changeDirection()
{
	GraphObject::Direction curDir = getDirection();
	if (curDir == up)
		setDirection(down);
	else if (curDir == down)
		setDirection(up);
	else if (curDir == left)
		setDirection(right);
	else
		setDirection(left);
}

void snarlBot::givePointsOnDeath()
{
	getWorld()->increaseScore(100);
}

snarlBot::~snarlBot() {}

Kleptobot::Kleptobot(int sX, int sY, StudentWorld* world, int imDir, int health)
	:offensiveBot(imDir, sX, sY, world, health)
{
	getWorld()->playSound(SOUND_ROBOT_BORN);
	setID("kbot");
	setDirection(right);
	distanceBeforeTurning = rand() % 6 + 1;
	m_moves = 0;
}

void Kleptobot::doSomething()
{
	if (!checkTick())
	{
		return;
	}
	if ((rand()%10 == 3) && munch()) return;
	if (m_moves >= distanceBeforeTurning || !moveMe(getDirection()))
	{
		m_moves = 0;
		distanceBeforeTurning = rand() % 6 + 1;
		GraphObject::Direction dirs[4] = { down, up, right, left };
		int index = rand() % 4, currIndex = index;
		do
		{
			currIndex++;
			if (currIndex == 4) currIndex = 0;
			if (moveMe(dirs[currIndex]))
			{
				m_moves++;
				return;
			}
		} while (currIndex != index);
		setDirection(dirs[index]);
		return;
	}
	m_moves++;
}


bool Kleptobot::munch()
{
	if (idofGoodie == "")
	{
		std::list<Actor*> goodieList = getWorld()->actorsAtCoord(getX(), getY(), this);
		for (Actor* act : goodieList)
		{
			string id = act->getID();
			if (id == "ammo" || id == "health" || id == "life")
			{
				act->setDead();
				idofGoodie = id;
				getWorld()->playSound(SOUND_ROBOT_MUNCH);
				return true;
			}
		}
	}
	return false;
}

void Kleptobot::givePointsOnDeath()
{
	getWorld()->increaseScore(10);
}

Kleptobot::~Kleptobot()
{
	if (idofGoodie != "")
	{
		if (idofGoodie == "ammo")
			getWorld()->add(new ammoBox(getX(), getY(), getWorld()));
		if (idofGoodie == "health")
			getWorld()->add(new healthBox(getX(), getY(), getWorld()));
		if (idofGoodie == "life")
			getWorld()->add(new lifeGiver(getX(), getY(), getWorld()));
	}
}

KleptobotFactory::KleptobotFactory(int sX, int sY, StudentWorld* world, bool if_angry)
	:Actor(IID_ROBOT_FACTORY, sX, sY, world)
{
	setVisible(true);
	setID("building");
	m_type = if_angry;
}

void KleptobotFactory::doSomething()
{
	int totalCount = 0;
	int startX = (getX() - 3 >= 0) ? getX() - 3 : 0, endX = (getX() + 3 <= VIEW_WIDTH - 1) ? getX() + 3 : VIEW_WIDTH - 1;
	int startY = (getY() - 3 >= 0) ? getY() - 3 : 0, endY = (getY() + 3 <= VIEW_HEIGHT - 1) ? getY() + 3 : VIEW_HEIGHT - 1;
	for (int x = startX; x <= endX && totalCount < 3; x++)
	{
		for (int y = startY; y <= endY && totalCount < 3; y++)
		{
			updateCount(x, y, totalCount);
		}
	}
	if (totalCount < 3)
	{
		if (getWorld()->actorAtCoord(getX(), getY(), this) != nullptr)
			return;
		int chance = rand() % 50;
		if (chance == 24)
		{
			if (m_type)
			{
				getWorld()->add(new AngryKleptobot(getX(), getY(), getWorld()));
				return;
			}
			getWorld()->add(new Kleptobot(getX(), getY(), getWorld()));
		}
	}
}

void KleptobotFactory::updateCount(int x, int y, int& count)
{
	std::list<Actor*> currList = getWorld()->actorsAtCoord(x, y, this);
	for (Actor* m : currList)
	{
		if (m->getID() == "kbot")
			count++;
	}
}

AngryKleptobot::AngryKleptobot(int sX, int sY, StudentWorld* world)
	:Kleptobot(sX, sY, world, IID_ANGRY_KLEPTOBOT, 8)  
{
	setID("kbot");
}

void AngryKleptobot::doSomething()
{
	if (checkTickNoChange())
	{
		if (doAttack())
		{
			checkTick();
			return;
		}
		else
		{
			Kleptobot::doSomething();
			return;
		}
	}
	checkTick();
}

void AngryKleptobot::givePointsOnDeath()
{
	getWorld()->increaseScore(20);
}