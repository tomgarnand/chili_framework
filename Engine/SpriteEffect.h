#pragma once
#include "Colors.h"
#include "Graphics.h"

namespace SpriteEffect
{
	class Chroma
	{
	public:
		Chroma(Color chroma)
			:
			chroma(chroma)
		{}
		Chroma operator()(Color cSrc, int xDest, int yDest, Graphics& gfx) const
		{
			if (cSrc != chroma)
			{
				gfx.PutPixel(xDest, yDest, cSrc);
			}
		}
	private:
		Color chroma;
	};
	class Copy
	{
	public:
		Copy() = default;
		Copy operator()(Color cSrc, int xDest, int yDest, Graphics& gfx) const
		{
			gfx.PutPixel(xDest, yDest, cSrc);
		}
	private:
	};
	class Substitution
	{
	public:
		Substitution(Color cSrc, int xDest, int yDest, Graphics& gfx)
			:
			sub(sub),
			chroma(chroma)
		{}
		Substitution operator()(Color cSrc, int xDest, int yDest, Graphics& gfx) const
		{
			if (cSrc == chroma)
			{
				gfx.PutPixel(xDest, yDest, sub);
			}
		}
	private:
		Color chroma = Colors::Magenta;
		Color sub;
	};
	class Ghost
	{
	public:
		Ghost(Color cSrc, int xDest, int yDest, Graphics& gfx)
			:
			chroma(chroma)
		{}
		Ghost operator()(Color cSrc, int xDest, int yDest, Graphics& gfx) const
		{
			if (cSrc != chroma)
			{
				const Color dest = gfx.GetPixel(xDest, yDest);
				const Color blend = Color({ (cSrc.GetR() + dest.GetR()) / 2,(cSrc.GetG() + dest.GetG()) / 2, (cSrc.GetB() + dest.GetB()) / 2 });
				gfx.PutPixel(xDest, yDest, blend);
			}
		}
	private:
		Color chroma;
	};
}