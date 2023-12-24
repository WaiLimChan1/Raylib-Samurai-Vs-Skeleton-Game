#pragma once
#include "Header.h"
#include "AnimationPro.h"
#include "Effect.h"

#include "Samurai.h"
#include "SamuraiCommander.h"
#include "SamuraiArcher.h"

#include "SkeletonWarrior.h"
#include "SkeletonSpearman.h"
#include "SkeletonArcher.h"

class Portal
{
private:
	Effect portalEffect;

	int currentHealth;
	int maxHealth;
	bool destroyed;

	CharacterStats characterStats;
	int SamuraiLevel;
	int SamuraiCommanderLevel;
	int SamuraiArcherLevel;

	int SkeletonWarriorLevel;
	int SkeletonSpearmanLevel;
	int SkeletonArcherLevel;

	std::vector<Effect*> specialEffects; //Holding all of the special effects created by the Portal in this frame

public:
	Portal();
	Portal(int health, Vector2 pos, bool facingLeft, float scale);

	Effect& getPortalEffect();
	float getUpgradePrice(CharacterENUM characterEnum);
	float getPurchasePrice(CharacterENUM characterEnum);
	std::vector<Effect*>& getSpecialEffects();
	int getLevel(CharacterENUM characterEnum);
	Vector2 getHitBoxCenterPos();
	int getCurrentHealth();
	int getMaxHealth();

	void setCurrentHealth(int currentHealth);
	void setMaxHealth(int maxHealth);
	void setLevel(CharacterENUM characterEnum, int level);

	bool upgrade(CharacterENUM characterEnum);
	bool upgrade(CharacterENUM characterEnum, int& money);

	Rectangle getHitBox(Vector2 cameraPos);
	void drawHitBox(Vector2 cameraPos, Color color = { 230, 41, 55, 150 });

	void drawHealthBar(Vector2 cameraPos, float size, Color healthColor = RED, Color borderColor = BLACK);
	void draw(Vector2 cameraPos, Color colorTint = WHITE);

	bool hasEnoughMoneyForSpawningCharacter(CharacterENUM characterEnum, int& money, bool buying);
	Character* createCharacter(CharacterENUM characterEnum, const char* name, float animationSpeed);
	Character* createBoss(CharacterENUM characterEnum, const char* name, float animationSpeed, CharacterStats* characterStats);
	bool isHitByRec(Rectangle attackBox, float damage, Vector2 cameraPos);
};