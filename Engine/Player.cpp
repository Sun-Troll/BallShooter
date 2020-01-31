#include "Player.h"
#include <algorithm>
#include <assert.h>

void Player::Move(float dt, bool left, bool right, bool up, bool down)
{
	Vec2 moveDir{ 0.0f, 0.0f };
	if (left)
	{
		moveDir.x = -1.0f;
	}
	else if (right)
	{
		moveDir.x = 1.0f;
	}
	if (up)
	{
		moveDir.y = -1.0f;
	}
	else if (down)
	{
		moveDir.y = 1.0f;
	}
	pos += moveDir.GetNormalized() * speed * dt;
}

void Player::ClampScreen()
{
	if (pos.x < 399.0f + radius)
	{
		pos.x = 399.0f + radius;
	}
	else if (pos.x > float(Graphics::ScreenWidth) - radius)
	{
		pos.x = float(Graphics::ScreenWidth) - radius;
	}
	if (pos.y < -1.0f + radius)
	{
		pos.y = -1.0f + radius;
	}
	else if (pos.y > float(Graphics::ScreenHeight) - radius)
	{
		pos.y = float(Graphics::ScreenHeight) - radius;
	}
}

bool Player::SlowDown(float dt, bool slow)
{
	if (slow && slowRechRem <= 0.0f)
	{
		isSlowed = true;
		slowRechRem = slowRech;
		slowDurRem = slowDur;
	}
	if (isSlowed && slowDurRem > 0.0f)
	{
		slowDurRem -= dt;
	}
	else
	{
		isSlowed = false;
		slowRechRem -= dt;
	}
	return isSlowed;
}

bool Player::Fire(const Vec2& mousePos, float dt, bool moving)
{
	fireTime -= dt;
	if (!moving)
	{
		fireTime -= dt;
	}
	
	if (fireTime <= 0.0f)
	{
		fireTime = fireRate;
		bullets[currentBullet].Spawn(mousePos, pos);
		if (currentBullet < nBullets - 1)
		{
			currentBullet++;
		}
		else
		{
			currentBullet = 0;
		}
		return true;
	}
	return false;
}

void Player::UpdateBullets(float dt)
{
	for (Bullet& b : bullets)
	{
		b.Move(dt);
		b.ClampScreen();
	}
}

Player::TeleportState Player::TeleportFirePort(const Vec2 & mousePos, float dt, bool fire)
{
	const bool telActive = teleport.GetActive();
	if (!telActive)
	{
		telRechCur -= dt;
	}
	if (fire)
	{
		if (telActive)
		{
			pos = teleport.GetPos();
			teleport.Port();
			return TeleportState::Port;
		}
		else if (telRechCur <= 0.0f)
		{
			telRechCur = telRech;
			teleport.Spawn(mousePos, pos);
			return TeleportState::Fire;
		}
	}
	return TeleportState::Nothing;
}

void Player::UpdateTelleport(float dt)
{
	teleport.Move(dt);
	teleport.ClampScreen();
}

bool Player::BombFire(const Vec2& mousePos, float dt, bool fire)
{
	assert(currentBomb < nBombs);
	assert(bombRechCur <= bombRech);
	bombRechCur -= dt;

	if (fire && bombRechCur <= 0.0f)
	{
		bombRechCur = bombRech;
		bombs[currentBomb].Spawn(mousePos, pos);
		if (currentBomb < nBombs - 1)
		{
			currentBomb++;
		}
		else
		{
			currentBomb = 0;
		}
		return true;
	}
	return false;
}

bool Player::BombUpdate(float dt)
{
	bool exploded = false;
	for (Bomb& b : bombs)
	{
		b.Move(dt);
		if (b.Explode(dt))
		{
			exploded = true;
		}
		b.Exploding(dt);
		b.ClampScreen();
	}
	return exploded;
}

CircF Player::GetCirc() const
{
	return CircF(pos, radius);
}

int Player::GetNBullets() const
{
	return nBullets;
}

bool Player::GetActiveBullet(int index) const
{
	assert(index < nBullets);
	return bullets[index].GetActive();
}

int Player::DealBulletDamage(int index)
{
	assert(index < nBullets);
	assert(bullets[index].GetActive());
	return bullets[index].Hit();
}

CircF Player::GetCircBullet(int index) const
{
	return CircF(bullets[index].GetCirc());
}

int Player::GetNBombs() const
{
	return nBombs;
}

bool Player::GetExplodingBomb(int index) const
{
	assert(index < nBombs);
	return bombs[index].GetExploding();
}

float Player::DealBombDamage(int index) const
{
	assert(index < nBombs);
	assert(bombs[index].GetExploding());
	return bombs[index].GetDamage();
}

CircF Player::GetCircBomb(int index) const
{
	return CircF(bombs[index].GetCirc());
}

bool Player::isDead() const
{
	return hp <= 0;
}

void Player::TakeDamage(int damage)
{
	hp -= damage;
}

void Player::Draw(Graphics& gfx) const
{
	const unsigned char color = (std::max(hp, 0) * 255 / hpMax);
	const float outline = 3.0f;
	CircF circ = GetCirc();
	gfx.DrawCircOutline(circ, outline, Colors::White);
	circ.radius -= outline + 2.0f;
	gfx.DrawCircOutline(circ, outline, Color{ color, color, color });
	circ.radius -= outline + 2.0f;
	gfx.DrawCircle(circ, Colors::White);
}

void Player::DrawBullets(Graphics& gfx) const
{
	for (const Bullet& b : bullets)
	{
		b.Draw(gfx);
	}
}

void Player::DrawTeleport(Graphics& gfx) const
{
	teleport.Draw(gfx);
}

void Player::DrawBombs(Graphics& gfx) const
{
	for (const Bomb& b : bombs)
	{
		b.Draw(gfx);
	}
}

void Player::Bullet::Spawn(const Vec2& mousePos, const Vec2& playerPos)
{
	assert(!active);
	pos = playerPos;
	vel = (mousePos - playerPos).GetNormalized() * speed;
	active = true;
}

void Player::Bullet::Move(float dt)
{
	if (active)
	{
		pos += vel * dt;
	}
}

void Player::Bullet::ClampScreen()
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

int Player::Bullet::Hit()
{
	assert(active);
	active = false;
	return damage;
}

CircF Player::Bullet::GetCirc() const
{
	return CircF(pos, radius);
}

bool Player::Bullet::GetActive() const
{
	return active;
}

void Player::Bullet::Draw(Graphics& gfx) const
{
	if (active)
	{
		gfx.DrawCircle(GetCirc(), Color{ 255, 255, 255 });
	}
}

void Player::Teleport::Spawn(const Vec2& mousePos, const Vec2& playerPos)
{
	assert(!active);
	pos = playerPos;
	vel = (mousePos - playerPos).GetNormalized() * speed;
	active = true;
}

void Player::Teleport::Port()
{
	active = false;
}

void Player::Teleport::Move(float dt)
{
	if (active)
	{
		pos += vel * dt;
	}
}

void Player::Teleport::ClampScreen()
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

CircF Player::Teleport::GetCirc() const
{
	return CircF(pos, radius);
}

bool Player::Teleport::GetActive() const
{
	return active;
}

Vec2 Player::Teleport::GetPos() const
{
	return Vec2(pos);
}

void Player::Teleport::Draw(Graphics& gfx) const
{
	if (active)
	{
		const float outline = 3.0f;
		CircF circ = GetCirc();
		gfx.DrawCircOutline(circ, outline, Colors::White);
		circ.radius -= 2.0f * (outline + 2.0f);
		gfx.DrawCircle(circ, Colors::White);
	}
}

void Player::Bomb::Spawn(const Vec2& mousePos, const Vec2& playerPos)
{
	assert(!active);
	assert(!exploding);
	assert(radius == radiusBase);
	pos = playerPos;
	vel = (mousePos - playerPos).GetNormalized() * speed;
	target = mousePos;
	active = true;
}

bool Player::Bomb::Explode(float dt)
{
	if (active && (pos + vel * dt - target).GetLengthSq() > (pos - target).GetLengthSq() && !exploding)
	{
		exploding = true;
		return true;
	}
	return false;
}

void Player::Bomb::Exploding(float dt)
{
	if (active && exploding)
	{
		if (radius < 150.0f)
		{
			radius += dt * 100.0f;
		}
		else
		{
			active = false;
			exploding = false;
			radius = radiusBase;
		}
	}
}

void Player::Bomb::Move(float dt)
{
	if (active && !exploding)
	{
		pos += vel * dt;
	}
}

void Player::Bomb::ClampScreen()
{
	if (active)
	{
		if (pos.x < 399.0f + radius)
		{
			active = false;
			exploding = false;
			radius = radiusBase;
		}
		else if (pos.x > float(Graphics::ScreenWidth) - radius)
		{
			active = false;
			exploding = false;
			radius = radiusBase;
		}
		if (pos.y < -1.0f + radius)
		{
			active = false;
			exploding = false;
			radius = radiusBase;
		}
		else if (pos.y > float(Graphics::ScreenHeight) - radius)
		{
			active = false;
			exploding = false;
			radius = radiusBase;
		}
	}
}

CircF Player::Bomb::GetCirc() const
{
	return CircF(pos, radius);
}

bool Player::Bomb::GetExploding() const
{
	return exploding;
}

float Player::Bomb::GetDamage() const
{
	assert(exploding);
	return damage;
}

void Player::Bomb::Draw(Graphics& gfx) const
{
	if (active && !exploding)
	{
		const float outline = 3.0f;
		CircF circ = GetCirc();
		gfx.DrawCircOutline(circ, outline, Colors::White);
		circ.radius -= outline + 2.0f;
		gfx.DrawCircOutline(circ, outline, Colors::White);
	}
	else if (active && exploding)
	{
		CircF circ = GetCirc();
		const float outline = 3.0f;
		do
		{
			gfx.DrawCircOutline(circ, outline, Colors::White);
			circ.radius -= 3.0f * (outline + 2.0f);
		} while (circ.radius > radiusBase);
		circ.radius = radiusBase;
		gfx.DrawCircle(circ, Colors::White);
	}
}
