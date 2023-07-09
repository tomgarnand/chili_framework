#pragma once

 inline bool IsBetween(float a, float b, float c, float error)
{
	if (((b - a) < error) && (c - b) < error)
	{
		return true;
	}
	else if (((b - c) < error) && (a - b) < error)
	{
		return true;
	}
	return false;
}