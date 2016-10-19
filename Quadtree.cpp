#include "Quadtree.h"
template<class Type>
Quadtree::Quadtree(const float& size_x, const float& size_y)
{
	Create(size_x, size_y);
}

template<class Type>
Quadtree::~Quadtree()
{
	Clear();
}

template<class Type>
void Quadtree<Type>::Clear()
{
	RemoveAll(root);
	root = nullptr;
}

template<class Type>
bool Quadtree<Type>::Insert(Type * object)
{
	return false;
}
template<class Type>
bool Quadtree<Type>::Remove(Type * object)
{
	return false;
}
template<class Type>
bool Quadtree<Type>::Intersect(std::vector<Type*>& ret)
{
	return false;
}
template<class Type>
void Quadtree<Type>::Create(const float& size_x, const float& size_y)
{
	root = new Quadnode();
	root->aabb = root->aabb.FromCenterAndSize(math::float3::zero, math::float3(size_x, 10, size_y)); //Y axis default value = 10
}
template<class Type>
void Quadtree<Type>::RemoveAll(Quadnode * node)
{
	std::list<Quadnode*>::iterator child = node->childs.begin();
	for (child; child != childs.end(); child++)
	{
		RemoveAll(*child);
		delete node;
	}
}


//------------------------------------------------------------
template<class Type>
Quadnode::Quadnode()
{}
template<class Type>
Quadnode::Quadnode(Quadnode* parent)
{
	this->parent = parent;
}
