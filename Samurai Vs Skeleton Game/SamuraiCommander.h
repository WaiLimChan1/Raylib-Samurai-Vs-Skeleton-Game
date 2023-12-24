#pragma once
#include "Header.h"
#include "SwordsMan.h"

class SamuraiCommander : public SwordsMan
{
protected:
	static Texture2D StaticIdleTexture;
	static Texture2D StaticWalkTexture;
	static Texture2D StaticRunTexture;
	static Texture2D StaticJumpTexture;

	static Texture2D StaticAttack1Texture;
	static Texture2D StaticAttack2Texture;
	static Texture2D StaticAttack3Texture;
	static Texture2D StaticProtectionTexture;

	static Texture2D StaticHurtTexture;
	static Texture2D StaticDeathTexture;

public:
	SamuraiCommander();
	SamuraiCommander(const char* name, Vector2 pos, bool facingLeft, float animationSpeed, CharacterStats* stats);

	static void LoadTextures()
	{
		StaticIdleTexture = LoadTexture("textures/Samurai_Commander/Idle.png");
		StaticWalkTexture = LoadTexture("textures/Samurai_Commander/Walk.png");
		StaticRunTexture = LoadTexture("textures/Samurai_Commander/Run.png");
		StaticJumpTexture = LoadTexture("textures/Samurai_Commander/Jump.png");

		StaticAttack1Texture = LoadTexture("textures/Samurai_Commander/Attack_1.png");
		StaticAttack2Texture = LoadTexture("textures/Samurai_Commander/Attack_2.png");
		StaticAttack3Texture = LoadTexture("textures/Samurai_Commander/Attack_3.png");
		StaticProtectionTexture = LoadTexture("textures/Samurai_Commander/Protection.png");

		StaticHurtTexture = LoadTexture("textures/Samurai_Commander/Hurt.png");
		StaticDeathTexture = LoadTexture("textures/Samurai_Commander/Death.png");
	}

	bool attackBoxesAreActive() override;

	void getCommandInputRandom() override;

	void takeInput() override;

	float getMeleeDamage();
	void unloadTexture();
};