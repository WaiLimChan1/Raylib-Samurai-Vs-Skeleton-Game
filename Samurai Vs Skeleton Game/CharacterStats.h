#pragma once
#include "Header.h"

class CharacterStats
{
public:
	std::vector<float> movementSpeedByLevel = {300, 350, 400, 450, 500};
	std::vector<float> damageByLevel = { 10, 20, 30, 50, 100 };
	std::vector<float> healthByLevel = { 100, 200, 300, 500, 800 };
	std::vector<float> scaleByLevel = { 2.3f, 2.5f, 2.7f, 3.0f, 3.5f };
	std::vector<float> attackSpeedByLevel = { 0.8f, 1.0f, 1.2f, 1.3f, 1.5f };
	std::vector<float> blockPercentageByLevel = { 0.5f, 0.6f, 0.7f, 0.8f, 1.0f };
	std::vector<float> fireSpeedByLevel = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };

	std::vector<float> upgradeCostByLevel = { 25, 50, 100, 200, 0 };
	std::vector<float> purchasePriceByLevel = { 5, 10, 20, 30, 100 };

	int maxLevel = 4;
	int level = 0;

	float movementSpeed;
	float damage;
	float health;
	float scale;
	float attackSpeed;
	float blockPercentage;
	float fireSpeed;

public:

	CharacterStats* setLevel(int level)
	{
		this->level = level;
		if (this->level < 0) this->level = 0;
		else if (this->level > maxLevel) this->level = maxLevel;

		movementSpeed = movementSpeedByLevel.at(this->level);
		damage = damageByLevel.at(this->level);
		health = healthByLevel.at(this->level);
		scale = scaleByLevel.at(this->level);
		attackSpeed = attackSpeedByLevel.at(this->level);
		blockPercentage = blockPercentageByLevel.at(this->level);
		fireSpeed = fireSpeedByLevel.at(this->level);

		return this;
	}

	int getLevel() { return level; }
};