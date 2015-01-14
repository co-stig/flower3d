#include "smooth_double.h"
#include "StdAfx.h"

long smooth_double::current_time = 0;

smooth_double::smooth_double(void)
: smooth (false), real_value (0)
{
}

smooth_double::smooth_double(double d)
: real_value (d), smooth (false)
{
}

void smooth_double::refresh (void)
{
	long tm = current_time - start_time;
	if (tm < time_to_complete)
	{
		double part = double(tm) / double(time_to_complete);
		// linear increment at first...
		real_value = start_value + (finish_value - start_value) * part;
	} else {
		real_value = finish_value;
		smooth = false;
	}
	last_time = current_time;
}


smooth_double smooth_double::operator += (double d)
{
	if ((last_time != current_time) && (smooth)) refresh ();
	return (real_value += d);
}

smooth_double smooth_double::operator -= (double d)
{
	if ((last_time != current_time) && (smooth)) refresh ();
	return (real_value -= d);
}

smooth_double smooth_double::operator *= (double d)
{
	if ((last_time != current_time) && (smooth)) refresh ();
	return (real_value *= d);
}

smooth_double smooth_double::operator /= (double d)
{
	if ((last_time != current_time) && (smooth)) refresh ();
	return (real_value /= d);
}

smooth_double smooth_double::operator = (double d)
{
	smooth = false;
	return (real_value = d);
}


smooth_double::operator double (void)
{
	if ((last_time != current_time) && (smooth)) refresh ();
	return (real_value);
}

void smooth_double::set_smooth (double dest_value, long complete_time)
{
	smooth = true;
	finish_value = dest_value;
	start_value = real_value;
	start_time = current_time;
	time_to_complete = complete_time;
	last_time = current_time;
}

void smooth_double::finish (void)
{
	smooth = false;
	real_value = finish_value;
}

void smooth_double::refresh_current_time (void)
{
	current_time = timeGetTime ();
}

bool smooth_double::in_progress (void)
{
	return smooth;
}
