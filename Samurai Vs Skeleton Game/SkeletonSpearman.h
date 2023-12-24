#pragma once
#include "Header.h"
#include "SwordsMan.h"

class SkeletonSpearman : public SwordsMan
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
	SkeletonSpearman();
	SkeletonSpearman(const char* name, Vector2 pos, bool facingLeft, float animationSpeed, CharacterStats* stats);

	static void LoadTextures()
	{
		StaticIdleTexture = LoadTexture("textures/Skeleton_Spearman/Idle.png");
		StaticWalkTexture = LoadTexture("textures/Skeleton_Spearman/Walk.png");
		StaticRunTexture = LoadTexture("textures/Skeleton_Spearman/Run.png");
		StaticRunAttackTexture = LoadTexture("textures/Skeleton_Spearman/Run_Attack.png");

		StaticAttack1Texture = LoadTexture("textures/Skeleton_Spearman/Attack_1.png");
		StaticAttack2Texture = LoadTexture("textures/Skeleton_Spearman/Attack_2.png");
		StaticAttack3Texture = LoadTexture("textures/Skeleton_Spearman/Attack_3.png");
		StaticProtectionTexture = LoadTexture("textures/Skeleton_Spearman/Protection.png");

		StaticHurtTexture = LoadTexture("textures/Skeleton_Spearman/Hurt.png");
		StaticDeathTexture = LoadTexture("textures/Skeleton_Spearman/Death.png");
	}

	bool attackBoxesAreActive() override;

	void getCommandInputRandom() override;

	void takeInput() override;

	float getMeleeDamage();
	void unloadTexture();
};