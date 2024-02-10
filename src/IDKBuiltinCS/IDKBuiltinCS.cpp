#include "IDKBuiltinCS.hpp"



void
idk::registerComponents( idecs::ECS &ecs )
{
    ecs.registerComponentMandatory<idk::IconCmp>("Icon");
    ecs.registerComponent<TransformCmp>         ("Transform");
    ecs.registerComponent<ModelCmp>             ("Model");
    ecs.registerComponent<ScriptCmp>            ("Script");
    ecs.registerComponent<CameraCmp>            ("Camera");

    ecs.registerComponent<AudioEmitterCmp>      ("AudioEmitter");
    ecs.registerComponent<AudioListenerCmp>     ("AudioListener");


    ecs.registerComponent<PhysXSphereRigidDynamicCmp> ("PhysXSphereRigidDynamic");
    ecs.registerComponent<PhysXMeshRigidStaticCmp>    ("PhysXMeshRigidStatic");

    // ecs.setComponentBehaviour<idk::TransformCmp>  (idk::getBehaviour<idk::TransformCmp>());
    // ecs.setComponentBehaviour<idk::ModelCmp>      (idk::getBehaviour<idk::ModelCmp>());
}




void
idk::registerSystems( idecs::ECS &ecs )
{
    ecs.registerSystem<idk::TransformSys>();
    ecs.registerSystem<idk::ScriptSys>();
    ecs.registerSystem<idk::ModelSys>();
    ecs.registerSystem<idk::CameraSys>();
    ecs.registerSystem<idk::PhysXSys>();
    ecs.registerSystem<idk::AudioSys>();
}