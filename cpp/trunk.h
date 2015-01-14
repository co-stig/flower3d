#pragma once

#include "stdafx.h"
#include "vector3d.h"
#include <vector>

class trunk_segment
{

	public:

		trunk_segment (trunk_segment* p = 0, vector3d c = vector3d(), double t = 1)
		: parent (p), coord (c), thickness (t)
		{ 
		}

		~trunk_segment (void)
		{
			for (int n = 0; n < count(); n++)
			{
				delete get_child(n);
			}
		}

		trunk_segment* add_child (vector3d c)
		{
			trunk_segment* t = new trunk_segment(this, c, thickness);
			children.push_back(t);
			return t;
		}

		trunk_segment* get_child (int n)
		{
			return children[n];
		}

		trunk_segment* get_parent (void)
		{
			return parent;
		}

		int count (void)
		{
			return children.size();
		}

		vector3d get_coord (void)
		{
			return coord;
		}

		void set_coord (vector3d c)
		{
			coord = c;
		}

		double get_thickness (void)
		{
			return thickness;
		}

		void set_thickness (double t)
		{
			thickness = t;
		}

	private:

		trunk_segment* parent;
		std::vector <trunk_segment*> children;
		vector3d coord;
		double thickness;

};

class trunk
{

	public:

		trunk(void);
		~trunk(void);

		void display_gl (void);
		void generate_trunk (void);
		trunk_segment* get_root (void);
		int free_count (void);
		void gen_tree (trunk_segment* p, int depth);

	private:

		trunk_segment root;
//		void gen_tree (trunk_segment* p, int depth);
		void display_tree (trunk_segment* p);
		void rec_count_free (trunk_segment* ts, int* k);

};
