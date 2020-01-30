#include "Enemy0.h"

bool Enemy0::Spawn(float dt)
{
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

void Enemy0::DamagePlayer(Player& player, float dt) const
{
	if (player.GetCirc().IsOverlapping(GetCirc()))
	{
		if (curPhase == Phase::First)
		{
			player.TakeDamage(int(touchDamage * dt));
		}
	}
}

CircF Enemy0::GetCirc() const
{
	return CircF(pos, radius);
}

void Enemy0::Draw(Graphics& gfx) const
{
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
		break;
	case Enemy0::Phase::Dead:
		break;
	}
}
