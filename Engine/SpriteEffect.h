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
		void operator()(Color cSrc, int xDest, int yDest, Graphics& gfx) const
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
		void operator()(Color cSrc, int xDest, int yDest, Graphics& gfx) const
		{
			gfx.PutPixel(xDest, yDest, cSrc);
		}
	private:
	};
	class Substitution
	{
	public:
		Substitution(Color chroma, Color sub)
			:
			chroma(chroma),
			sub(sub)
		{}
		void operator()(Color cSrc, int xDest, int yDest, Graphics& gfx) const
		{
			if (cSrc != chroma)
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
		Ghost(Color chroma)
			:
			chroma(chroma)
		{}
		void operator()(Color cSrc, int xDest, int yDest, Graphics& gfx) const
		{
			if (cSrc != chroma)
			{
				const Color dest = gfx.GetPixel(xDest, yDest);
				const Color blend = Color({ unsigned char((cSrc.GetR() + dest.GetR()) / 2),unsigned char((cSrc.GetG() + dest.GetG()) / 2), unsigned char((cSrc.GetB() + dest.GetB()) / 2) });
				gfx.PutPixel(xDest, yDest, blend);
			}
		}
	private:
		Color chroma;
	};
}