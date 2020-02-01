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
		if (curPhase == Phase::First || curPhase == Phase::Second)
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
	if (curPhase != Phase::Waiting && player.GetCircBullet(index).IsOverlapping(GetCirc()))
	{
		hp -= player.DealBulletDamage(index);
	}
}

void Enemy0::BombHit(Player& player, int index, float dt)
{
	assert(curPhase != Phase::Dead);
	assert(index < player.GetNBombs());
	assert(player.GetExplodingBomb(index));
	if (curPhase != Phase::Waiting && player.GetCircBomb(index).IsOverlapping(GetCirc()))
	{
		hp -= int(player.DealBombDamage(index) * dt);
	}
}

bool Enemy0::FireBasic(float dt)
{
	assert(curPhase != Phase::Dead);
	if (curPhase == Phase::First || curPhase == Phase::Second)
	{
		fireTimeBasic -= dt;
		targetBasic += pi * dt / 2.0f;
		if (fireTimeBasic <= 0.0f)
		{
			const Vec2 targetPos{ std::sin(targetBasic), std::cos(targetBasic) };
			if (curPhase == Phase::First)
			{
				fireTimeBasic = fireRateBasic * (float(hp) / float(hpMax));
			}
			else
			{
				fireTimeBasic = fireRateBasic / 2.0f;
			}
			bulletsBasic[currentBulletBasic].Spawn(targetPos, pos);
			if (currentBulletBasic < nBulletsBasic - 1)
			{
				currentBulletBasic++;
			}
			else
			{
				currentBulletBasic = 0;
			}
			if (curPhase == Phase::Second)
			{
				const int aditFireDirs = 4;
				float aditTargetsPos = targetBasic;
				for (int a = 0; a < aditFireDirs; a++)
				{
					aditTargetsPos += 2.0f * pi / float(aditFireDirs + 1);
					const Vec2 targetPos{ std::sin(aditTargetsPos), std::cos(aditTargetsPos) };
					bulletsBasic[currentBulletBasic].Spawn(targetPos, pos);
					if (currentBulletBasic < nBulletsBasic - 1)
					{
						currentBulletBasic++;
					}
					else
					{
						currentBulletBasic = 0;
					}
				}
			}
			return true;
		}
	}
	return false;
}

void Enemy0::UpdateBulletsBasic(float dt)
{
	assert(curPhase != Phase::Dead);
	if (curPhase == Phase::First || curPhase == Phase::Second)
	{
		for (BulletBasic& b : bulletsBasic)
		{
			b.Move(dt);
			b.ClampScreen();
		}
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

void Enemy0::DrawBulletsBasic(Graphics& gfx) const
{
	assert(curPhase != Phase::Dead);
	if (curPhase == Phase::First || curPhase == Phase::Second)
	{
		for (const BulletBasic& b : bulletsBasic)
		{
			b.Draw(gfx);
		}
	}
}

void Enemy0::BulletBasic::Spawn(const Vec2 & targetPos, const Vec2 & enemy0Pos)
{
	assert(!active);
	pos = enemy0Pos;
	vel = (targetPos) * speed;
	active = true;
}

void Enemy0::BulletBasic::Move(float dt)
{
	if (active)
	{
		pos += vel * dt;
	}
}

void Enemy0::BulletBasic::ClampScreen()
{
	if (active)
	{
		if (pos.x < 399.0f + radius)
		{
			active = false;
		}
		else if (pos.x > float(Graphics::ScreenWidth) - radius)
		{
			active = false;
		}
		if (pos.y < -1.0f + radius)
		{
			active = false;
		}
		else if (pos.y > float(Graphics::ScreenHeight) - radius)
		{
			active = false;
		}
	}
}

CircF Enemy0::BulletBasic::GetCirc() const
{
	return CircF(pos, radius);
}

void Enemy0::BulletBasic::Draw(Graphics & gfx) const
{
	if (active)
	{
		gfx.DrawCircle(GetCirc(), Colors::Red);
	}
}
