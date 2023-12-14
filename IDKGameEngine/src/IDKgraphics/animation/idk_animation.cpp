#include "idk_animation.hpp"
#include <glm/gtx/matrix_interpolation.hpp>


void
idk::AnimBone::tick( float current_time )
{
    m_time = current_time;

    int num_p = positions.size();
    int num_r = rotations.size();
    int num_s = scales.size();

    if (num_p > 0)
    {
        current_p = (current_time / position_timings.back()) * (num_p - 1);
    }

    if (num_r > 0)
    {
        current_r = (current_time / rotation_timings.back()) * (num_r - 1);
    }


    // for (int i=0; i<num_p-1; i++)
    // {
    //     if (m_time < position_timings[i])
    //     {
    //         current_p = i;
    //         break;
    //     }
    // }

    // for (int i=0; i<num_r-1; i++)
    // {
    //     if (m_time < rotation_timings[i])
    //     {
    //         current_r = i;
    //         break;
    //     }
    // }

    for (int i=0; i<num_s-1; i++)
    {
        if (m_time < scale_timings[i])
        {
            current_s = i;
            break;
        }
    }

}


float compute_alpha( float last_time, float next_time, float current_time )
{
    return (current_time - last_time) / (next_time - last_time);
}


glm::vec3
idk::AnimBone::getPosition()
{
    glm::vec3 P = glm::vec3(0.0f);

    if (positions.size() > 0)
    {
        int next_p = (current_p + 1) % positions.size();
        float a = compute_alpha(position_timings[current_p], position_timings[next_p], m_time);
        P = glm::mix(positions[current_p], positions[next_p], a);
    }

    return P;
}


glm::quat
idk::AnimBone::getRotation()
{
    glm::quat R;

    if (rotations.size() > 0)
    {
        int next_r = (current_r + 1) % rotations.size();
        float a = compute_alpha(rotation_timings[current_r], rotation_timings[next_r], m_time);
        R = glm::slerp(rotations[current_r], rotations[next_r], a);
    }

    return R;
}


glm::vec3
idk::AnimBone::getScale()
{
    glm::vec3 S = glm::vec3(2.0f);

    if (scales.size() > 0)
    {
        S = scales[current_s];
    }

    return S;
}



glm::mat4
idk::AnimBone::getTransform()
{
    glm::mat4 T = glm::translate(glm::mat4(1.0f), getPosition());
    glm::mat4 R = glm::toMat4(getRotation());
    glm::mat4 S = glm::scale(glm::mat4(1.0f), getScale());

    return T * R * S;
}


void
idk::Animation::computeTransforms( const glm::mat4 &parent_transform, int bone_id,
                                   std::vector<glm::mat4> &out )
{
    idk::AnimBone &bone = m_bones[bone_id];

    glm::mat4 animated_transform = bone.getTransform();
    glm::mat4 bone_transform     = parent_transform * animated_transform;

    out[bone_id] = bone_transform * bone.inverse_bind;

    for (int child_id: bone.children)
    {
        computeTransforms(bone_transform, child_id, out);
    }
}


void
idk::Animation::computeTransforms( std::vector<glm::mat4> &transforms )
{
    transforms.resize(m_bones.size());
    computeTransforms(glm::mat4(1.0f), 0, transforms);
}


void
idk::Animation::tick( float dt )
{
    m_time = fmod(m_time+dt, m_duration);

    for (idk::AnimBone &bone: m_bones)
    {
        bone.m_duration = m_duration;
        bone.tick(m_time);
    }
}


void
idk::AnimationController::computeBlended( float alpha, const glm::mat4 &parent_transform,
                                          idk::Animation &A, idk::Animation &B,
                                          int bone_id, std::vector<glm::mat4> &out )
{
    idk::AnimBone &bone_A = A.m_bones[bone_id];
    idk::AnimBone &bone_B = B.m_bones[bone_id];

    glm::vec3 pos_A = bone_A.getPosition();
    glm::vec3 pos_B = bone_B.getPosition();

    glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::mix(pos_A, pos_B, alpha));
    glm::mat4 R = glm::toMat4(glm::slerp(bone_A.getRotation(), bone_B.getRotation(), alpha));
    glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::mix(bone_A.getScale(), bone_A.getScale(), alpha));

    glm::mat4 animated_transform = T * R * S;
    glm::mat4 transform = parent_transform * animated_transform;

    out[bone_id] = transform * bone_B.inverse_bind;

    for (int child_id: bone_A.children)
    {
        computeBlended(alpha, transform, A, B, child_id, out);
    }
}



void
idk::AnimationController::computeTransforms( int animation_id, std::vector<glm::mat4> &out )
{
    m_animations[animation_id].computeTransforms(out);
}


void
idk::AnimationController::computeTransforms( std::vector<glm::mat4> &transforms )
{
    idk::Animation &A = m_animations[m_anim_A];
    idk::Animation &B = m_animations[m_anim_B];
    transforms.resize(A.m_bones.size());

    if (m_anim_A == m_anim_B)
    {
        computeTransforms(m_anim_A, transforms);
    }

    else
    {
        computeBlended(m_blend_alpha, glm::mat4(1.0f), A, B, 0, transforms);
    }
}


void
idk::AnimationController::tick( float dt )
{
    idk::Animation &anim = m_animations[m_anim_A];
    anim.tick(1000.0f * dt);

    if (m_anim_A != m_anim_B)
        m_animations[m_anim_B].tick(1000.0f * dt);
}


