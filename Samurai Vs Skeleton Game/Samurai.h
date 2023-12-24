#pragma once
#include "Header.h"
#include "SwordsMan.h"

class Samurai : public SwordsMan
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
	Samurai();
	Samurai(const char* name, Vector2 pos, bool facingLeft, float animationSpeed, CharacterStats* stats);
	
	static void LoadTextures()
	{
		StaticIdleTexture = LoadTexture("textures/Samurai/Idle.png");
		StaticWalkTexture = LoadTexture("textures/Samurai/Walk.png");
		StaticRunTexture = LoadTexture("textures/Samurai/Run.png");
		StaticJumpTexture = LoadTexture("textures/Samurai/Jump.png");

		StaticAttack1Texture = LoadTexture("textures/Samurai/Attack_1.png");
		StaticAttack2Texture = LoadTexture("textures/Samurai/Attack_2.png");
		StaticAttack3Texture = LoadTexture("textures/Samurai/Attack_3.png");
		StaticProtectionTexture = LoadTexture("textures/Samurai/Protection.png");

		StaticHurtTexture = LoadTexture("textures/Samurai/Hurt.png");
		StaticDeathTexture = LoadTexture("textures/Samurai/Death.png");
	}

	bool attackBoxesAreActive() override;

	void getCommandInputRandom() override;

	void takeInput() override;

	float getMeleeDamage();
	void unloadTexture();
};