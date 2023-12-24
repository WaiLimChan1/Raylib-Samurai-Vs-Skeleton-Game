#pragma once
#include "Header.h"
#include "Ranger.h"
#include "Arrow.h"

class SamuraiArcher : public Ranger
{
protected:
	static Texture2D StaticIdleTexture;
	static Texture2D StaticWalkTexture;
	static Texture2D StaticRunTexture;
	static Texture2D StaticJumpTexture;

	static Texture2D StaticAttack1Texture;
	static Texture2D StaticAttack2Texture;
	static Texture2D StaticAttack3Texture;
	static Texture2D StaticShoot1Texture;

	static Texture2D StaticHurtTexture;
	static Texture2D StaticDeathTexture;

	static Texture2D StaticArrowTexture;

public:
	SamuraiArcher();
	SamuraiArcher(const char* name, Vector2 pos, bool facingLeft, float animationSpeed, CharacterStats* stats);

	static void LoadTextures()
	{
		StaticIdleTexture = LoadTexture("textures/Samurai_Archer/Idle.png");
		StaticWalkTexture = LoadTexture("textures/Samurai_Archer/Walk.png");
		StaticRunTexture = LoadTexture("textures/Samurai_Archer/Run.png");
		StaticJumpTexture = LoadTexture("textures/Samurai_Archer/Jump.png");

		StaticAttack1Texture = LoadTexture("textures/Samurai_Archer/Attack_1.png");
		StaticAttack2Texture = LoadTexture("textures/Samurai_Archer/Attack_2.png");
		StaticAttack3Texture = LoadTexture("textures/Samurai_Archer/Attack_3.png");
		StaticShoot1Texture = LoadTexture("textures/Samurai_Archer/Shoot_1.png");

		StaticHurtTexture = LoadTexture("textures/Samurai_Archer/Hurt.png");
		StaticDeathTexture = LoadTexture("textures/Samurai_Archer/Death.png");

		StaticArrowTexture = LoadTexture("textures/Samurai_Archer/Arrow.png");
	}

	bool attackBoxesAreActive() override;

	bool isFiringArrow() override;
	Arrow* fireArrow() override;

	void fireIfPossible(Rectangle portalHitBox, std::vector<Character*>& enemyCharacters, float range = 1500) override;

	void getCommandInputRandom() override;
	void getCommandInputHold(Rectangle portalHitBox, std::vector<Character*>& enemyCharacters) override;
	void getCommandInputAttack(Rectangle portalHitBox, std::vector<Character*>& enemyCharacters) override;
	void getCommandInputCharge(Rectangle portalHitBox, std::vector<Character*>& enemyCharacters) override;

	void takeInput() override;

	float getMeleeDamage() override;
	void unloadTexture() override;
};