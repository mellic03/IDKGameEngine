#pragma once

#include "../model/idk_vertex.hpp"
#include <libidk/GL/common.hpp>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>


namespace idk
{
    struct AnimBone;
    class Animation;
    class Animator;
};




struct idk::AnimBone
{
    std::vector<int> children;

    float m_time = 0.0f;
    float m_duration = 0.0f;
    int current_p = 0;
    int current_r = 0;
    int current_s = 0;

    std::vector<glm::vec3> positions;
    std::vector<glm::quat> rotations;
    std::vector<glm::vec3> scales;

    std::vector<float>     position_timings;
    std::vector<float>     rotation_timings;
    std::vector<float>     scale_timings;

    glm::mat4 local_transform = glm::mat4(1.0f);
    glm::mat4 inverse_bind    = glm::mat4(1.0f);


    glm::vec3   getPosition();
    glm::quat   getRotation();
    glm::vec3   getScale();

    glm::mat4   getTransform();
    void        tick( float dt );
};



class idk::Animation
{
private:
    void computeTransforms( const glm::mat4 &parent_transform, int bone_id,
                            std::vector<glm::mat4> &out );

public:

    float m_duration = 0.0f;
    float m_time     = 0.0f;

    std::vector<idk::AnimBone> m_bones;

    void    tick( float dt );
    void    computeTransforms( std::vector<glm::mat4> & );

};



class idk::Animator
{
private:
    glm::vec3 m_root_motion = glm::vec3(0.0f);
    float m_blend_alpha = 0.0f;
    int   m_anim_A = 0;
    int   m_anim_B = 0;

    void    computeBlended( float alpha, const glm::mat4 &parent_transform,
                            idk::Animation &A, idk::Animation &B,
                            int bone_id, std::vector<glm::mat4> &out );

    void    computeTransforms( int animation_id, std::vector<glm::mat4> & );

public:
    int m_active_animation = 0;
    std::vector<idk::Animation> m_animations;

            Animator() {  };
            Animator( const Animator & );

    size_t  numAnimations() { return m_animations.size(); };

    void    setAnimA( int id ) { m_anim_A = id; };
    void    setAnimB( int id ) { m_anim_B = id; };
    void    setBlendAlpha( float alpha ) { m_blend_alpha = glm::clamp(alpha, 0.0f, 1.0f); };

    void    tick( float dt );
    void    computeTransforms( std::vector<glm::mat4> & );

    const glm::vec3 rootMotion()
    {
        glm::vec3 v = m_root_motion;
        m_root_motion = glm::vec3(0.0f);
        return v;
    };

};

