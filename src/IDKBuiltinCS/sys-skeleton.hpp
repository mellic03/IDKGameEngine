#pragma once

#include "idk_components.hpp"
#include <IDKECS/IDKECS.hpp>


namespace idk
{
    struct Bone;
    struct Skeleton;

    struct AnimatorCmp;
    struct SkeletonCmp;
    class  SkeletonSys;
}


struct idk::Bone
{
    int              m_parent;
    idk::vector<int> m_children;
};


struct idk::Skeleton
{
    idk::vector<idk::Bone> m_bones; // m_bones[0] is root bone.
};



class idk::SkeletonSys: public idk::ecs::System
{
private:

public:
    virtual void    init   ( idk::EngineAPI & ) final;
    virtual void    update ( idk::EngineAPI & ) final;

};


struct idk::AnimatorCmp
{

};


struct idk::SkeletonCmp
{
    int obj_id = -1;

    size_t  serialize            ( std::ofstream &stream ) const;
    size_t  deserialize          ( std::ifstream &stream );
    void    onObjectAssignment   ( idk::EngineAPI &api, int obj_id );
    void    onObjectDeassignment ( idk::EngineAPI &api, int obj_id );
    void    onObjectCopy         ( idk::EngineAPI &api, int src_obj, int dst_obj );
};


