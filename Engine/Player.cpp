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

bool Player::Fire(const Vec2& mousePos, float dt, bool fire)
{
	fireTime -= dt;
	if (fireTime <= 0.0f && fire)
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

CircF Player::GetCirc() const
{
	return CircF(pos, radius);
}

void Player::Draw(Graphics& gfx) const
{
	int blue = std::max(hp, 0) * 255 / hpMax;
	gfx.DrawCircle(GetCirc(), Color{ 255, 0, unsigned char(std::max(hp, 0) * 255 / hpMax) });
}

void Player::DrawBullets(Graphics& gfx)
{
	for (Bullet& b : bullets)
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
		gfx.DrawCircle(GetCirc(), Color{ 200, 0, 255 });
	}
}
