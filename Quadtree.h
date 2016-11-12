#ifndef __QUADTREE_H__
#define __QUADTREE_H__

#include "MathGeoLib\include\MathGeoLib.h"
#include <list>
#include <vector>

template<typename Type>
class Quadnode
{
public:
	Quadnode(){}
	Quadnode(Quadnode<Type> * parent, const math::AABB & aabb) : parent(parent), aabb(aabb)
	{
		if (parent)
			parent->childs.push_back(this);
	}

	~Quadnode()
	{
		for (std::list<Quadnode<Type>*>::iterator child = childs.begin(); child != childs.end(); ++child)
		{
			delete *child;
		}
	}

	///If the content intersects more than one child the content is assigned to multiple childs.
	bool Insert(Type* item, const math::vec& point)
	{
		bool ret = false;

		if (aabb.Contains(point))
		{
			if (childs.size() > 0)
			{
				for (std::list<Quadnode<Type>*>::iterator i = childs.begin(); i != childs.end(); i++) //Try with childs
					(*i)->Insert(item, point);
			}
			else //Leaf
			{
				if (content) //Divide the node
				{
					Divide();
					for (std::list<Quadnode<Type>*>::iterator i = childs.begin(); i != childs.end(); i++) //Insert content and item in childs
					{
						bool success;
						(*i)->Insert(content, content_position);
						success = (*i)->Insert(item, point);

						if (success == true)
							ret = true;
					}
					content = nullptr;
					content_position = math::vec::zero;
				}
				else
				{
					content = item;
					content_position = point;
					ret = true;			//Successful insertion
				}

			}
		}

		return ret;
	}

	bool Remove(Type* item, const math::vec& point, vector<Quadnode*>& nodes_to_remove)
	{
		bool ret = false;

		if (aabb.Contains(point))
		{
			if (childs.size > 0)
			{
				for (std::list<Quadnode<Type>*>::iterator i = childs.begin(); i != childs.end(); i++)
				{
					(*i)->Remove(item, point);
				}
			}
			else //Leaf
			{
				if (content == item)
				{
					content = nullptr;
					content_position = math::vec::zero;

					nodes_to_remove.push_back(this);
					ret = true;
				}
			}
		}
		
		return ret;
	}

	void Draw() const
	{
		g_Debug->AddAABB(aabb, g_Debug->red);
		for (std::list<Quadnode<Type>*>::const_iterator i = childs.begin(); i != childs.end(); i++)
		{
			(*i)->Draw();
		}
	}

private:

	void Divide()
	{
		math::AABB new_aabb;
		for (int i = 0; i < 4; i++)
		{
			new_aabb = aabb;

			if (i == 0)
			{
				new_aabb.minPoint.z /= 2;
				new_aabb.maxPoint.x /= 2;
			}

			if (i == 1)
			{
				new_aabb.minPoint.x /= 2;
				new_aabb.minPoint.z /= 2;
			}

			if (i == 2)
			{
				new_aabb.maxPoint.x /= 2;
				new_aabb.maxPoint.z /= 2;
			}

			if (i == 3)
			{
				new_aabb.minPoint.x /= 2;
				new_aabb.maxPoint.z /= 2;
			}

			Quadnode<Type>* node = new Quadnode<Type>(this, new_aabb);
		}
	}

private:
	Type* content = nullptr;
	Quadnode<Type>* parent = nullptr;
	std::list<Quadnode<Type>*> childs;
	math::AABB aabb;
	math::vec content_position = math::vec::zero;
};


template<typename Type>
class Quadtree
{
public:

	Quadtree(){}

	Quadtree(const math::AABB& limits)
	{
		Create(limits);
	}

	~Quadtree()
	{
		Clear();
	}

	void Clear()
	{
		if (root == NULL)
			return;

		delete root;
		root = NULL;
	}

	bool Insert(Type* item, const math::vec& point) 
	{
		bool ret = false;

		if (root != nullptr)
		{
			ret = root->Insert(item, point);
		}

		return ret;
	}

	bool Remove(Type* item, const math::vec& point) 
	{
		bool ret = false;

		if (root != nullptr)
		{
			vector<Quadnode*> nodes_to_remove;
			ret = root->Remove(item, point, nodes_to_remove);

			for (vector<Quadnode*>::iterator it = nodes_to_remove.begin(); it != nodes_to_remove.end(); ++it)
				delete (*it);
		}

		return ret;
	}

	bool Intersect(std::vector<Type*>& result)
	{
		return true;
	}

	void Create(const math::AABB& limits)
	{
		root = new Quadnode<Type>(NULL, limits);
	}

	void Draw() const
	{
		if (root)
		{
			root->Draw();
		}
	}

private:
	Quadnode<Type>* root = nullptr;
};

#endif // !__QUADTREE_H_