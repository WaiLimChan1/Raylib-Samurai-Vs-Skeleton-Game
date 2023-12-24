#pragma once
#include "Header.h"
#include "SwordsMan.h"

class SkeletonWarrior : public SwordsMan
{	
protected:
	static Texture2D StaticIdleTexture;
	static Texture2D StaticWalkTexture;
	static Texture2D StaticRunTexture;
	static Texture2D StaticRunAttackTexture;

	static Texture2D StaticAttack1Texture;
	static Texture2D StaticAttack2Texture;
	static Texture2D StaticAttack3Texture;
	static Texture2D StaticProtectionTexture;

	static Texture2D StaticHurtTexture;
	static Texture2D StaticDeathTexture;

public:
	SkeletonWarrior();
	SkeletonWarrior(const char* name, Vector2 pos, bool facingLeft, float animationSpeed, CharacterStats* stats);

	static void LoadTextures()
	{
		StaticIdleTexture = LoadTexture("textures/Skeleton_Warrior/Idle.png");
		StaticWalkTexture = LoadTexture("textures/Skeleton_Warrior/Walk.png");
		StaticRunTexture = LoadTexture("textures/Skeleton_Warrior/Run.png");
		StaticRunAttackTexture = LoadTexture("textures/Skeleton_Warrior/Run_Attack.png");

		StaticAttack1Texture = LoadTexture("textures/Skeleton_Warrior/Attack_1.png");
		StaticAttack2Texture = LoadTexture("textures/Skeleton_Warrior/Attack_2.png");
		StaticAttack3Texture = LoadTexture("textures/Skeleton_Warrior/Attack_3.png");
		StaticProtectionTexture = LoadTexture("textures/Skeleton_Warrior/Protection.png");

		StaticHurtTexture = LoadTexture("textures/Skeleton_Warrior/Hurt.png");
		StaticDeathTexture = LoadTexture("textures/Skeleton_Warrior/Death.png");
	}

	bool attackBoxesAreActive() override;

	void getCommandInputRandom() override;

	void takeInput() override;

	float getMeleeDamage();
	void unloadTexture();
};