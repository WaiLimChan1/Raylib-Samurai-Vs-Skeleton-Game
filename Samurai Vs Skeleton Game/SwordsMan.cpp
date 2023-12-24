#include "SwordsMan.h"

SwordsMan::SwordsMan() {}
SwordsMan::SwordsMan(const char* name, Vector2 pos, float movementSpeed, bool facingLeft, float damage, int health, float scale, float animationSpeed, float attackSpeed, int level, float blockPercentage)
	: Character(name, pos, movementSpeed, facingLeft, damage, health, scale, animationSpeed, attackSpeed, level), blockPercentage(blockPercentage) {}

bool SwordsMan::attackBoxesAreActive()
{
	return (((status == ATTACK1 && current.getFrame() == 3) ||
		(status == ATTACK2 && current.getFrame() == 5) ||
		(status == ATTACK3 && current.getFrame() == 3))
		&& current.startedCurrentFrame());
}

void SwordsMan::getCommandInputHold(Rectangle portalHitBox, std::vector<Character*>& enemyCharacters)
{
	if (!(currentHealth <= 0)) //Can get hurt only if character isn't dead
	{
		if (status != ATTACK1 && status != ATTACK2 && status != ATTACK3 && status != RUNATTACK && status != JUMP && status != HURT) //Player Can't Interrupt These Animations
		{
			status = PROTECTION;
			attackIfPossible(portalHitBox, enemyCharacters);
		}
	}
}
void SwordsMan::getCommandInputAttack(Rectangle portalHitBox, std::vector<Character*>& enemyCharacters)
{
	if (!(currentHealth <= 0)) //Can get hurt only if character isn't dead
	{
		if (status != ATTACK1 && status != ATTACK2 && status != ATTACK3 && status != RUNATTACK && status != JUMP && status != HURT) //Player Can't Interrupt These Animations
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
void SwordsMan::getCommandInputCharge(Rectangle portalHitBox, std::vector<Character*>& enemyCharacters)
{
	if (!(currentHealth <= 0)) //Can get hurt only if character isn't dead
	{
		if (status != ATTACK1 && status != ATTACK2 && status != ATTACK3 && status != RUNATTACK && status != JUMP && status != HURT) //Player Can't Interrupt These Animations
		{
			status = IDLE;
			Vector2 closestEnemyPos = findClosestEnemyPos(portalHitBox, enemyCharacters);
			if (closestEnemyPos.x != 0 && closestEnemyPos.y != 0)
			{
				if (getHitBoxCenterPos().x < closestEnemyPos.x) facingLeft = false;
				else if (getHitBoxCenterPos().x > closestEnemyPos.x) facingLeft = true;
				status = RUN;
			}
			attackIfPossible(portalHitBox, enemyCharacters);
		}
	}
}

void SwordsMan::takeInput()
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

			if (IsKeyPressed(KEY_SPACE))
			{
				status = JUMP;
			}
		}
	}
}

bool SwordsMan::isHitByRec(Rectangle attackBox, float damage, bool facingLeft, Vector2 cameraPos)
{
	if (!(currentHealth <= 0)) //Can get hurt only if character isn't dead
	{
		if (CheckCollisionRecs(attackBox, getHitBox(cameraPos)))
		{
			//Protection occurs at the last frame of the protection animation and if they attacker and defender are facing different directions
			if (status == PROTECTION && current.getFrame() == current.getTotalFrames() && this->facingLeft != facingLeft)
			{
				status = PROTECTION;
				current = protection;
				currentHealth -= (damage - damage * blockPercentage);

				//Add Block Effect
				Vector2 blockEffectPos = getSpecialEffectPos();
				if (this->facingLeft) blockEffectPos.x -= getHitBox({0,0}).width / 2;
				else blockEffectPos.x += getHitBox({ 0,0 }).width;
				specialEffects.push_back(new Effect(BLOCK_EFFECT, blockEffectPos, this->facingLeft, this->scale));
			}
			else
			{
				//status = HURT;
				//current = hurt;
				currentHealth -= damage;

				//Add blood effects
				specialEffects.push_back(new Effect(BLOOD_EFFECT, getSpecialEffectPos(), this->facingLeft, this->scale));
			}

			if (currentHealth <= 0)
			{
				currentHealth = 0;
				status = DEATH;
			}

			return true;
		}
	}
	return false;
}

void SwordsMan::unloadTexture() {};