#include "IDK_gameobject.h"


bool
idk::GameObject::hasComponent(idk::GameObject::Component cmp)
{
    return _components[static_cast<uint>(cmp)] > 0;
}


void
idk::GameObject::giveComponent(idk::GameObject::Component cmp)
{
    _components[static_cast<uint>(cmp)] += 1;
}


void
idk::GameObject::removeComponent(idk::GameObject::Component cmp)
{
    _components[static_cast<uint>(cmp)] -= 1;
}



void createActor()
{
    idk::GameObject obj;
}

