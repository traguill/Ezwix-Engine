#ifndef __QUADTREE_H__
#define __QUADTREE_H__

#include <list>
#include <vector>
#include "MathGeoLib\include\MathGeoLib.h"

template<class Type>
struct Quadnode
{
	Quadnode* parent =  nullptr; //Necessary?
	Type* content = nullptr;
	std::list<Quadnode*> childs;
	math::AABB aabb;

	Quadnode();
	Quadnode(Quadnode* parent);
};

template<class Type>
class Quadtree
{
public:
	Quadtree(const float& size_x, const float& size_y);
	~Quadtree();

	void Clear();

	bool Insert(Type* object);
	bool Remove(Type* object);
	bool Intersect(std::vector<Type*>& ret); //TODO:include primitive type

private:
	void Create(const float& size_x, const float& size_y);

	//Called when removing all nodes. Does not update the quadtree
	void RemoveAll(Quadnode* node);

private:
	Quadnode* root; 
};

#endif // !__QUADTREE_H__
