#include "sys-planet.hpp"
#include "sys-transform.hpp"


static idk::EngineAPI *api_ptr;

static glm::vec3 sun_position = glm::vec3(100.0f, 0.0f, 100.0f);




void
idk::PlanetSys::update_sun( idk::EngineAPI &api )
{
    auto &ren = api.getRenderer();
    auto &ecs = api.getECS();

    for (idk::SunCmp &cmp: ecs.getComponentArray<idk::SunCmp>())
    {
        sun_position = TransformSys::getPositionWorldspace(cmp.obj_id);
    }
}


void
idk::PlanetSys::update_planet( idk::EngineAPI &api )
{
    auto &engine = api.getEngine();
    auto &ren = api.getRenderer();
    auto &ecs = api.getECS();

    for (idk::PlanetCmp &cmp: ecs.getComponentArray<idk::PlanetCmp>())
    {
        // for (int child: ecs.getChildren(cmp.obj_id))
        // {
        //     glm::vec3 planet_pos = TransformSys::getPositionWorldspace(cmp.obj_id);
        //     glm::vec3 child_pos  = TransformSys::getPositionWorldspace(child);

        //     float alpha = glm::clamp(4.0f * engine.deltaTime(), 0.0f, 0.9999f);

        //     glm::vec3 G = glm::normalize(child_pos - planet_pos);
        //     glm::vec3 &up = TransformSys::getData(child).up;

        //     up = glm::mix(up, G, alpha);

        //     float &roll = TransformSys::getData(child).roll;
        //     roll = glm::mix(roll, 0.0f, alpha);
        // }
    }
}


void
idk::PlanetSys::update_atmosphere( idk::EngineAPI &api )
{
    auto &ren = api.getRenderer();
    auto &ecs = api.getECS();

    for (idk::AtmosphereCmp &cmp: ecs.getComponentArray<idk::AtmosphereCmp>())
    {
        int obj_id = cmp.obj_id;
        glm::vec3 position = TransformSys::getPositionWorldspace(obj_id);

        auto &atmosphere            = ren.getAtmosphere(cmp.atmosphere_id);
        atmosphere.position         = glm::vec4(position, 1.0f);
        atmosphere.sun_position     = glm::vec4(sun_position, 1.0);
        atmosphere.wavelengths      = cmp.wavelengths;
        atmosphere.radius           = cmp.radius;
        atmosphere.density_falloff  = cmp.density_falloff;
        atmosphere.scatter_strength = cmp.scatter_strength;
        atmosphere.atmosphere_scale = cmp.atmosphere_scale;
    }
}




void
idk::PlanetSys::update( idk::EngineAPI &api )
{
    api_ptr      = &api;
    auto &engine = api.getEngine();
    auto &ren    = api.getRenderer();
    auto &ecs    = api.getECS();

    update_sun(api);
    update_planet(api);
    update_atmosphere(api);


    for (auto &cmp: ecs.getComponentArray<idk::PlanetCmp>())
    {
        if (cmp.orbital_speed != glm::vec4(0.0f))
        {
            float x = cmp.orbital_origin[0] + cmp.orbital_radii[0] * sin(cmp.orbital_increment[0]);
            float y = cmp.orbital_origin[1] + cmp.orbital_radii[1] * sin(cmp.orbital_increment[1]);
            float z = cmp.orbital_origin[2] + cmp.orbital_radii[2] * cos(cmp.orbital_increment[2]);


            glm::vec3 old_pos = TransformSys::getPositionWorldspace(cmp.obj_id);
            glm::vec3 new_pos = glm::vec3(x, y, z);
            glm::vec3 delta   = new_pos - old_pos;

            cmp.orbital_origin[3] = delta.x;
            cmp.orbital_increment[3] = delta.y;
            cmp.orbital_radii[3] = delta.z;

            TransformSys::setPositionWorldspace(cmp.obj_id, new_pos);

            cmp.orbital_increment += api.getEngine().deltaTime() * cmp.orbital_speed;
        }
    }


    for (auto &[obj_id, enabled, has_planet]: ecs.getComponentArray<idk::PlanetActorCmp>())
    {
        if (enabled == false)
        {
            continue;
        }

        glm::vec3 position = TransformSys::getPositionWorldspace(obj_id);
        int       nearest  = nearestPlanet(position);
        int       parent   = ecs.getParent(obj_id);
    
        bool      inside_SOI  = (nearest != -1);
        bool      outside_SOI = !inside_SOI;
        bool      has_parent  = (parent != -1);


        if (has_parent && ecs.hasComponent<PlanetCmp>(parent) == false)
        {
            has_planet = false;
            continue;
        }


        if (inside_SOI)
        {
            if (has_parent == false)
            {
                ecs.giveChild(nearest, obj_id);
            }

            has_planet = true;
        }

        else if (outside_SOI)
        {
            if (has_parent == true && ecs.hasComponent<PlanetCmp>(parent))
            {
                ecs.removeParent(obj_id);
            }

            has_planet = false;
        }

        else
        {
            has_planet = false;
        }


        if (has_planet)
        {
            glm::vec3 planet_pos = TransformSys::getPositionWorldspace(parent);
            // TransformSys::getData(obj_id).up = glm::normalize(position - parent_pos);

            float alpha = glm::clamp(4.0f * engine.deltaTime(), 0.0f, 0.9999f);

            glm::vec3 G = glm::normalize(position - planet_pos);
            glm::vec3 &up = TransformSys::getData(obj_id).up;
            float &roll = TransformSys::getData(obj_id).roll;

            // up = G;
            // roll = 0.0f;
            up = glm::mix(up, G, alpha);
            roll = glm::mix(roll, 0.0f, alpha);
        }
    }
}


void
idk::PlanetSys::exposeToLua( lua_State *LS )
{
    luaaa::LuaModule mod(LS, "PlanetSys");
    mod.fun("actorHasPlanet", PlanetSys::actorHasPlanet);

    mod.fun("disablePlanetActor", PlanetSys::disablePlanetActor);
    mod.fun("enablePlanetActor",  PlanetSys::enablePlanetActor);

}



int
idk::PlanetSys::nearestPlanet( const glm::vec3 &position )
{
    int nearest = -1;
    float nearest_dist = 1000000.0f;

    for (idk::PlanetCmp &cmp: api_ptr->getECS().getComponentArray<idk::PlanetCmp>())
    {
        glm::vec3 p = TransformSys::getPositionWorldspace(cmp.obj_id);

        float dist = glm::distance(position, p);

        if (dist < nearest_dist && dist < cmp.SOI)
        {
            nearest_dist = dist;
            nearest = cmp.obj_id;
        }
    }

    return nearest;
}



int
idk::PlanetSys::isInsideLocalGravity( const glm::vec3 &pos, glm::vec3 &G )
{
    auto &ecs = api_ptr->getECS();

    // TODO: Use an octree for this.
    // -----------------------------------------------------------------------------------------
    for (idk::LocalGravityCmp &cmp: ecs.getComponentArray<idk::LocalGravityCmp>())
    {
        if (TransformSys::isInsideBoundingRect(cmp.obj_id, pos))
        {
            G = -TransformSys::getUp(cmp.obj_id);
            return cmp.obj_id;
        }
    }
    // -----------------------------------------------------------------------------------------

    return -1;
}


int
idk::PlanetSys::isInsideLocalGravity( int obj_id, glm::vec3 &G )
{
    auto &ecs = api_ptr->getECS();

    glm::vec3 obj_pos = TransformSys::getPositionWorldspace(obj_id);

    return isInsideLocalGravity(obj_pos, G);
}



glm::vec3
idk::PlanetSys::averageGravity( const glm::vec3 &position )
{
    auto &ren = api_ptr->getRenderer();
    auto &ecs = api_ptr->getECS();

    glm::vec3 G = glm::vec3(0.0f);

    // First check if point is inside object with LocalGravityCmp.
    // -----------------------------------------------------------------------------------------
    int LG_ID = isInsideLocalGravity(position, G);

    if (LG_ID != -1)
    {
        return G;
    }
    // -----------------------------------------------------------------------------------------


    for (idk::PlanetCmp &cmp: ecs.getComponentArray<idk::PlanetCmp>())
    {
        int   obj_id = cmp.obj_id;
        float dist   = glm::distance(TransformSys::getPositionWorldspace(obj_id), position);

        glm::vec3 g  = cmp.gravity * (TransformSys::getPositionWorldspace(obj_id) - position);
                  g *= 1.0f - glm::clamp(dist / cmp.SOI, 0.0f, 1.0f);
                
        G += g;
    }

    return G;
}


bool
idk::PlanetSys::actorHasPlanet( int obj_id )
{
    return api_ptr->getECS().getComponent<idk::PlanetActorCmp>(obj_id).has_planet;
}



idk::PlanetCmp &
idk::PlanetSys::_getPlanetCmp( int obj_id )
{
    return api_ptr->getECS().getComponent<idk::PlanetCmp>(obj_id);
}


float
idk::PlanetSys::getPlanetActorGravity( int obj_id )
{
    int parent = api_ptr->getECS().getParent(obj_id);

    glm::vec3 cpos = TransformSys::getPositionWorldspace(obj_id);
    glm::vec3 ppos = TransformSys::getPositionWorldspace(parent);

    float strength = glm::exp(-(glm::distance(cpos, ppos) / _getPlanetCmp(parent).SOI));

    return strength * _getPlanetCmp(obj_id).gravity;
}




void
idk::PlanetSys::disablePlanetActor( int obj_id )
{
    api_ptr->getECS().getComponent<PlanetActorCmp>(obj_id).enabled = false;
}


void
idk::PlanetSys::enablePlanetActor( int obj_id )
{
    api_ptr->getECS().getComponent<PlanetActorCmp>(obj_id).enabled = true;
}






size_t
idk::PlanetActorCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;

    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, enabled);

    return n;
};


size_t
idk::PlanetActorCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
        
    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, enabled);

    return n;
};



void
idk::PlanetActorCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{
    this->obj_id = obj_id;
};


void
idk::PlanetActorCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{
    auto &cmp = idk::TransformSys::getData(obj_id);

    cmp.up    = glm::vec3(0.0f, 1.0f, 0.0f);
    cmp.right = glm::vec3(1.0f, 0.0f, 0.0f);
    cmp.front = glm::vec3(0.0f, 0.0f, -1.0f);

};


void
idk::PlanetActorCmp::onObjectCopy( idk::EngineAPI &api, int src_obj, int dst_obj )
{
    
};




