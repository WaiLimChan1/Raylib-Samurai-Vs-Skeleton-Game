#include "Samurai.h"

Texture2D Samurai::StaticIdleTexture;
Texture2D Samurai::StaticWalkTexture;
Texture2D Samurai::StaticRunTexture;
Texture2D Samurai::StaticJumpTexture;

Texture2D Samurai::StaticAttack1Texture;
Texture2D Samurai::StaticAttack2Texture;
Texture2D Samurai::StaticAttack3Texture;
Texture2D Samurai::StaticProtectionTexture;

Texture2D Samurai::StaticHurtTexture;
Texture2D Samurai::StaticDeathTexture;

Samurai::Samurai() {}
Samurai::Samurai(const char* name, Vector2 pos, bool facingLeft, float animationSpeed, CharacterStats* stats)
	: SwordsMan(name, pos, stats->movementSpeed, facingLeft, stats->damage, stats->health, stats->scale, animationSpeed, stats->attackSpeed, stats->getLevel(), stats->blockPercentage)
{
	float movementAnimationRatio = 1 / (movementSpeed / 300.0f);
	float attackAnimationRatio = 1 / attackSpeed;

	idle = AnimationPro(StaticIdleTexture, scale, animationSpeed * 2, 6);
	walk = AnimationPro(StaticWalkTexture, scale, animationSpeed * movementAnimationRatio, 9);
	run = AnimationPro(StaticRunTexture, scale, animationSpeed * movementAnimationRatio, 8);
	jump = AnimationPro(StaticJumpTexture, scale, animationSpeed * movementAnimationRatio, 9);

	attack1 = AnimationPro(StaticAttack1Texture, scale, animationSpeed * attackAnimationRatio, 4);
	attack2 = AnimationPro(StaticAttack2Texture, scale, animationSpeed * attackAnimationRatio, 5);
	attack3 = AnimationPro(StaticAttack3Texture, scale, animationSpeed * attackAnimationRatio, 4);
	protection = AnimationPro(StaticProtectionTexture, scale, animationSpeed * attackAnimationRatio, 2);

	hurt = AnimationPro(StaticHurtTexture, scale, animationSpeed, 3);
	death = AnimationPro(StaticDeathTexture, scale, animationSpeed, 6);

	current = idle;
}

bool Samurai::attackBoxesAreActive()
{
	return (((status == ATTACK1 && current.getFrame() == 3) ||
		(status == ATTACK2 && current.getFrame() == 5) ||
		(status == ATTACK3 && current.getFrame() == 3))
		&& current.startedCurrentFrame());
}

void Samurai::getCommandInputRandom() 
{
	if (!(currentHealth <= 0)) //Can get hurt only if character isn't dead
	{
		if (status != ATTACK1 && status != ATTACK2 && status != ATTACK3 && status != JUMP && status != HURT) //Player Can't Interrupt These Animations
		{
			status = IDLE;
			int randomNum = rand() % 100;
			if (randomNum >= 0 && randomNum <= 5)
			{
				status = PROTECTION;
			}

			if (randomNum >= 6 && randomNum <= 50)
			{
				int randomNum2 = rand() % 2;
				if (randomNum2 == 0)
				{
					status = RUN;
					facingLeft = true;
				}
				if (randomNum2 == 1)
				{
					status = RUN;
					facingLeft = false;
				}
			}
			if (randomNum >= 51 && randomNum <= 80)
			{
				int randomNum2 = rand() % 2;
				if (randomNum2 == 0)
				{
					status = WALK;
					facingLeft = true;
				}
				if (randomNum2 == 1)
				{
					status = WALK;
					facingLeft = false;
				}
			}

			if (randomNum >= 81 && randomNum <= 85) status = ATTACK1;
			if (randomNum >= 86 && randomNum <= 90) status = ATTACK2;
			if (randomNum >= 91 && randomNum <= 95) status = ATTACK3;
			if (randomNum >= 96 && randomNum <= 99) status = JUMP;
		}
	}
}

void Samurai::takeInput()
{
	if (!(currentHealth <= 0)) //Can get hurt only if character isn't dead
	{
		if (status != ATTACK1 && status != ATTACK2 && status != ATTACK3 && status != JUMP && status != HURT) //Player Can't Interrupt These Animations
		{
			status = IDLE;

			if (IsKeyDown(KEY_S))
			{
				status = PROTECTION;
			}

			if (IsKeyDown(KEY_W))
			{
				if (IsKeyDown(KEY_A))
				{
					status = RUN;
					facingLeft = true;
				}
				if (IsKeyDown(KEY_D))
				{
					status = RUN;
					facingLeft = false;
				}
			}
			else
			{
				if (IsKeyDown(KEY_A))
				{
					status = WALK;
					facingLeft = true;
				}
				if (IsKeyDown(KEY_D))
				{
					status = WALK;
					facingLeft = false;
				}
			}

			if (IsKeyPressed(KEY_G)) status = ATTACK1;
			if (IsKeyPressed(KEY_H)) status = ATTACK2;
			if (IsKeyPressed(KEY_J)) status = ATTACK3;
			if (IsKeyPressed(KEY_SPACE)) status = JUMP;
		}
	}
}

float Samurai::getMeleeDamage()
{
	if (status == ATTACK1) return damage;
	else if (status == ATTACK2) return damage * 2;
	else if (status == ATTACK3) return damage;
	else return 0;
}

void Samurai::unloadTexture() {}