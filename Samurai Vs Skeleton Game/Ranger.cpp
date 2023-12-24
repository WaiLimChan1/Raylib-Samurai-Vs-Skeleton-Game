#include "Ranger.h"

Ranger::Ranger() {}
Ranger::Ranger(const char* name, Vector2 pos, float movementSpeed, bool facingLeft, float damage, int health, float scale, float animationSpeed, float attackSpeed, int level, float fireSpeed)
	: Character(name, pos, movementSpeed, facingLeft, damage, health, scale, animationSpeed, attackSpeed, level), fireSpeed(fireSpeed) 
{
	this->maxHealth /= 2;
	this->currentHealth /= 2;
	this->damage /= 5;
}