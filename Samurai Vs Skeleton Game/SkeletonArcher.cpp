#include "SkeletonArcher.h"

Texture2D SkeletonArcher::StaticIdleTexture;
Texture2D SkeletonArcher::StaticWalkTexture;
Texture2D SkeletonArcher::StaticJumpTexture;

Texture2D SkeletonArcher::StaticAttack1Texture;
Texture2D SkeletonArcher::StaticAttack2Texture;
Texture2D SkeletonArcher::StaticAttack3Texture;
Texture2D SkeletonArcher::StaticShoot1Texture;
Texture2D SkeletonArcher::StaticShoot2Texture;

Texture2D SkeletonArcher::StaticHurtTexture;
Texture2D SkeletonArcher::StaticDeathTexture;

Texture2D SkeletonArcher::StaticArrowTexture;

SkeletonArcher::SkeletonArcher() {}
SkeletonArcher::SkeletonArcher(const char* name, Vector2 pos, bool facingLeft, float animationSpeed, CharacterStats* stats)
	: Ranger(name, pos, stats->movementSpeed, facingLeft, stats->damage, stats->health, stats->scale, animationSpeed, stats->attackSpeed, stats->getLevel(), stats->fireSpeed)
{
	float movementAnimationRatio = 1 / (movementSpeed / 300.0f);
	float attackAnimationRatio = 1 / attackSpeed;
	float shootAnimationRatio = 1 / fireSpeed;

	idle = AnimationPro(StaticIdleTexture, scale, animationSpeed * 2, 7);
	walk = AnimationPro(StaticWalkTexture, scale, animationSpeed * movementAnimationRatio, 8);
	jump = AnimationPro(StaticJumpTexture, scale, animationSpeed * movementAnimationRatio, 6);

	attack1 = AnimationPro(StaticAttack1Texture, scale, animationSpeed * attackAnimationRatio, 5);
	attack2 = AnimationPro(StaticAttack2Texture, scale, animationSpeed * attackAnimationRatio, 4);
	attack3 = AnimationPro(StaticAttack3Texture, scale, animationSpeed * attackAnimationRatio, 3);
	shoot1 = AnimationPro(StaticShoot1Texture, scale, animationSpeed * shootAnimationRatio, 15);
	shoot2 = AnimationPro(StaticShoot2Texture, scale, animationSpeed * shootAnimationRatio, 15);

	hurt = AnimationPro(StaticHurtTexture, scale, animationSpeed, 2);
	death = AnimationPro(StaticDeathTexture, scale, animationSpeed, 5);

	current = idle;

	arrowTexture = StaticArrowTexture;
	arrowDamage = this->damage * 2;
	arrowSpeed = 1000;
	arrowRange = 5000;
}

bool SkeletonArcher::attackBoxesAreActive()
{
	return (((status == ATTACK1 && current.getFrame() == 4) ||
		(status == ATTACK2 && current.getFrame() == 3) ||
		(status == ATTACK3 && current.getFrame() == 2))
		&& current.startedCurrentFrame());
}

bool SkeletonArcher::isFiringArrow() { return (status == SHOOT1 && current.getFrame() == 13 && current.startedCurrentFrame()) || 
												(status == SHOOT2 && current.getFrame() == 13 && current.startedCurrentFrame()); }

Arrow* SkeletonArcher::fireArrow()
{
	if (isFiringArrow())
	{
		Vector2 arrowPos = pos;
		arrowPos.y += 3 * scale;
		if (status == SHOOT1) arrowPos.y += 3 * (arrowTexture.height * scale / 3);
		else if (status == SHOOT2) arrowPos.y += 4 * (arrowTexture.height * scale / 3);
		if (!facingLeft) arrowPos.x += arrowTexture.width * scale;

		return new Arrow(arrowTexture, scale, arrowPos, facingLeft, arrowDamage, arrowSpeed, arrowRange);
	}
	return nullptr;
}

void SkeletonArcher::fireIfPossible(Rectangle portalHitBox, std::vector<Character*>& enemyCharacters, float range)
{
	portalHitBox.x = portalHitBox.x + portalHitBox.width / 2;
	portalHitBox.y = portalHitBox.y = portalHitBox.height / 2;
	if (abs(getHitBoxCenterPos().x - portalHitBox.x) <= range && ((facingLeft && portalHitBox.x < getHitBoxCenterPos().x) || !facingLeft && portalHitBox.x > getHitBoxCenterPos().x))
	{
		int randNum = rand() % 2;
		if (randNum == 0) status = SHOOT1;
		else status = SHOOT2;
		return;
	}

	for (auto& enemy : enemyCharacters)
	{
		if (abs(getHitBoxCenterPos().x - enemy->getHitBoxCenterPos().x) <= range && ((facingLeft && enemy->getHitBoxCenterPos().x < getHitBoxCenterPos().x) || !facingLeft && enemy->getHitBoxCenterPos().x > getHitBoxCenterPos().x))
		{
			int randNum = rand() % 2;
			if (randNum == 0) status = SHOOT1;
			else status = SHOOT2;
			break;
		}
	}
}

void SkeletonArcher::getCommandInputRandom()
{
	if (!(currentHealth <= 0)) //Can get hurt only if character isn't dead
	{
		if (status != ATTACK1 && status != ATTACK2 && status != ATTACK3 && status != SHOOT1 && status != SHOOT2 && status != JUMP && status != HURT) //Player Can't Interrupt These Animations
		{
			status = IDLE;

			int randomNum = rand() % 100;
			if (randomNum >= 0 && randomNum <= 20)
			{
				status = SHOOT1;
			}

			if (randomNum >= 21 && randomNum <= 50)
			{
				int randomNum2 = rand() % 2;
				if (randomNum2 == 0)
				{
					status = SHOOT2;
					facingLeft = true;
				}
				if (randomNum2 == 1)
				{
					status = SHOOT2;
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
void SkeletonArcher::getCommandInputHold(Rectangle portalHitBox, std::vector<Character*>& enemyCharacters)
{
	if (!(currentHealth <= 0)) //Can get hurt only if character isn't dead
	{
		if (status != ATTACK1 && status != ATTACK2 && status != ATTACK3 && status != SHOOT1 && status != SHOOT2 && status != JUMP && status != HURT) //Player Can't Interrupt These Animations
		{
			status = IDLE;
			fireIfPossible(portalHitBox, enemyCharacters);
			attackIfPossible(portalHitBox, enemyCharacters);
		}
	}
}
void SkeletonArcher::getCommandInputAttack(Rectangle portalHitBox, std::vector<Character*>& enemyCharacters)
{
	if (!(currentHealth <= 0)) //Can get hurt only if character isn't dead
	{
		if (status != ATTACK1 && status != ATTACK2 && status != ATTACK3 && status != SHOOT1 && status != SHOOT2 && status != JUMP && status != HURT) //Player Can't Interrupt These Animations
		{
			status = IDLE;
			Vector2 closestEnemyPos = findClosestEnemyPos(portalHitBox, enemyCharacters);
			if (closestEnemyPos.x != 0 && closestEnemyPos.y != 0)
			{
				if (getHitBoxCenterPos().x < closestEnemyPos.x) facingLeft = false;
				else if (getHitBoxCenterPos().x > closestEnemyPos.x) facingLeft = true;
				status = WALK;
			}
			fireIfPossible(portalHitBox, enemyCharacters);
			attackIfPossible(portalHitBox, enemyCharacters);
		}
	}
}
void SkeletonArcher::getCommandInputCharge(Rectangle portalHitBox, std::vector<Character*>& enemyCharacters)
{
	if (!(currentHealth <= 0)) //Can get hurt only if character isn't dead
	{
		if (status != ATTACK1 && status != ATTACK2 && status != ATTACK3 && status != SHOOT1 && status != SHOOT2 && status != JUMP && status != HURT) //Player Can't Interrupt These Animations
		{
			status = IDLE;
			Vector2 closestEnemyPos = findClosestEnemyPos(portalHitBox, enemyCharacters);
			if (closestEnemyPos.x != 0 && closestEnemyPos.y != 0)
			{
				if (getHitBoxCenterPos().x < closestEnemyPos.x) facingLeft = false;
				else if (getHitBoxCenterPos().x > closestEnemyPos.x) facingLeft = true;
				status = WALK;
			}
			attackIfPossible(portalHitBox, enemyCharacters);
		}
	}
}

void SkeletonArcher::takeInput()
{
	if (!(currentHealth <= 0)) //Can get hurt only if character isn't dead
	{
		if (status != ATTACK1 && status != ATTACK2 && status != ATTACK3 && status != SHOOT1 && status != SHOOT2 && status != JUMP && status != HURT) //Player Can't Interrupt These Animations
		{
			status = IDLE;

			if (IsKeyPressed(KEY_S))
			{
				status = SHOOT1;
			}

			if (IsKeyDown(KEY_W))
			{
				if (IsKeyDown(KEY_A))
				{
					status = SHOOT2;
					facingLeft = true;
				}
				if (IsKeyDown(KEY_D))
				{
					status = SHOOT2;
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

			if (IsKeyPressed(KEY_SPACE)) { status = JUMP; }
		}
	}
}

float SkeletonArcher::getMeleeDamage()
{
	if (status == ATTACK1) return damage;
	else if (status == ATTACK2) return damage / 2;
	else if (status == ATTACK3) return damage / 3;
	else return 0;
}

void SkeletonArcher::unloadTexture() {}