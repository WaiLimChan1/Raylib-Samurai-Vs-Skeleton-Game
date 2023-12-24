#include "Character.h"

Character::Character() {}
Character::Character(const char* name, Vector2 pos, float movementSpeed, bool facingLeft, float damage, int health, float scale, float animationSpeed, float attackSpeed, int level)
	: name(name), pos(pos), movementSpeed(movementSpeed), facingLeft(facingLeft), damage(damage), maxHealth(health), currentHealth(health), scale(scale), animationSpeed(animationSpeed), attackSpeed(attackSpeed), level(level)
{
	previousStatus = IDLE;
	lastStatus = IDLE;
	status = IDLE;

	commandStatus = ATTACK_COMMAND;

	inAirVelocity = { 0,0 };

	completelyDead = false;
}

const char* Character::getName() { return name;  }
StatusEnum Character::getStatus() { return status; }
CommandStatusEnum Character::getCommandStatus() { return commandStatus; }
std::vector<Effect*>& Character::getSpecialEffects() { return specialEffects; }
Vector2 Character::getPos() { return pos; }
float Character::getWidth() { return current.getWidth(); }
float Character::getHeight() { return current.getHeight(); }
bool Character::getFacingLeft() { return facingLeft; }
int Character::getCurrentHealth() { return currentHealth; }
bool Character::getCompletelyDead() { return completelyDead; }
float Character::getScale() { return scale; }
int Character::getLevel() { return level; }

Vector2 Character::getHitBoxCenterPos() 
{
	Rectangle hitBox = getHitBox({0,0});
	return { hitBox.x + hitBox.width / 2, hitBox.y + hitBox.height / 2 };
}
Vector2 Character::getSpecialEffectPos()
{
	return { pos.x + getWidth() / 3, pos.y + getHeight() / 2};
}
Vector2 Character::getSyncCameraPos()
{
	//return { pos.x + getWidth() / 2 - SCREEN_WIDTH / 2, pos.y + getHeight() / 2 - SCREEN_HEIGHT / 2 }; //Character at the center of SCREEN
	//return { pos.x + getWidth() / 2 - SCREEN_WIDTH / 2, pos.y - SCREEN_HEIGHT / 2}; //Character at the bottom of the SCREEN
	return { pos.x + getWidth() / 2 - SCREEN_WIDTH / 2, pos.y + getHeight() / 2 - SCREEN_HEIGHT / 2 };
}

void Character::setPos(Vector2 pos) { this->pos = pos; }
void Character::setInAirVelocity(Vector2 inAirVelocity) { this->inAirVelocity = inAirVelocity; }

//Print name and health
void Character::printCharacterInfo()
{
	int posX = 10;
	int posY = 10;
	int fontSize = 30;
	Color color = YELLOW;

	int offsetY = fontSize;

	char message[100];

	strcpy_s(message, "Name: ");
	strcat_s(message, name);
	strcat_s(message, " (LVL ");
	strcat_s(message, std::to_string(level + 1).c_str());
	strcat_s(message, ")");
	DrawText(message, posX, posY, fontSize, color);
	posY += offsetY;

	strcpy_s(message, "Health: ");
	strcat_s(message, std::to_string(currentHealth).c_str());
	DrawText(message, posX, posY, fontSize, color);
	posY += offsetY;

	strcpy_s(message, "Command: ");
	switch (commandStatus)
	{
	case NONE_COMMAND:
		strcat_s(message, "None");
		break;
	case RANDOM_COMMAND:
		strcat_s(message, "Random");
		break;
	case HOLD_COMMAND:
		strcat_s(message, "Hold");
		break;
	case ATTACK_COMMAND:
		strcat_s(message, "Attack");
		break;
	case CHARGE_COMMAND:
		strcat_s(message, "Charge");
		break;
	}
	DrawText(message, posX, posY, fontSize, color);
	posY += offsetY;
}

//Draw an indicators that indicates this Character is selected
void Character::drawTargetIndicator(Vector2 cameraPos, float radius, Color color)
{
	//Draw target indicator
	float indicatorX = pos.x + getWidth() / 2;
	float indicatorY = pos.y + getHeight() / 3;
	DrawCircle(indicatorX - cameraPos.x, indicatorY - cameraPos.y, radius, color);
}

//Draw character's health bar
void Character::drawHealthBar(Vector2 cameraPos, float size, Color healthColor, Color borderColor)
{
	float healthBarWidth = size;
	float healthBarHeight = size / 20.0;
	float healthBarX = pos.x + getWidth() / 2.0 - healthBarWidth / 2.0;
	float healthBarY = pos.y + getHeight() / 3.0 - healthBarHeight / 2.0 + healthBarHeight * 4;

	//Draw border
	float borderBuffer = size / 20.0;
	DrawRectangle(healthBarX - cameraPos.x - borderBuffer, healthBarY - cameraPos.y - borderBuffer, healthBarWidth + borderBuffer * 2, healthBarHeight + borderBuffer * 2, borderColor);
	DrawRectangle(healthBarX - cameraPos.x, healthBarY - cameraPos.y, healthBarWidth * (float) currentHealth / (float) maxHealth, healthBarHeight, healthColor);
}

Rectangle Character::getHitBox(Vector2 cameraPos)
{
	float hitBoxWidth = getWidth() / 4;
	float hitBoxHeight = getHeight() * 2 / 3;
	float hitBoxPosX = pos.x - cameraPos.x + hitBoxWidth / 2 + hitBoxWidth;
	float hitBoxPosY = pos.y - cameraPos.y + hitBoxHeight / 2;
	return Rectangle{ hitBoxPosX, hitBoxPosY, hitBoxWidth, hitBoxHeight };
}
void Character::drawHitBox(Vector2 cameraPos, Color color) { DrawRectangleRec(getHitBox(cameraPos), color); }

Rectangle Character::getRawAttackBox(Vector2 cameraPos)
{
	/*float attackBoxWidth = getWidth() / 2;
	float attackBoxHeight = getHeight() * 2 / 3;
	float attackBoxPosX = pos.x - cameraPos.x;
	float attackBoxPosY = pos.y - cameraPos.y + attackBoxHeight / 2;

	if (!facingLeft)
		attackBoxPosX += attackBoxWidth;*/

	float attackBoxWidth = getWidth() / 3;
	float attackBoxHeight = getHeight() * 2 / 3;
	float attackBoxPosX = pos.x - cameraPos.x;
	float attackBoxPosY = pos.y - cameraPos.y + attackBoxHeight / 2;

	if (facingLeft) attackBoxPosX += attackBoxWidth * 1 / 2;
	else attackBoxPosX += attackBoxWidth * 3 / 2;
	return Rectangle{ attackBoxPosX, attackBoxPosY, attackBoxWidth, attackBoxHeight };
}
Rectangle Character::getAttackBox(Vector2 cameraPos) //Get the attack box only if it is active
{
	if (attackBoxesAreActive())
	{
		return getRawAttackBox(cameraPos);
	}
	return Rectangle();
}
void Character::drawAttackbox(Vector2 cameraPos, Color color) { DrawRectangleRec(getAttackBox(cameraPos), color); }

void Character::draw(Vector2 cameraPos, Color tintColor, Color hitColor)
{
	if (status == HURT) current.draw({ pos.x - cameraPos.x, pos.y - cameraPos.y }, facingLeft, hitColor);
	else current.draw({ pos.x - cameraPos.x, pos.y - cameraPos.y }, facingLeft, tintColor);

	if (!(status == PROTECTION && current.finishedAnimation()) //Don't update frame if it is at the last frame of protection animation
		&& !(inAir() && status == JUMP && current.getFrame() == current.getTotalFrames() - 2) //Don't update frame if it is at a jump in midair and 2 frames before completing
		&& !completelyDead) //Don't update frame if it is completely dead
		current.updateFrame();

	if (status == DEATH && current.finishedAnimation())
		completelyDead = true;

	endPlayOnceAnimations();
}

//Stop animations that only play once
void Character::endPlayOnceAnimations()
{
	if (current.finishedAnimation())
		if (status == ATTACK1 || status == ATTACK2 || status == ATTACK3 || status == SHOOT1 || status == SHOOT2 || status == RUNATTACK ||(status == JUMP && !inAir()) || status == HURT)
			status = IDLE;
}

void Character::endPreviousControl()
{
	if (status == WALK || status == RUN || status == PROTECTION)
		status = IDLE;
}

void Character::takeInputForCommandStatus()
{
	if (IsKeyPressed(KEY_ONE)) commandStatus = NONE_COMMAND;
	else if (IsKeyPressed(KEY_TWO)) commandStatus = RANDOM_COMMAND;
	else if (IsKeyPressed(KEY_THREE)) commandStatus = HOLD_COMMAND;
	else if (IsKeyPressed(KEY_FOUR)) commandStatus = ATTACK_COMMAND;
	else if (IsKeyPressed(KEY_FIVE)) commandStatus = CHARGE_COMMAND;
}

void Character::getInputBasedOnCommandStatus(Rectangle portalHitBox, std::vector<Character*>& enemyCharacters)
{
	switch (commandStatus)
	{
	case NONE_COMMAND:
		break;
	case RANDOM_COMMAND:
		getCommandInputRandom();
		break;
	case HOLD_COMMAND:
		getCommandInputHold(portalHitBox, enemyCharacters);
		break;
	case ATTACK_COMMAND:
		getCommandInputAttack(portalHitBox,enemyCharacters);
		break;
	case CHARGE_COMMAND:
		getCommandInputCharge(portalHitBox, enemyCharacters);
		break;
	}
}

Vector2 Character::findClosestEnemyPos(Rectangle portalHitBox, std::vector<Character*>& enemyCharacters)
{
	Vector2 result = { portalHitBox.x + portalHitBox.width / 2, portalHitBox.y = portalHitBox.height / 2};
	float smallestDistance = abs(getHitBoxCenterPos().x - result.x);
	for (auto& enemyCharacter : enemyCharacters)
	{
		if (smallestDistance > abs(getHitBoxCenterPos().x - enemyCharacter->getHitBoxCenterPos().x))
		{
			result = enemyCharacter->getHitBoxCenterPos();
			smallestDistance = abs(getHitBoxCenterPos().x - enemyCharacter->getHitBoxCenterPos().x);
		}
	}
	return result;
}

void Character::attackIfPossible(Rectangle portalHitBox, std::vector<Character*>& enemyCharacters)
{
	bool hit = false;
	if (CheckCollisionRecs(getRawAttackBox({ 0,0 }), portalHitBox))
		hit = true;

	if (!hit)
		for (auto& enemy : enemyCharacters)
		{
			if (CheckCollisionRecs(getRawAttackBox({ 0,0 }), enemy->getHitBox({ 0,0 })))
			{
				hit = true;
				break;
			}
		}

	if (hit)
	{
		int randNum = rand() % 3;
		if (randNum == 0) status = ATTACK1;
		else if (randNum == 1) status = ATTACK2;
		else if (randNum == 2) status = ATTACK3;
	}
}

bool Character::isHitByRec(Rectangle attackBox, float damage, bool facingLeft, Vector2 cameraPos)
{
	if (!(currentHealth <= 0)) //Can get hurt only if character isn't dead
	{
		if (CheckCollisionRecs(attackBox, getHitBox(cameraPos)))
		{
			//status = HURT;
			//current = hurt;
			currentHealth -= damage;

			//Add blood effects
			specialEffects.push_back(new Effect(BLOOD_EFFECT, getSpecialEffectPos(), this->facingLeft, this->scale));

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

void Character::changeAnimation()
{
	if (lastStatus != status)
	{
		previousStatus = lastStatus;
		if (status == IDLE) current = idle;
		if (status == WALK) current = walk;
		if (status == RUN) current = run;
		if (status == JUMP) current = jump;
		if (status == ATTACK1) current = attack1;
		if (status == ATTACK2) current = attack2;
		if (status == ATTACK3) current = attack3;
		if (status == SHOOT1) current = shoot1;
		if (status == SHOOT2) current = shoot2;
		if (status == RUNATTACK) current = runAttack;
		if (status == PROTECTION) current = protection;
		if (status == DEATH) current = death;
	}
	lastStatus = status;
}

bool Character::inAir()
{
	return (pos.y < SCREEN_HEIGHT - getHeight());
}

void Character::updatePosition()
{
	float dt = GetFrameTime();
	if (status == WALK)
	{
		if (facingLeft) pos.x -= movementSpeed * dt;
		else pos.x += movementSpeed * dt;
	}
	if (status == RUN || status == RUNATTACK)
	{
		if (facingLeft) pos.x -= 2 * movementSpeed * dt;
		else pos.x += 2 * movementSpeed * dt;
	}

	//Give it some initial velocity for JUMP
	if (status == JUMP && current.getFrame() == 4 && current.startedCurrentFrame() && inAirVelocity.x == 0 && inAirVelocity.y == 0)
	{
		//Bonus initial velocity in the x direction
		float velocityXMagnitude = movementSpeed / 3;
		if (previousStatus == WALK) velocityXMagnitude = movementSpeed;
		else if (previousStatus == RUN) velocityXMagnitude = 2 * movementSpeed;

		if (facingLeft) inAirVelocity.x -= velocityXMagnitude;
		else inAirVelocity.x += velocityXMagnitude;

		inAirVelocity.y = -2 * movementSpeed;
	}

	pos.y += inAirVelocity.y * dt;

	if (inAir())
	{
		//Change to falling animation
		if (status != JUMP && currentHealth > 0 && jump.getTexture().width != 0)
		{
			lastStatus = JUMP;
			status = JUMP;
			current = jump;
			current.setFrame(current.getTotalFrames() - 2);
		}

		pos.x += inAirVelocity.x * dt;
		inAirVelocity.y += GRAVITY * dt;
	}
}

void Character::checkBound(Vector2 horizontalBound)
{
	//Touching the ground
	if (pos.y >= SCREEN_HEIGHT - current.getHeight())
	{
		pos.y = SCREEN_HEIGHT - current.getHeight();
		inAirVelocity = { 0,0 };
	}

	//Touching side bounds. Using hitbox as the deciding factor for whether or not the character is out of bound
	Rectangle hitBox = getHitBox({0,0});

	if (hitBox.x < horizontalBound.x) pos.x = horizontalBound.x - (hitBox.x - pos.x);
	if (hitBox.x > horizontalBound.y - hitBox.width) pos.x = horizontalBound.y - (hitBox.x - pos.x) - hitBox.width;

}