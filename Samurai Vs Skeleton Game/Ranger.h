#pragma once
#include "Header.h"
#include "Character.h"
#include "Arrow.h"

class Ranger : public Character
{
protected:
	Texture arrowTexture;
	float arrowDamage;
	float arrowSpeed;
	float arrowRange;

	float fireSpeed;

public:
	Ranger();
	Ranger(const char* name, Vector2 pos, float movementSpeed, bool facingLeft, float damage, int health, float scale, float animationSpeed, float attackSpeed, int level, float fireSpeed);

	virtual bool attackBoxesAreActive() = 0;

	virtual bool isFiringArrow() = 0;
	virtual Arrow* fireArrow() = 0;

	virtual void fireIfPossible(Rectangle portalHitBox, std::vector<Character*>& enemyCharacters, float range) = 0;

	virtual void getCommandInputRandom() = 0;
	virtual void getCommandInputHold(Rectangle portalHitBox, std::vector<Character*>& enemyCharacters) = 0;
	virtual void getCommandInputAttack(Rectangle portalHitBox, std::vector<Character*>& enemyCharacters) = 0;
	virtual void getCommandInputCharge(Rectangle portalHitBox, std::vector<Character*>& enemyCharacters) = 0;

	virtual void takeInput() = 0;

	virtual float getMeleeDamage() = 0;
	virtual void unloadTexture() = 0;
};