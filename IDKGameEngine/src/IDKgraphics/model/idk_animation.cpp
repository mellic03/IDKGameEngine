#include "idk_animation.hpp"



void
idk::AnimBone::tick( float time, float duration )
{
    current_time = time;

    float alpha = time/duration;

    int num_p = position.size();
    int num_r = rotation.size();
    int num_s = scale.size();

    if (num_p > 0)
    {
        current_p = (int)(alpha * (float)(num_p)) % num_p;
    }

    if (num_r > 0)
    {
        current_r = (int)(alpha * (float)(num_r)) % num_r;
    }

    if (num_s > 0)
    {
        current_s = (int)(alpha * (float)(num_s)) % num_s;
    }
}


glm::mat4
idk::AnimBone::getAnimated()
{
    if (position.size() == 0 || rotation.size() == 0 || scale.size() == 0)
    {
        return local_transform; // glm::mat4(1.0f);
    }

    // int next_p = (current_p + 1) % position.size();
    // int next_r = (current_r + 1) % rotation.size();
    // int next_s = (current_s + 1) % scale.size();

    // float t0 = rotation_time[current_p];
    // float t1 = rotation_time[next_p];

    // float alpha = (current_time-t0) / (t1-t0);

    // glm::vec3 pos = (1.0f - alpha)*position[current_p] + alpha*position[next_p];
    // glm::quat rot = glm::slerp(rotation[current_p], rotation[next_p], alpha);
    // glm::vec3 sc  = (1.0f - alpha)*scale[current_s] + alpha*scale[next_s];

    // glm::mat4 S = glm::scale(glm::mat4(1.0f), -sc);
    // glm::mat4 T = glm::translate(glm::mat4(1.0f), pos);
    // glm::mat4 R = glm::mat4_cast(rot);

    // return T * R * S;

    glm::mat4 S = glm::scale(glm::mat4(1.0f), -scale[current_s]);
    glm::mat4 T = glm::translate(glm::mat4(1.0f), position[current_p]);
    glm::mat4 R = glm::mat4_cast(rotation[current_r]);

    return T * R * S;
}


void
idk::Animation::init()
{
    m_transforms.resize(m_bones.size(), glm::mat4(1.0f));
}



glm::mat4
idk::Animation::compute_parent( int bone_id )
{
    int parent_id = m_bones[bone_id].parent_id;

    if (parent_id == -1)
    {
        return m_bones[bone_id].getAnimated();
    }

    glm::mat4 anim_local_transform = m_bones[parent_id].getAnimated();

    return compute_parent(parent_id) * anim_local_transform;
}


std::set<int> computed_bones;
std::map<int, glm::mat4> computed_transforms;


glm::mat4
idk::Animation::compute_pose( int parent_id, int bone_id )
{
    glm::mat4 parent = computed_transforms[parent_id];
    glm::mat4 local  = m_bones[bone_id].getAnimated();

    return parent * local;
}


void
idk::Animation::tick( float dt )
{
    m_time += dt;

    for (size_t i=0; i<m_bones.size(); i++)
    {
        m_bones[i].tick(m_time, m_duration);
    }

    computed_bones.insert(-1);
    computed_transforms[-1] = m_bones[0].local_transform;

    while (computed_bones.size() < m_bones.size())
    {
        for (size_t i=0; i<m_bones.size(); i++)
        {
            int bone_id   = i;
            int parent_id = m_bones[i].parent_id;

            if (computed_bones.find(parent_id) == computed_bones.end())
            {
                continue;
            }

            if (computed_bones.find(bone_id) != computed_bones.end())
            {
                continue;
            }

            glm::mat4 transform = compute_pose(parent_id, bone_id);

            computed_bones.insert(bone_id);
            computed_transforms[bone_id] = transform;

            m_transforms[bone_id] = transform * m_bones[bone_id].inverse_bind;
        }
    }

    computed_bones.clear();
    computed_transforms.clear();

}

