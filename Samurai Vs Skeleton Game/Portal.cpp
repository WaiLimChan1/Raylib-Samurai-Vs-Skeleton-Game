#include "Portal.h"

Portal::Portal() {}
Portal::Portal(int health, Vector2 pos, bool facingLeft, float scale)
	: currentHealth(health), maxHealth(health)
{
	portalEffect = Effect(PORTAL_EFFECT, pos, facingLeft, scale);
	
	destroyed = false;

	SamuraiLevel = 0;
	SamuraiCommanderLevel = 0;
	SamuraiArcherLevel = 0;

	SkeletonWarriorLevel = 0;
	SkeletonSpearmanLevel = 0;
	SkeletonArcherLevel = 0;
}

Effect& Portal::getPortalEffect() { return portalEffect; }
float Portal::getUpgradePrice(CharacterENUM characterEnum) { return characterStats.upgradeCostByLevel.at(getLevel(characterEnum)); }
float Portal::getPurchasePrice(CharacterENUM characterEnum) { return characterStats.purchasePriceByLevel.at(getLevel(characterEnum)); }
std::vector<Effect*>& Portal::getSpecialEffects() { return specialEffects; }
int Portal::getLevel(CharacterENUM characterEnum)
{
	switch (characterEnum)
	{
	case SAMURAI:
		return SamuraiLevel;
	case SAMURAI_COMMANDER:
		return SamuraiCommanderLevel;
	case SAMURAI_ARCHER:
		return SamuraiArcherLevel;
	case SKELETON_WARRIOR:
		return SkeletonWarriorLevel;
	case SKELETON_SPEARMAN:
		return SkeletonSpearmanLevel;
	case SKELETON_ARCHER:
		return SkeletonArcherLevel;
	default:
		return -1;
	}
}
Vector2 Portal::getHitBoxCenterPos()
{
	Rectangle hitBox = getHitBox({ 0,0 });
	return { hitBox.x + hitBox.width / 2, hitBox.y + hitBox.height / 2 };
}
int Portal::getCurrentHealth() { return currentHealth; }
int Portal::getMaxHealth() { return maxHealth; }

void Portal::setCurrentHealth(int currentHealth) { this->currentHealth = currentHealth; }
void Portal::setMaxHealth(int maxHealth) { this->maxHealth = maxHealth; }

void Portal::setLevel(CharacterENUM characterEnum, int level)
{
	switch (characterEnum)
	{
	case SAMURAI:
		SamuraiLevel = level;
		break;
	case SAMURAI_COMMANDER:
		SamuraiCommanderLevel = level;
		break;
	case SAMURAI_ARCHER:
		SamuraiArcherLevel = level;
		break;
	case SKELETON_WARRIOR:
		SkeletonWarriorLevel = level;
		break;
	case SKELETON_SPEARMAN:
		SkeletonSpearmanLevel = level;
		break;
	case SKELETON_ARCHER:
		SkeletonArcherLevel = level;
		break;
	}
}

bool Portal::upgrade(CharacterENUM characterEnum)
{
	int characterLevel = getLevel(characterEnum);

	if (characterLevel < characterStats.maxLevel)
	{
		setLevel(characterEnum, characterLevel + 1);
		return true;
	}
	return false;
}

bool Portal::upgrade(CharacterENUM characterEnum, int& money)
{
	int characterLevel = getLevel(characterEnum);

	if (characterLevel < characterStats.maxLevel)
	{
		if (money >= characterStats.upgradeCostByLevel.at(characterLevel))
		{
			money -= characterStats.upgradeCostByLevel.at(characterLevel);
			setLevel(characterEnum, characterLevel + 1);
			return true;
		}
	}
	return false;
}

Rectangle Portal::getHitBox(Vector2 cameraPos)
{
	Rectangle hitBox = portalEffect.getHitBox(cameraPos);
	hitBox.x += hitBox.width * 1.0 / 4.0;
	hitBox.width *= 2.0 / 4.0;
	return hitBox;
}
void Portal::drawHitBox(Vector2 cameraPos, Color color) { DrawRectangleRec(getHitBox(cameraPos), color); }

//Draw portal's health bar
void Portal::drawHealthBar(Vector2 cameraPos, float size, Color healthColor, Color borderColor)
{
	float healthBarWidth = size;
	float healthBarHeight = size / 20.0;
	float healthBarX = portalEffect.getPos().x + portalEffect.getWidth() / 2.0 - healthBarWidth / 2.0;
	float healthBarY = portalEffect.getPos().y;

	//Draw border
	float borderBuffer = size / 20.0;
	DrawRectangle(healthBarX - cameraPos.x - borderBuffer, healthBarY - cameraPos.y - borderBuffer, healthBarWidth + borderBuffer * 2, healthBarHeight + borderBuffer * 2, borderColor);
	DrawRectangle(healthBarX - cameraPos.x, healthBarY - cameraPos.y, healthBarWidth * (float)currentHealth / (float)maxHealth, healthBarHeight, healthColor);
}
void Portal::draw(Vector2 cameraPos, Color colorTint) 
{ 
	portalEffect.draw(cameraPos, colorTint); 
}

bool Portal::hasEnoughMoneyForSpawningCharacter(CharacterENUM characterEnum, int& money, bool buying)
{
	int characterLevel = getLevel(characterEnum);

	if (money >= characterStats.purchasePriceByLevel.at(characterLevel))
	{
		if (buying) 
			money -= characterStats.purchasePriceByLevel.at(characterLevel);
		return true;
	}
	return false;
}

Character* Portal::createCharacter(CharacterENUM characterEnum, const char* name, float animationSpeed)
{
	Character* character = nullptr;
	Vector2 initialPosition = portalEffect.getPos();
	bool facingLeft = portalEffect.getFacingLeft();

	switch (characterEnum)
	{
	case SAMURAI:
		character = new Samurai(name, initialPosition, facingLeft, animationSpeed, characterStats.setLevel(SamuraiLevel));
		break;
	case SAMURAI_COMMANDER:
		character = new SamuraiCommander(name, initialPosition, facingLeft, animationSpeed, characterStats.setLevel(SamuraiCommanderLevel));
		break;
	case SAMURAI_ARCHER:
		character = new SamuraiArcher(name, initialPosition, facingLeft, animationSpeed, characterStats.setLevel(SamuraiArcherLevel));
		break;
	case SKELETON_WARRIOR:
		character = new SkeletonWarrior(name, initialPosition, facingLeft, animationSpeed, characterStats.setLevel(SkeletonWarriorLevel));
		break;
	case SKELETON_SPEARMAN:
		character = new SkeletonSpearman(name, initialPosition, facingLeft, animationSpeed, characterStats.setLevel(SkeletonSpearmanLevel));
		break;
	case SKELETON_ARCHER:
		character = new SkeletonArcher("Skeleton Archer", initialPosition, facingLeft, animationSpeed, characterStats.setLevel(SkeletonArcherLevel));
		break;
	}

	//Properly position the new character by matching up the centerHitBoxPos of the portal and the newly created character
	Vector2 shift = Vector2Subtract(portalEffect.getHitBoxCenterPos(), character->getHitBoxCenterPos());
	character->setPos({ character->getPos().x + shift.x, character->getPos().y + shift.y });

	//Add Spawn Effect
	specialEffects.push_back(new Effect(SPAWN_EFFECT, initialPosition, facingLeft, characterStats.scale * 3));

	return character;
}

Character* Portal::createBoss(CharacterENUM characterEnum, const char* name, float animationSpeed, CharacterStats* characterStats)
{
	Character* character = nullptr;
	Vector2 initialPosition = portalEffect.getPos();
	bool facingLeft = portalEffect.getFacingLeft();

	switch (characterEnum)
	{
	case SAMURAI:
		character = new Samurai(name, initialPosition, facingLeft, animationSpeed, characterStats);
		break;
	case SAMURAI_COMMANDER:
		character = new SamuraiCommander(name, initialPosition, facingLeft, animationSpeed, characterStats);
		break;
	case SAMURAI_ARCHER:
		character = new SamuraiArcher(name, initialPosition, facingLeft, animationSpeed, characterStats);
		break;
	case SKELETON_WARRIOR:
		character = new SkeletonWarrior(name, initialPosition, facingLeft, animationSpeed, characterStats);
		break;
	case SKELETON_SPEARMAN:
		character = new SkeletonSpearman(name, initialPosition, facingLeft, animationSpeed, characterStats);
		break;
	case SKELETON_ARCHER:
		character = new SkeletonArcher(name, initialPosition, facingLeft, animationSpeed, characterStats);
		break;
	}

	//Properly position the new character by matching up the centerHitBoxPos of the portal and the newly created character
	Vector2 shift = Vector2Subtract(portalEffect.getHitBoxCenterPos(), character->getHitBoxCenterPos());
	character->setPos({ character->getPos().x + shift.x, character->getPos().y + shift.y });

	//Add Spawn Effect
	specialEffects.push_back(new Effect(SPAWN_EFFECT, initialPosition, facingLeft, characterStats->scale * 3));

	return character;
}

bool Portal::isHitByRec(Rectangle attackBox, float damage, Vector2 cameraPos)
{
	if (!(currentHealth <= 0)) //Can get hurt only if portal isn't dead
	{
		if (CheckCollisionRecs(attackBox, getHitBox(cameraPos)))
		{
			currentHealth -= damage;
			if (currentHealth <= 0)
			{
				currentHealth = 0;
				destroyed = true;
				specialEffects.push_back(new Effect(EXPLOSION_EFFECT, portalEffect.getPos(), portalEffect.getFacingLeft(), portalEffect.getScale() / 12));
			}
			return true;
		}
	}
	return false;
}