// #include "sys-physics.hpp"
// #include "sys-transform.hpp"
// #include "sys-model.hpp"

// #include <libidk/idk_geometry.hpp>
// #include <libidk/idk_log.hpp>





// static constexpr float TRIANGLE_GRID_CELL_SPAN = 4.0f;

// namespace
// {
//     // std::vector<std::vector<std::vector<idk::geometry::Triangle>>> m_triangle_grid;
//     std::map<std::tuple<int, int>, std::vector<idk::geometry::Triangle>> m_triangle_grid;
// }




// static idk::EngineAPI *api_ptr;


// void
// idk::PhysicsSys::init( idk::EngineAPI &api )
// {
//     // m_triangle_grid.resize(64);

//     // for (auto &col: m_triangle_grid)
//     // {
//     //     col.resize(64);
//     // }

//     api_ptr = &api;
// }




// void
// idk::PhysicsSys::_integrate( idk::EngineAPI &api, float dt )
// {
//     for (auto &cmp: idk::ECS2::getComponentArray<idk::KinematicCapsuleCmp>())
//     {
//         if (cmp.enabled == false)
//         {
//             continue;
//         }

//         if (cmp.curr_pos != cmp.curr_pos)
//         {
//             cmp.curr_pos = TransformSys::getLocalPosition(cmp.obj_id);
//             cmp.prev_pos = cmp.curr_pos - glm::vec3(0.01f, 1.0f, 0.01f);
//         }

//         cmp.curr_pos += dt*cmp.impulse;
//         cmp.impulse  *= 0.5f;
    
//         if (cmp.crouch && cmp.bottom > 0.2f)
//         {
//             cmp.bottom -= 0.1f;
//         }
    
//         else if (cmp.bottom < 0.75f)
//         {
//             cmp.bottom += 0.1f;
//         }


//         for (auto &r_cmp: idk::ECS2::getComponentArray<idk::StaticRectCmp>())
//         {
//             kinematicCapsule_staticRect(dt, cmp, r_cmp);
//         }


//         for (auto &tcmp: ECS2::getComponentArray<idk::TerrainCmp>())
//         {
//             glm::vec3 origin = cmp.curr_pos - glm::vec3(0.0f, cmp.bottom+cmp.radius, 0.0f);
        
//             float terrain_y = TerrainRenderer::heightQuery(origin.x, origin.z);
//             float overlap   = terrain_y - origin.y;
        
//             bool grounded = false;

//             if (overlap > 0.0f)
//             {
//                 if (cmp.force.y < 0.0f)
//                 {
//                     cmp.force.y = 0.0f;
//                 }

//                 if (cmp.vel.y < 0.0f)
//                 {
//                     cmp.vel.y = 0.0f;
//                 }

//                 cmp.curr_pos += 0.5f * glm::vec3(0.0f, overlap, 0.0f);

//                 grounded = true;
//             }


//             if (grounded)
//             {
//                 cmp.airtime = 0.0f;
//             }

//             else
//             {
//                 cmp.airtime += dt;
//             }
            
//             cmp.grounded = (cmp.airtime < 1.0f / 2.0f);
//         }


//         cmp.curr_pos += cmp.force;
//         cmp.force    *= 0.0f;


//         cmp.acc.y = -PhysicsConstants::G;

//         if (cmp.grounded)
//         {
//             cmp.vel.y *= 0.5f;

//             // if (K.keyTapped(idk::Keycode::SPACE))
//             // {
//             //     vel.y = m_jump_force*PhysicsConstants::G;
//             //     glm::vec3 f = glm::vec3(0.0f, m_jump_force*PhysicsConstants::G, 0.0f);
//             //     PhysicsSys::addImpulse(m_obj_id, f);
//             // }
//         }

//         cmp.vel = glm::clamp(cmp.vel, glm::vec3(-8.0f), glm::vec3(+8.0f, +32.0f, +8.0f));
//         cmp.vel += dt * cmp.acc;

//         glm::vec3 dP = cmp.vel + 0.5f*dt*cmp.acc;
//         PhysicsSys::applyForce(cmp.obj_id, dP);
//     }
// }



// void
// idk::PhysicsSys::update( idk::EngineAPI &api )
// {
//     auto  &ren  = api.getRenderer();
//     float dt = api.dtime();

//     m_accumulator += dt;
//     const float step = 1.0f / 60.0f;

//     if (m_accumulator > step)
//     {
//         for (auto &cmp: idk::ECS2::getComponentArray<idk::KinematicCapsuleCmp>())
//         {
//             cmp.prev_pos = cmp.curr_pos;

//             // if (cmp.prev_pos != cmp.prev_pos)
//             // {
//             //     cmp.prev_pos = TransformSys::getPositionWorldspace(cmp.obj_id);
//             // }

//             // if (cmp.curr_pos != cmp.curr_pos)
//             // {
//             //     cmp.curr_pos = TransformSys::getPositionWorldspace(cmp.obj_id);
//             // }
//         }

//         while (m_accumulator > step)
//         {
//             _integrate(api, step);
//             m_accumulator -= step;
//         }
//     }


//     for (auto &s_cmp: idk::ECS2::getComponentArray<idk::KinematicCapsuleCmp>())
//     {
//         if (s_cmp.enabled == false)
//         {
//             continue;
//         }

//         // float alpha = glm::clamp(m_accumulator / step, 0.0f, 1.0f);
//         // glm::vec3 position = glm::mix(s_cmp.prev_pos, s_cmp.curr_pos, alpha);

//         // if (position != position)
//         // {
//         //     s_cmp.prev_pos = TransformSys::getPositionWorldspace(s_cmp.obj_id);
//         //     s_cmp.curr_pos = s_cmp.prev_pos;

//         //     position = s_cmp.curr_pos;
//         // }


//         glm::vec3 prev_world = TransformSys::getWorldPosition(s_cmp.obj_id);
//         glm::vec3 curr_world = glm::mix(prev_world, s_cmp.curr_pos, 0.95f);

//         TransformSys::setWorldPosition(s_cmp.obj_id, curr_world);

//         if (s_cmp.visualise)
//         {
//             glm::vec3 top = curr_world + glm::vec3(0.0f, s_cmp.top, 0.0f);
//             glm::vec3 bot = curr_world - glm::vec3(0.0f, s_cmp.bottom, 0.0f);

//             ren.drawCapsule(top, bot, s_cmp.radius);
//         }
//     }

//     for (auto &r_cmp: idk::ECS2::getComponentArray<idk::StaticRectCmp>())
//     {
//         if (r_cmp.visualise)
//         {
//             glm::mat4 M = idk::TransformSys::getModelMatrix(r_cmp.obj_id);
//             ren.drawRect(M);
//         }
//     }


// }

// void
// idk::PhysicsSys::applyForce( int obj_id, const glm::vec3 &force )
// {
//     float dt = api_ptr->dtime();
//     ECS2::getComponent<KinematicCapsuleCmp>(obj_id).force += dt*force;
// }

// void
// idk::PhysicsSys::addImpulse( int obj_id, const glm::vec3 &impulse )
// {
//     float dt = api_ptr->dtime();
//     ECS2::getComponent<KinematicCapsuleCmp>(obj_id).impulse += impulse;
// }


// void
// idk::PhysicsSys::jump( int obj_id, float force )
// {
//     auto &cmp = ECS2::getComponent<KinematicCapsuleCmp>(obj_id);

//     if (cmp.grounded)
//     {
//         cmp.vel.y = force*PhysicsConstants::G;
//         glm::vec3 f = glm::vec3(0.0f, force*PhysicsConstants::G, 0.0f);
//         PhysicsSys::addImpulse(obj_id, f);
//     }
// }



// bool
// idk::PhysicsSys::raycast( const glm::vec3 &origin, const glm::vec3 &dir, glm::vec3 &hit )
// {
//     float nearest_dist = INFINITY;
//     glm::vec3 nearest_hit = glm::vec3(0.0f);

//     for (auto &r_cmp: idk::ECS2::getComponentArray<idk::StaticRectCmp>())
//     {
//         glm::mat4 M = TransformSys::getModelMatrix(r_cmp.obj_id);
//         glm::mat4 R = glm::mat4_cast(TransformSys::getWorldRotation(r_cmp.obj_id));
//         glm::vec3 r_pos   = TransformSys::getPositionWorldspace(r_cmp.obj_id);
//         glm::vec3 r_scale = TransformSys::getXYZScale(r_cmp.obj_id);

//         glm::vec3 origin_local = glm::vec3(glm::inverse(R) * glm::vec4(origin-r_pos, 1.0f));
//         glm::vec3 dir_local    = glm::mat3(glm::inverse(R)) * dir;

//         glm::vec3 r_min = r_scale * glm::vec3(-0.5f, -0.5f, -0.5f);
//         glm::vec3 r_max = r_scale * glm::vec3(+0.5f, +0.5f, +0.5f);

//         glm::vec3 inv_dir = 1.0f / dir_local;

//         glm::vec3 t0s = inv_dir * (r_min - origin_local);
//         glm::vec3 t1s = inv_dir * (r_max - origin_local);

//         glm::vec3 tsmaller = glm::min(t0s, t1s);
//         glm::vec3 tlarger  = glm::max(t0s, t1s);



//         float tmin = -INFINITY;
//         float tmax = +INFINITY;

//         tmin = glm::max(tmin, glm::max(tsmaller[0], glm::max(tsmaller[1], tsmaller[2])));
//         tmax = glm::min(tmax, glm::min(tlarger[0], glm::min(tlarger[1], tlarger[2])));

//         if (tmin < tmax && tmin > 0.0f)
//         {
//             if (tmin < nearest_dist)
//             {
//                 nearest_dist = tmin;
//                 nearest_hit = origin + tmin*dir;
//             }
//         }
//     }

//     hit = nearest_hit;

//     return nearest_dist < INFINITY;
// }



// void
// idk::PhysicsSys::kinematicCapsule_staticRect( float timestep, KinematicCapsuleCmp &s_cmp, StaticRectCmp &r_cmp )
// {
//     int s_obj = s_cmp.obj_id;
//     int r_obj = r_cmp.obj_id;

//     glm::mat4 r_M   = idk::TransformSys::getModelMatrix(r_obj);
//     glm::vec3 s_pos = s_cmp.curr_pos;

//     glm::vec3 res;

//     bool collides = idk::geometry::capsuleRectIntersection(
//         s_pos, s_cmp.bottom, s_cmp.top, s_cmp.radius, r_M, res
//     );

//     bool grounded = false;

//     if (collides)
//     {
//         if (fabs(res.x) < 0.001f && fabs(res.y) < 0.001f && fabs(res.z) < 0.001f)
//         {

//         }

//         else
//         {
//             float dot = glm::dot(glm::normalize(res), glm::vec3(0.0f, 1.0f, 0.0f));

//             if (dot > 0.7)
//             {                
//                 res.x *= 0.0f;
//                 res.z *= 0.0f;
//                 grounded = true;
//             }
        
//             if (res != res)
//             {
//                 return;
//             }

//             else
//             {
//                 s_cmp.force += 1.0f * res;
//             }
//         }
//     }

//     if (grounded)
//     {
//         s_cmp.airtime = 0.0f;
//     }

//     else
//     {
//         s_cmp.airtime += timestep;
//     }
    
//     s_cmp.grounded = (s_cmp.airtime < 1.0f / 2.0f);
// }


// void
// idk::PhysicsSys::_insert_triangle( const idk::geometry::Triangle &tri )
// {
//     // m_triangle_grid[0][0].push_back(tri);

//     float xmin = glm::min(tri.v0.x, glm::min(tri.v1.x, tri.v2.x));
//     float ymin = glm::min(tri.v0.y, glm::min(tri.v1.y, tri.v2.y));
//     float zmin = glm::min(tri.v0.z, glm::min(tri.v1.z, tri.v2.z));

//     float xmax = glm::max(tri.v0.x, glm::max(tri.v1.x, tri.v2.x));
//     float ymax = glm::max(tri.v0.y, glm::max(tri.v1.y, tri.v2.y));
//     float zmax = glm::max(tri.v0.z, glm::max(tri.v1.z, tri.v2.z));

//     glm::vec3 center  = 0.5f * glm::vec3(xmin+xmax, ymin+ymax, zmin+zmax);
//     glm::vec3 extents = glm::vec3(xmax, ymax, zmax) - glm::vec3(xmin, ymin, zmin);
//     // geometry::AABB triangle_AABB = { center, extents };

//     for (int x=xmin; x<=xmax; x+=2)
//     {
//         for (int z=zmin; z<=zmax; z+=2)
//         {
//             // geometry::AABB cell_AABB = {glm::vec3(x, 0.0f, z), glm::vec3(1)};

//             // if (geometry::AABB_AABB_Intersects(triangle_AABB, cell_AABB))
//             {
//                 auto key = std::make_pair(x, z);
//                 m_triangle_grid[key].push_back(tri);
//             }
//         }
//     }

// }



// void
// idk::PhysicsSys::bakeMeshCollider( int obj_id )
// {
//     auto &ren = api_ptr->getRenderer();

//     auto &cmp = ECS2::getComponent<ModelCmp>(obj_id);
//     int model = cmp.model_id;

//     std::unique_ptr<idk::Vertex_P_N_T_UV[]> vertices;
//     std::unique_ptr<uint32_t[]>             indices;

//     size_t num_vertices, num_indices;

//     ren.modelAllocator().getVertices(model, num_vertices, vertices);
//     ren.modelAllocator().getIndices(model, num_indices, indices);


//     glm::mat4 M = TransformSys::getModelMatrix(obj_id);
//     int count = 0;

//     for (size_t i=0; i<num_indices; i+=3)
//     {
//         size_t idx0 = (indices.get())[i+0];
//         size_t idx1 = (indices.get())[i+1];
//         size_t idx2 = (indices.get())[i+2];

//         const auto &v0 = (vertices.get())[idx0];
//         const auto &v1 = (vertices.get())[idx1];
//         const auto &v2 = (vertices.get())[idx2];

//         idk::geometry::Triangle tri = { v0.position, v1.position, v2.position };

//         tri.v0 = glm::vec3(M * glm::vec4(tri.v0, 1.0f));
//         tri.v1 = glm::vec3(M * glm::vec4(tri.v1, 1.0f));
//         tri.v2 = glm::vec3(M * glm::vec4(tri.v2, 1.0f));

//         _insert_triangle(tri);

//         count += 1;
//     }

//     LOG_INFO() << "[PhysicsSys] Baked " << count << " triangles";

// }


// void
// idk::PhysicsSys::kinematicCapsule_triangle( float dt, KinematicCapsuleCmp &cmp,
//                                             idk::geometry::Triangle &tri )
// {
//     // glm::vec3 res;

//     // bool collides = geometry::capsuleTriangleIntersection(
//     //     cmp.curr_pos, cmp.bottom, cmp.top, cmp.radius, tri, res
//     // );

//     // bool grounded = false;

//     // if (collides)
//     // {
//     //     if (fabs(res.x) < 0.001f && fabs(res.y) < 0.001f && fabs(res.z) < 0.001f)
//     //     {

//     //     }

//     //     else
//     //     {
//     //         float dot = glm::dot(glm::normalize(res), glm::vec3(0.0f, 1.0f, 0.0f));

//     //         // if grounded
//     //         if (dot > 0.7)
//     //         {                
//     //             res.x *= 0.0f;
//     //             res.z *= 0.0f;
//     //             grounded = true;
//     //         }

//     //         cmp.curr_pos += 1.0f * res;
//     //     }
//     // }

//     // if (grounded)
//     // {
//     //     cmp.airtime = 0.0f;
//     // }

//     // else
//     // {
//     //     cmp.airtime += dt;
//     // }

//     // cmp.grounded = (cmp.airtime < 1.0f / 30.0f);
// }


// void
// idk::PhysicsSys::kinematicCapsule_triangleGrid( float dt, KinematicCapsuleCmp &cmp )
// {
//     float xmin = cmp.curr_pos.x - 5;
//     float xmax = cmp.curr_pos.x + 5;

//     float zmin = cmp.curr_pos.z - 5;
//     float zmax = cmp.curr_pos.z + 5;

//     for (float x=xmin; x<=xmax; x+=1)
//     {
//         for (float z=zmin; z<=zmax; z+=1)
//         {
//             auto key = std::make_pair(int(x), int(z));

//             for (auto &tri: m_triangle_grid[key])
//             {
//                 kinematicCapsule_triangle(dt, cmp, tri);
//             }
//         }
//     }


//     // for (auto &row: m_triangle_grid)
//     // {
//     //     for (auto &col: row)
//     //     {

//     //     }
//     //     kinematicCapsule_triangle(dt, cmp, tri);
//     // }
// }







// void
// idk::PhysicsSys::bakeHeightmap( idk::TextureWrapper &wrapper )
// {
//     m_heightmaps.push_back(wrapper);
// }



// float
// idk::PhysicsSys::queryHeightmap( TextureWrapper &heightmap, const glm::vec3 &position, const glm::vec3 &scale )
// {
//     float xmin = -0.5f * scale.x;
//     float xmax = +0.5f * scale.x;

//     float zmin = -0.5f * scale.z;
//     float zmax = +0.5f * scale.z;

//     float xpos = glm::clamp(position.x, xmin, xmax);
//     float zpos = glm::clamp(position.z, zmin, zmax);

//     if (xpos != position.x || zpos != position.z)
//     {
//         return -1.0f;
//     }

//     uint32_t *pixels = (uint32_t *)(heightmap.data());

//     int row = int(heightmap.height * ((zpos - zmin) / (zmax - zmin)));
//     int col = int(heightmap.width  * ((xpos - xmin) / (xmax - xmin)));

//     uint32_t argb   = pixels[heightmap.width*row + col];
//     uint8_t  red    = (argb >> 8) & 0xFF;

//     float    height = float(red);
//              height /= 255.0f;
//              height *= scale.y;

//     return position.y - height;

// }


























// size_t
// idk::PhysicsCmp::serialize( std::ofstream &stream ) const
// {
//     size_t n = 0;

//     n += idk::streamwrite(stream, obj_id);
//     n += idk::streamwrite(stream, linear);
//     n += idk::streamwrite(stream, angular);

//     return n;
// };


// size_t
// idk::PhysicsCmp::deserialize( std::ifstream &stream )
// {
//     size_t n = 0;
        
//     n += idk::streamread(stream, obj_id);
//     n += idk::streamread(stream, linear);
//     n += idk::streamread(stream, angular);

//     return n;
// };


// void
// idk::PhysicsCmp::onObjectAssignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
// {
//     auto &cmp = ECS2::getComponent<TransformCmp>(obj_id);
//     cmp.transform = idk::Transform::fromGLM(glm::mat4(1.0f));
//     // this->obj_id = obj_id;
// };


// void
// idk::PhysicsCmp::onObjectDeassignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
// {
//     // this->obj_id = -1;
// };


// void
// idk::PhysicsCmp::onObjectCopy( idk::ECS &ecs, int src_obj, int dst_obj )
// {
//     // auto &src = idk::ECS2::getComponent<TransformCmp>(src_obj);

//     // *this = src;
//     // this->obj_id = dst_obj;
// };





// size_t
// idk::StaticRectCmp::serialize( std::ofstream &stream ) const
// {
//     size_t n = 0;
//     n += idk::streamwrite(stream, obj_id);
//     n += idk::streamwrite(stream, visualise);
//     return n;
// };


// size_t
// idk::StaticRectCmp::deserialize( std::ifstream &stream )
// {
//     size_t n = 0;
//     n += idk::streamread(stream, obj_id);
//     n += idk::streamread(stream, visualise);
//     return n;
// };


// void
// idk::StaticRectCmp::onObjectAssignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
// {
//     // this->obj_id = obj_id;
// };


// void
// idk::StaticRectCmp::onObjectDeassignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
// {
//     // this->obj_id = -1;
// };


// void
// idk::StaticRectCmp::onObjectCopy( idk::ECS &ecs, int src_obj, int dst_obj )
// {
//     auto &src = idk::ECS2::getComponent<StaticRectCmp>(src_obj);
//     auto &dst = idk::ECS2::getComponent<StaticRectCmp>(dst_obj);
//     dst.visualise = src.visualise;

//     TransformSys::getTransform(dst_obj) = TransformSys::getTransform(src_obj);
// };







// size_t
// idk::KinematicRectCmp::serialize( std::ofstream &stream ) const
// {
//     size_t n = 0;
//     n += idk::streamwrite(stream, obj_id);
//     return n;
// };


// size_t
// idk::KinematicRectCmp::deserialize( std::ifstream &stream )
// {
//     size_t n = 0;
//     n += idk::streamread(stream, obj_id);
//     return n;
// };


// void
// idk::KinematicRectCmp::onObjectAssignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
// {

// };


// void
// idk::KinematicRectCmp::onObjectDeassignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
// {

// };


// void
// idk::KinematicRectCmp::onObjectCopy( idk::ECS &ecs, int src_obj, int dst_obj )
// {
//     auto &src = idk::ECS2::getComponent<KinematicRectCmp>(src_obj);
//     auto &dst = idk::ECS2::getComponent<KinematicRectCmp>(dst_obj);
//     dst.visualise = src.visualise;
// };





// size_t
// idk::KinematicCapsuleCmp::serialize( std::ofstream &stream ) const
// {
//     size_t n = 0;
//     n += idk::streamwrite(stream, obj_id);
//     n += idk::streamwrite(stream, enabled);
//     n += idk::streamwrite(stream, visualise);
//     n += idk::streamwrite(stream, radius);
//     n += idk::streamwrite(stream, bottom);
//     n += idk::streamwrite(stream, top);
//     return n;
// };


// size_t
// idk::KinematicCapsuleCmp::deserialize( std::ifstream &stream )
// {
//     size_t n = 0;
//     n += idk::streamread(stream, obj_id);
//     n += idk::streamread(stream, enabled);
//     n += idk::streamread(stream, visualise);
//     n += idk::streamread(stream, radius);
//     n += idk::streamread(stream, bottom);
//     n += idk::streamread(stream, top);
//     return n;
// };


// void
// idk::KinematicCapsuleCmp::onObjectAssignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
// {
//     auto &cmp = idk::ECS2::getComponent<KinematicCapsuleCmp>(obj_id);
//     cmp.curr_pos = TransformSys::getPositionWorldspace(obj_id);
//     cmp.prev_pos = cmp.curr_pos;
// };


// void
// idk::KinematicCapsuleCmp::onObjectDeassignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
// {
//     // this->obj_id = -1;
// };


// void
// idk::KinematicCapsuleCmp::onObjectCopy( idk::ECS &ecs, int src_obj, int dst_obj )
// {
//     auto &src = idk::ECS2::getComponent<KinematicCapsuleCmp>(src_obj);
//     auto &dst = idk::ECS2::getComponent<KinematicCapsuleCmp>(dst_obj);
//     dst.visualise = src.visualise;
// };


