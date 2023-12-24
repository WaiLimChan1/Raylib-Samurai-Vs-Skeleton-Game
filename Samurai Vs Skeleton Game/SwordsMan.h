#pragma once
#include "Header.h"
#include "Character.h"

class SwordsMan : public Character
{
protected:
	float blockPercentage; //The amount the protection blocks

public:
	SwordsMan();
	SwordsMan(const char* name, Vector2 pos, float movementSpeed, bool facingLeft, float damage, int health, float scale, float animationSpeed, float attackSpeed, int level, float blockPercentage);

	virtual bool attackBoxesAreActive() = 0;

	virtual void getCommandInputRandom() = 0;
	void getCommandInputHold(Rectangle portalHitBox, std::vector<Character*>& enemyCharacters) override;
	void getCommandInputAttack(Rectangle portalHitBox, std::vector<Character*>& enemyCharacters) override;
	void getCommandInputCharge(Rectangle portalHitBox, std::vector<Character*>& enemyCharacters) override;

	virtual void takeInput() = 0;

	virtual float getMeleeDamage() = 0;
	bool isHitByRec(Rectangle attackBox, float damage, bool facingLeft, Vector2 cameraPos) override;

	virtual void unloadTexture() = 0;
};