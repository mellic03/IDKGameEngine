#include "IDK_gameobject.h"


bool
idk::GameObject::hasComponent(uint idx)
{
    return _components[idx] > 0;
}


void
idk::GameObject::giveComponent(uint idx)
{
    _components[idx] += 1;
}


void
idk::GameObject::removeComponent(uint idx)
{
    _components[idx] -= 1;
}

