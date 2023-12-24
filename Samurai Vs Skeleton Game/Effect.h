#pragma once
#include "Header.h"
#include "AnimationPro.h"

class Effect
{
private:
	static AnimationPro StaticPortalEffectAnimation;
	static AnimationPro StaticSpawnEffectAnimation;
	static AnimationPro StaticExplosionEffectAnimation;
	static AnimationPro StaticBloodEffectAnimation;
	static AnimationPro StaticBlockEffectAnimation;

	AnimationPro animation;
	Vector2 pos;
	bool facingLeft;
	float scale;

public:
	Effect();
	Effect(SpecialEffectEnum specialEffect, Vector2 pos, bool facingLeft, float scale);

	static void LoadAnimations()
	{
		StaticPortalEffectAnimation = AnimationPro(LoadTexture("textures/Effects/Blue Portal.png"), 1, 1.0f / 48.0f, 59);
		StaticSpawnEffectAnimation = AnimationPro(LoadTexture("textures/Effects/Spawn Effect.png"), 1, 1.0f / 12.0f, 4);
		StaticExplosionEffectAnimation = AnimationPro(LoadTexture("textures/Effects/Explosion Effect.png"), 1, 1.0f / 12.0f, 12);
		StaticBloodEffectAnimation = AnimationPro(LoadTexture("textures/Effects/Blood Effect.png"), 1, 1.0f / 48.0f, 20);
		StaticBlockEffectAnimation = AnimationPro(LoadTexture("textures/Effects/Block Effect.png"), 1, 1.0f / 12.0f, 3);
	}

	Vector2 getPos();
	void setPos(Vector2 pos);
	Vector2 getHitBoxCenterPos();
	bool getFacingLeft();
	float getScale();
	float getWidth();
	float getHeight();

	Rectangle getHitBox(Vector2 cameraPos);
	void drawHitBox(Vector2 cameraPos, Color color = { 230, 41, 55, 150 });

	void draw(Vector2 cameraPos, Color colorTint = WHITE);

	bool finishedAnimation();
};