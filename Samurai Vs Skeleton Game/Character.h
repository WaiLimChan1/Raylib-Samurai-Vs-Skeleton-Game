#pragma once
#include "Header.h"
#include "CharacterStats.h"
#include "AnimationPro.h"
#include "Effect.h"

class Character
{
protected:
	const char* name;

	AnimationPro current;

	AnimationPro idle;
	AnimationPro walk;
	AnimationPro run;
	AnimationPro jump;

	AnimationPro attack1;
	AnimationPro attack2;
	AnimationPro attack3;
	AnimationPro shoot1;
	AnimationPro shoot2;
	AnimationPro runAttack;
	AnimationPro protection;

	AnimationPro hurt;
	AnimationPro death;

	StatusEnum previousStatus; //The previous status
	StatusEnum lastStatus; //The last immediate status
	StatusEnum status;

	CommandStatusEnum commandStatus;

	std::vector<Effect*> specialEffects; //Holding all of the special effects created by the Character in this frame

	Vector2 pos;
	float movementSpeed;

	Vector2 inAirVelocity;

	bool facingLeft;

	float damage;
	int maxHealth;
	int currentHealth;
	bool completelyDead; //Is dead and has finished death animation.

	float scale;
	float animationSpeed;
	float attackSpeed;

	int level;

public:
	Character();
	Character(const char* name, Vector2 pos, float movementSpeed, bool facingLeft, float damage, int health, float scale, float animationSpeed, float attackSpeed, int level);

	const char* getName();
	StatusEnum getStatus();
	CommandStatusEnum getCommandStatus();
	std::vector<Effect*>& getSpecialEffects();
	Vector2 getPos();
	float getWidth();
	float getHeight();
	bool getFacingLeft();
	int getCurrentHealth();
	bool getCompletelyDead();
	float getScale();
	int getLevel();

	Vector2 getHitBoxCenterPos();
	Vector2 getSpecialEffectPos();
	Vector2 getSyncCameraPos();

	void setPos(Vector2 pos);
	void setInAirVelocity(Vector2 inAirVelocity);

	void printCharacterInfo();
	void drawTargetIndicator(Vector2 cameraPos, float radius, Color color = GREEN);
	void drawHealthBar(Vector2 cameraPos, float size, Color healthColor = RED, Color borderColor = BLACK);

	Rectangle getHitBox(Vector2 cameraPos);
	void drawHitBox(Vector2 cameraPos, Color color = {230, 41, 55, 150});

	virtual bool attackBoxesAreActive() = 0;
	Rectangle getRawAttackBox(Vector2 cameraPos);
	Rectangle getAttackBox(Vector2 cameraPos);
	void drawAttackbox(Vector2 cameraPos, Color color = { 255, 161, 0, 150 });

	void draw(Vector2 cameraPos, Color tintColor = WHITE, Color hitColor = RED);
	void endPlayOnceAnimations();

	void endPreviousControl();

	void takeInputForCommandStatus();
	void getInputBasedOnCommandStatus(Rectangle portalHitBox, std::vector<Character*>& enemyCharacters);
	Vector2 findClosestEnemyPos(Rectangle portalHitBox, std::vector<Character*>& enemyCharacters);
	void attackIfPossible(Rectangle portalHitBox, std::vector<Character*>& enemyCharacters);

	virtual void getCommandInputRandom() = 0;
	virtual void getCommandInputHold(Rectangle portalHitBox, std::vector<Character*>& enemyCharacters) = 0;
	virtual void getCommandInputAttack(Rectangle portalHitBox, std::vector<Character*>& enemyCharacters) = 0;
	virtual void getCommandInputCharge(Rectangle portalHitBox, std::vector<Character*>& enemyCharacters) = 0;

	virtual void takeInput() = 0;

	virtual float getMeleeDamage() = 0;
	virtual bool isHitByRec(Rectangle attackBox, float damage, bool facingLeft, Vector2 cameraPos);
	void changeAnimation();

	bool inAir();
	void updatePosition();
	void checkBound(Vector2 horizontalBound);

	virtual void unloadTexture() = 0;
};