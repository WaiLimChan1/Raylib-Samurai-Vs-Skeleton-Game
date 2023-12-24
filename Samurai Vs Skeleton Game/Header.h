#pragma once
#include <raylib.h>
#include "raymath.h"
#include <cstdlib>
#include <time.h>
#include <vector>
#include <string>

//For testing
#include <iostream>

//Global Const Variables
//---------------------------------------------------------------------
const int SCREEN_WIDTH = 1500;
const int SCREEN_HEIGHT = 900;
const float GRAVITY = 1500;
//---------------------------------------------------------------------

enum SelectionModeENUM {NONE, SINGLE, MULTIPLE};

//Characters
enum CharacterENUM { SAMURAI, SAMURAI_COMMANDER, SAMURAI_ARCHER,
	                 SKELETON_WARRIOR, SKELETON_SPEARMAN, SKELETON_ARCHER};

//Character Status
enum StatusEnum { IDLE, WALK, RUN, JUMP, ATTACK1, ATTACK2, ATTACK3, SHOOT1, SHOOT2, RUNATTACK, PROTECTION, HURT, DEATH };

//Character Command Status
enum CommandStatusEnum {NONE_COMMAND, RANDOM_COMMAND, HOLD_COMMAND, ATTACK_COMMAND, CHARGE_COMMAND};

//Special Effects
enum SpecialEffectEnum { PORTAL_EFFECT, SPAWN_EFFECT, EXPLOSION_EFFECT, BLOOD_EFFECT, BLOCK_EFFECT };