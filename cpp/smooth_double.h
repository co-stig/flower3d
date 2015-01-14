#pragma once

class smooth_double
{

public:

	smooth_double(void);
	smooth_double(double d);

	smooth_double operator += (double d);
	smooth_double operator -= (double d);
	smooth_double operator *= (double d);
	smooth_double operator /= (double d);
	smooth_double operator = (double d);

	operator double (void);

	void set_smooth (double dest_value, long complete_time);
	void finish (void);
	bool in_progress (void);
	
	static void refresh_current_time (void);

private:

	void refresh (void);

	bool smooth;
	long start_time;
	long time_to_complete;
	long last_time;
	double start_value;
	double finish_value;
	double real_value;
	
	static long current_time;
	
};

