#pragma once

#include "idk_vertex.hpp"
#include <libidk/libidk.hpp>
#include <libidk/IDKgl.hpp>


namespace idk { struct AnimBone; class Animation; };
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>


struct idk_AnimNode
{
    int m_parent_id = -1;
    glm::mat4 local_transform = glm::mat4(1.0f);
};


struct idk_AnimBone
{
    int m_node_id = -1;
    glm::mat4 inverse_bind = glm::mat4(1.0f);
};


class idk_Animation
{
public:

    float m_duration = 0.0f;
    float m_time     = 0.0f;

    std::vector<idk_AnimNode> m_nodes;
    std::vector<idk_AnimBone> m_bones;

    void init();
    void tick( float dt );

};


struct idk::AnimBone
{
    std::string name = "unnamed";
    int current_p = 0;
    int current_r = 0;
    int current_s = 0;
    float current_time = 0.0f;

    int parent_id = -1;
    std::vector<int> children;

    std::vector<glm::vec3> position;
    std::vector<glm::quat> rotation;
    std::vector<glm::vec3> scale;

    std::vector<float>     position_time;
    std::vector<float>     rotation_time;
    std::vector<float>     scale_time;

    glm::mat4 world_transform = glm::mat4(1.0f);
    glm::mat4 local_transform = glm::mat4(1.0f);
    glm::mat4 inverse_bind    = glm::mat4(1.0f);

    glm::mat4   getAnimated();
    void        tick( float time, float duration );
};



class idk::Animation
{
private:
    glm::mat4 compute_parent( int bone_id );
    glm::mat4 compute_pose( int parent_id, int bone_id );

public:
    float m_duration = 0.0f;
    float m_time = 0.0f;

    std::vector<AnimBone>  m_bones;
    std::vector<glm::mat4> m_transforms;

    void init();
    void tick( float dt );


};


