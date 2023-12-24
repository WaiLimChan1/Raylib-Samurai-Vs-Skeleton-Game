#pragma once
#include "Header.h"
#include "Ranger.h"
#include "Arrow.h"

class SkeletonArcher : public Ranger
{
protected:
	static Texture2D StaticIdleTexture;
	static Texture2D StaticWalkTexture;
	static Texture2D StaticJumpTexture;

	static Texture2D StaticAttack1Texture;
	static Texture2D StaticAttack2Texture;
	static Texture2D StaticAttack3Texture;
	static Texture2D StaticShoot1Texture;
	static Texture2D StaticShoot2Texture;

	static Texture2D StaticHurtTexture;
	static Texture2D StaticDeathTexture;

	static Texture2D StaticArrowTexture;
public:
	SkeletonArcher();
	SkeletonArcher(const char* name, Vector2 pos, bool facingLeft, float animationSpeed, CharacterStats* stats);

	static void LoadTextures()
	{
		StaticIdleTexture = LoadTexture("textures/Skeleton_Archer/Idle.png");
		StaticWalkTexture = LoadTexture("textures/Skeleton_Archer/Walk.png");
		StaticJumpTexture = LoadTexture("textures/Skeleton_Archer/Jump.png");

		StaticAttack1Texture = LoadTexture("textures/Skeleton_Archer/Attack_1.png");
		StaticAttack2Texture = LoadTexture("textures/Skeleton_Archer/Attack_2.png");
		StaticAttack3Texture = LoadTexture("textures/Skeleton_Archer/Attack_3.png");
		StaticShoot1Texture = LoadTexture("textures/Skeleton_Archer/Shoot_1.png");
		StaticShoot2Texture = LoadTexture("textures/Skeleton_Archer/Shoot_2.png");

		StaticHurtTexture = LoadTexture("textures/Skeleton_Archer/Hurt.png");
		StaticDeathTexture = LoadTexture("textures/Skeleton_Archer/Death.png");

		StaticArrowTexture = LoadTexture("textures/Skeleton_Archer/Arrow.png");
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