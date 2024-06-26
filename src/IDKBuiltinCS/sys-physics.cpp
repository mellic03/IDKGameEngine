#include "sys-physics.hpp"
#include "sys-transform.hpp"
#include "sys-model.hpp"

#include <libidk/idk_geometry.hpp>
#include <libidk/idk_log.hpp>


static idk::EngineAPI *api_ptr;



void
idk::PhysicsSys::init( idk::EngineAPI &api )
{
    api_ptr = &api;
}




void
idk::PhysicsSys::_integrate( idk::EngineAPI &api, float dt )
{
    for (auto &s_cmp: idk::ECS2::getComponentArray<idk::KinematicCapsuleCmp>())
    {
        if (s_cmp.enabled == false)
        {
            continue;
        }

        s_cmp.curr_pos += s_cmp.force;
        s_cmp.force *= 0.0f;

        for (auto &r_cmp: idk::ECS2::getComponentArray<idk::StaticRectCmp>())
        {
            kinematicCapsule_staticRect(dt, s_cmp, r_cmp);
        }

        for (auto &t_cmp: ECS2::getComponentArray<StaticHeightmapCmp>())
        {
            auto &heightmap = t_cmp.heightmap;
        
            float height = queryHeightmap(
                heightmap, 
                s_cmp.curr_pos,
                TransformSys::getData(t_cmp.obj_id).scale * TransformSys::getData(t_cmp.obj_id).scale3
            );

            // if (height < s_cmp.bottom)
            {
                float overlap = s_cmp.bottom - height;
                s_cmp.curr_pos.y += overlap;
            }
        }
    }
}



void
idk::PhysicsSys::update( idk::EngineAPI &api )
{
    auto  &ren  = api.getRenderer();
    float dt = api.getEngine().deltaTime();

    m_accumulator += dt;
    const float step = 1.0f / 60.0f;

    if (m_accumulator > step)
    {
        for (auto &cmp: idk::ECS2::getComponentArray<idk::KinematicCapsuleCmp>())
        {
            cmp.prev_pos = cmp.curr_pos;

            if (cmp.prev_pos != cmp.prev_pos)
            {
                cmp.prev_pos = TransformSys::getPositionWorldspace(cmp.obj_id);
            }

            if (cmp.curr_pos != cmp.curr_pos)
            {
                cmp.curr_pos = TransformSys::getPositionWorldspace(cmp.obj_id);
            }
        }

        while (m_accumulator > step)
        {
            _integrate(api, step);
            m_accumulator -= step;
        }
    }


    for (auto &s_cmp: idk::ECS2::getComponentArray<idk::KinematicCapsuleCmp>())
    {
        float alpha = m_accumulator / step;

        glm::vec3 position = glm::mix(s_cmp.prev_pos, s_cmp.curr_pos, alpha);
        TransformSys::setPositionWorldspace(s_cmp.obj_id, position);

        if (s_cmp.visualise)
        {
            glm::vec3 top = position + glm::vec3(0.0f, s_cmp.top, 0.0f);
            glm::vec3 bot = position - glm::vec3(0.0f, s_cmp.bottom, 0.0f);

            ren.drawCapsule(top, bot, s_cmp.radius);
        }
    }

    for (auto &r_cmp: idk::ECS2::getComponentArray<idk::StaticRectCmp>())
    {
        if (r_cmp.visualise)
        {
            glm::mat4 M = idk::TransformSys::getModelMatrix(r_cmp.obj_id);
            ren.drawRect(M);
        }
    }


}

void
idk::PhysicsSys::addForce( int obj_id, const glm::vec3 &force )
{
    float dt = api_ptr->getEngine().deltaTime();
    idk::ECS2::getComponent<KinematicCapsuleCmp>(obj_id).force += dt*force;
}


bool
idk::PhysicsSys::raycast( const glm::vec3 &origin, const glm::vec3 &dir, glm::vec3 &hit )
{
    float nearest_dist = INFINITY;
    glm::vec3 nearest_hit = glm::vec3(0.0f);

    for (auto &r_cmp: idk::ECS2::getComponentArray<idk::StaticRectCmp>())
    {
        glm::mat4 M = TransformSys::getModelMatrix(r_cmp.obj_id);
        glm::mat4 R = glm::mat4_cast(TransformSys::getData(r_cmp.obj_id).rotation);
        glm::vec3 r_pos   = TransformSys::getPositionWorldspace(r_cmp.obj_id);
        glm::vec3 r_scale = TransformSys::getData(r_cmp.obj_id).scale3;

        glm::vec3 origin_local = glm::vec3(glm::inverse(R) * glm::vec4(origin-r_pos, 1.0f));
        glm::vec3 dir_local    = glm::mat3(glm::inverse(R)) * dir;

        glm::vec3 r_min = r_scale * glm::vec3(-0.5f, -0.5f, -0.5f);
        glm::vec3 r_max = r_scale * glm::vec3(+0.5f, +0.5f, +0.5f);

        glm::vec3 inv_dir = 1.0f / dir_local;

        glm::vec3 t0s = inv_dir * (r_min - origin_local);
        glm::vec3 t1s = inv_dir * (r_max - origin_local);

        glm::vec3 tsmaller = glm::min(t0s, t1s);
        glm::vec3 tlarger  = glm::max(t0s, t1s);



        float tmin = -INFINITY;
        float tmax = +INFINITY;

        tmin = glm::max(tmin, glm::max(tsmaller[0], glm::max(tsmaller[1], tsmaller[2])));
        tmax = glm::min(tmax, glm::min(tlarger[0], glm::min(tlarger[1], tlarger[2])));

        if (tmin < tmax && tmin > 0.0f)
        {
            if (tmin < nearest_dist)
            {
                nearest_dist = tmin;
                nearest_hit = origin + tmin*dir;
            }
        }
    }


    hit = nearest_hit;

    for (auto &cmp: ECS2::getComponentArray<StaticHeightmapCmp>())
    {
        auto &heightmap = cmp.heightmap;
    
        float height = queryHeightmap(
            heightmap, 
            origin,
            TransformSys::getData(cmp.obj_id).scale * TransformSys::getData(cmp.obj_id).scale3
        );

        if (height < glm::distance(hit, origin))
        {
            hit = origin - glm::vec3(0.0f, height, 0.0f);
            return true;
        }
    }


    return nearest_dist < INFINITY;
}



void
idk::PhysicsSys::kinematicCapsule_staticRect( float timestep, KinematicCapsuleCmp &s_cmp, StaticRectCmp &r_cmp )
{
    int s_obj = s_cmp.obj_id;
    int r_obj = r_cmp.obj_id;

    glm::mat4 r_M   = idk::TransformSys::getModelMatrix(r_obj);
    glm::vec3 s_pos = s_cmp.curr_pos;

    glm::vec3 res;

    bool collides = idk::geometry::capsuleRectIntersection(
        s_pos, s_cmp.bottom, s_cmp.top, s_cmp.radius, r_M, res
    );

    bool grounded = false;

    if (collides)
    {
        if (fabs(res.x) < 0.001f && fabs(res.y) < 0.001f && fabs(res.z) < 0.001f)
        {

        }

        else
        {
            float dot = glm::dot(glm::normalize(res), glm::vec3(0.0f, 1.0f, 0.0f));

            // if grounded
            if (dot > 0.7)
            {                
                res.x *= 0.0f;
                res.z *= 0.0f;
                grounded = true;
            }

            s_cmp.curr_pos += 1.0f * res;
        }
    }

    if (grounded)
    {
        s_cmp.airtime = 0.0f;
    }

    else
    {
        s_cmp.airtime += timestep;
    }

    s_cmp.grounded = (s_cmp.airtime < 1.0f / 30.0f);

}







void
idk::PhysicsSys::bakeHeightmap( idk::TextureWrapper &wrapper )
{
    m_heightmaps.push_back(wrapper);
}



float
idk::PhysicsSys::queryHeightmap( TextureWrapper &heightmap, const glm::vec3 &position, const glm::vec3 &scale )
{
    float xmin = -0.5f * scale.x;
    float xmax = +0.5f * scale.x;

    float zmin = -0.5f * scale.z;
    float zmax = +0.5f * scale.z;

    float xpos = glm::clamp(position.x, xmin, xmax);
    float zpos = glm::clamp(position.z, zmin, zmax);

    if (xpos != position.x || zpos != position.z)
    {
        return -1.0f;
    }

    uint32_t *pixels = (uint32_t *)(heightmap.data());

    int row = int(heightmap.height * ((zpos - zmin) / (zmax - zmin)));
    int col = int(heightmap.width  * ((xpos - xmin) / (xmax - xmin)));

    uint32_t argb   = pixels[heightmap.width*row + col];
    uint8_t  red    = (argb >> 8) & 0xFF;

    float    height = float(red);
             height /= 255.0f;
             height *= scale.y;

    return position.y - height;

}


























size_t
idk::PhysicsCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;

    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, linear);
    n += idk::streamwrite(stream, angular);

    return n;
};


size_t
idk::PhysicsCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
        
    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, linear);
    n += idk::streamread(stream, angular);

    return n;
};


void
idk::PhysicsCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{
    // this->obj_id = obj_id;
};


void
idk::PhysicsCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{
    // this->obj_id = -1;
};


void
idk::PhysicsCmp::onObjectCopy( int src_obj, int dst_obj )
{
    // auto &src = idk::ECS2::getComponent<TransformCmp>(src_obj);

    // *this = src;
    // this->obj_id = dst_obj;
};





size_t
idk::StaticRectCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, visualise);
    return n;
};


size_t
idk::StaticRectCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, visualise);
    return n;
};


void
idk::StaticRectCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{
    // this->obj_id = obj_id;
};


void
idk::StaticRectCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{
    // this->obj_id = -1;
};


void
idk::StaticRectCmp::onObjectCopy( int src_obj, int dst_obj )
{
    auto &src = idk::ECS2::getComponent<KinematicRectCmp>(src_obj);
    auto &dst = idk::ECS2::getComponent<KinematicRectCmp>(dst_obj);
    dst.visualise = src.visualise;
};







size_t
idk::KinematicRectCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    return n;
};


size_t
idk::KinematicRectCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    return n;
};


void
idk::KinematicRectCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{

};


void
idk::KinematicRectCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{

};


void
idk::KinematicRectCmp::onObjectCopy( int src_obj, int dst_obj )
{
    auto &src = idk::ECS2::getComponent<KinematicRectCmp>(src_obj);
    auto &dst = idk::ECS2::getComponent<KinematicRectCmp>(dst_obj);
    dst.visualise = src.visualise;
};





size_t
idk::KinematicCapsuleCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, enabled);
    n += idk::streamwrite(stream, visualise);
    n += idk::streamwrite(stream, curr_pos);
    n += idk::streamwrite(stream, prev_pos);
    n += idk::streamwrite(stream, radius);
    n += idk::streamwrite(stream, bottom);
    n += idk::streamwrite(stream, top);
    return n;
};


size_t
idk::KinematicCapsuleCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, enabled);
    n += idk::streamread(stream, visualise);
    n += idk::streamread(stream, curr_pos);
    n += idk::streamread(stream, prev_pos);
    n += idk::streamread(stream, radius);
    n += idk::streamread(stream, bottom);
    n += idk::streamread(stream, top);
    return n;
};


void
idk::KinematicCapsuleCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{
    auto &cmp = idk::ECS2::getComponent<KinematicCapsuleCmp>(obj_id);
    cmp.curr_pos = TransformSys::getPositionWorldspace(obj_id);
    cmp.prev_pos = cmp.curr_pos;
};


void
idk::KinematicCapsuleCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{
    // this->obj_id = -1;
};


void
idk::KinematicCapsuleCmp::onObjectCopy( int src_obj, int dst_obj )
{
    auto &src = idk::ECS2::getComponent<KinematicCapsuleCmp>(src_obj);
    auto &dst = idk::ECS2::getComponent<KinematicCapsuleCmp>(dst_obj);
    dst.visualise = src.visualise;
};


