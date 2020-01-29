#include "Player.h"
#include <algorithm>

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
		b.ClampScreen();
		if (b.Explode(dt))
		{
			exploded = true;
		}
	}
	return exploded;
}

CircF Player::GetCirc() const
{
	return CircF(pos, radius);
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

CircF Player::Bullet::GetCirc() const
{
	return CircF(pos, radius);
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
	pos = playerPos;
	vel = (mousePos - playerPos).GetNormalized() * speed;
	target = mousePos;
	active = true;
}

bool Player::Bomb::Explode(float dt)
{
	if (active && (pos + vel * dt - target).GetLengthSq() > (pos - target).GetLengthSq())
	{
		exploding = true;
		return true;
	}
	return false;
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

CircF Player::Bomb::GetCirc() const
{
	return CircF(pos, radius);
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
		gfx.DrawCircle(circ, Colors::White);
	}
}
