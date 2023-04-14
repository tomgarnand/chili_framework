#include "Character.h"

Character::Character(const Vec2& pos)
	:
	sprite("Images//link90x90.bmp"),
	pos(pos)
{
	for (int i = 0; i < int(Sequence::StandingLeft); i++)
	{
		animations.emplace_back(Animation(90, 90*i, 90, 90, 4, sprite, 0.16f));
	}
	for (int i = int(Sequence::StandingLeft); i < int(Sequence::Count); i++)
	{
		animations.emplace_back(Animation(0, 90 * (i - int(Sequence::StandingLeft)), 90, 90, 1, sprite, 0.16f));
	}
}

void Character::Draw(Graphics& gfx) const
{
	if (effectActive)
	{
		animations[int(iCurSequence)].DrawInverse(pos, gfx);
	}
	else
	{
		animations[int(iCurSequence)].Draw(pos, gfx);
	}
}

void Character::SetDirection(const Vec2& dir)
{
	if (dir.x > 0.0f)
	{
		iCurSequence = Sequence::WalkingRight;
	}
	else if (dir.x < 0.0f)
	{
		iCurSequence = Sequence::WalkingLeft;
	}
	else if (dir.y > 0.0f)
	{
		iCurSequence = Sequence::WalkingDown;
	}
	else if (dir.y < 0.0f)
	{
		iCurSequence = Sequence::WalkingUp;
	}
	else 
	{
		if (vel.x > 0.0f)
		{
			iCurSequence = Sequence::StandingRight;
		}
		else if (vel.x < 0.0f)
		{
			iCurSequence = Sequence::StandingLeft;
		}
		else if (vel.y > 0.0f)
		{
			iCurSequence = Sequence::StandingDown;
		}
		else if (vel.y < 0.0f)
		{
			iCurSequence = Sequence::StandingUp;
		}
	}
	vel = dir * speed;

}

void Character::Update(float dt)
{
	pos += vel * dt;
	animations[int(iCurSequence)].Update(dt);

	if (effectActive)
	{
		effectTime += dt;
		if (effectTime >= effectDuration)
		{
			effectTime = 0.0f;
			effectActive = false;
		}
	}
}

void Character::effectActivate()
{
	effectActive = true;
	effectTime = 0.0f;
}

Surface Character::GetSurface() const
{
	return sprite;
}
