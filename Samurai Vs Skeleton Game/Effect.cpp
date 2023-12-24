#include "Effect.h"

AnimationPro Effect::StaticBloodEffectAnimation;
AnimationPro Effect::StaticSpawnEffectAnimation;
AnimationPro Effect::StaticExplosionEffectAnimation;
AnimationPro Effect::StaticBlockEffectAnimation;
AnimationPro Effect::StaticPortalEffectAnimation;

Effect::Effect() {}
Effect::Effect(SpecialEffectEnum specialEffect, Vector2 pos, bool facingLeft, float scale)
	: pos(pos), facingLeft(facingLeft), scale(scale) 
{
	if (specialEffect == PORTAL_EFFECT) animation = StaticPortalEffectAnimation;
	else if (specialEffect == SPAWN_EFFECT) animation = StaticSpawnEffectAnimation;
	else if (specialEffect == EXPLOSION_EFFECT) animation = StaticExplosionEffectAnimation;
	else if (specialEffect == BLOCK_EFFECT) animation = StaticBlockEffectAnimation;
	else if (specialEffect == BLOOD_EFFECT) animation = StaticBloodEffectAnimation;
	animation.reScale(scale);
}

Vector2 Effect::getPos() { return pos; }
void Effect::setPos(Vector2 pos) { this->pos = pos; }
Vector2 Effect::getHitBoxCenterPos()
{
	Rectangle hitBox = getHitBox({ 0,0 });
	return { hitBox.x + hitBox.width / 2, hitBox.y + hitBox.height / 2 };
}
bool Effect::getFacingLeft() { return facingLeft; }
float Effect::getScale() { return scale; }
float Effect::getWidth() { return animation.getWidth(); }
float Effect::getHeight() { return animation.getHeight(); }

Rectangle Effect::getHitBox(Vector2 cameraPos)
{
	return Rectangle{ pos.x - cameraPos.x, pos.y - cameraPos.y, animation.getWidth(), animation.getHeight() };
}
void Effect::drawHitBox(Vector2 cameraPos, Color color) { DrawRectangleRec(getHitBox(cameraPos), color); }

void Effect::draw(Vector2 cameraPos, Color colorTint)
{
	animation.draw({ pos.x - cameraPos.x, pos.y - cameraPos.y }, facingLeft, colorTint);
	animation.updateFrame();
}

bool Effect::finishedAnimation() { return animation.finishedAnimation(); }