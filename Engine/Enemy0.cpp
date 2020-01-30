#include "Enemy0.h"
#include <assert.h>

bool Enemy0::Spawn(float dt)
{
	assert(curPhase != Phase::Dead);
	if (curPhase == Phase::Waiting)
	{
		spawnTimer += dt;
		if (spawnTimer >= 0.0f)
		{
			curPhase = Phase::First;
			return true;
		}
	}
	return false;
}

bool Enemy0::PhaseSwitch()
{
	assert(curPhase != Phase::Dead);
	if (curPhase == Phase::First && hp < hpMax / 2)
	{
		curPhase = Phase::Second;
		return true;
	}
	return false;
}

bool Enemy0::Die()
{
	assert(curPhase != Phase::Dead);
	if (hp <= 0)
	{
		assert(curPhase == Phase::Second);
		curPhase = Phase::Dead;
		return true;
	}
	return false;
}

void Enemy0::DamagePlayer(Player& player, float dt) const
{
	assert(curPhase != Phase::Dead);
	if (player.GetCirc().IsOverlapping(GetCirc()))
	{
		if (curPhase == Phase::First)
		{
			player.TakeDamage(int(touchDamage * dt));
		}
	}
}

void Enemy0::BulletHit(Player& player, int index)
{
	assert(curPhase != Phase::Dead);
	assert(index < player.GetNBullets());
	assert(player.GetActiveBullet(index));
	if (player.GetCircBullet(index).IsOverlapping(GetCirc()))
	{
		hp -= player.DealBulletDamage(index);
	}
}

CircF Enemy0::GetCirc() const
{
	assert(curPhase != Phase::Dead);
	return CircF(pos, radius);
}

void Enemy0::Draw(Graphics& gfx) const
{
	assert(curPhase != Phase::Dead);
	CircF circ = GetCirc();
	const float outline = 12.0f;
	const float padding = 8.0f;
	switch (curPhase)
	{
	case Enemy0::Phase::Waiting:
		gfx.DrawCircOutline(circ, outline, Colors::Gray);
		break;
	case Enemy0::Phase::First:
		gfx.DrawCircOutline(circ, outline, Colors::Red);
		circ.radius -= outline + padding;
		gfx.DrawCircle(circ, Colors::Blue);
		break;
	case Enemy0::Phase::Second:
		gfx.DrawCircOutline(circ, outline, Colors::Red);
		circ.radius -= outline + padding;
		gfx.DrawCircle(circ, Colors::Green);
		break;
	}
}
