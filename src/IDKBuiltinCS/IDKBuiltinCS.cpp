#include "IDKBuiltinCS.hpp"


void
idk::registerComponents( idk::ecs::ECS &ecs )
{
    ecs.registerComponentMandatory<idk::IconCmp>        ("Icon");
    ecs.registerComponent<TransformCmp>                 ("Transform");
    ecs.registerComponent<ModelCmp>                     ("Model");
    ecs.registerComponent<ScriptCmp>                    ("Script");
    ecs.registerComponent<CameraCmp>                    ("Camera");


    ecs.registerComponent<SunCmp>                       ("Sun");
    ecs.registerComponent<PlanetCmp>                    ("Planet");
    ecs.registerComponent<AtmosphereCmp>                ("Atmosphere");
    ecs.registerComponent<PlanetActorCmp>               ("PlanetActor");

    ecs.registerComponent<DirlightCmp>                  ("Dirlight");
    ecs.registerComponent<PointlightCmp>                ("Pointlight");
    ecs.registerComponent<SpotlightCmp>                 ("Spotlight");

    ecs.registerComponent<AudioEmitterCmp>              ("AudioEmitter");
    ecs.registerComponent<AudioListenerCmp>             ("AudioListener");

    ecs.registerComponent<PhysXSphereRigidDynamicCmp>   ("PhysXSphereRigidDynamic");
    ecs.registerComponent<PhysXMeshRigidStaticCmp>      ("PhysXMeshRigidStatic");
    ecs.registerComponent<PhysXKinematicControllerCmp>  ("PhysXCapsuleKinematic");

    ecs.registerComponent<ProgressionEventCmp>          ("ProgressionEvent");
    ecs.registerComponent<ProgressionStateCmp>          ("ProgressionState");

    ecs.registerComponent<PhysXRectRigidStaticCmp>      ("PhysX_Rect_RigidStatic");

    ecs.registerComponent<PlayerControllerCmp>          ("PlayerController");
    ecs.registerComponent<LocalGravityCmp>              ("LocalGravity");

}




void
idk::registerSystems( idk::ecs::ECS &ecs )
{
    ecs.registerSystem<idk::TransformSys>();
    ecs.registerSystem<idk::ScriptSys>();
    ecs.registerSystem<idk::ModelSys>();
    ecs.registerSystem<idk::CameraSys>();
    ecs.registerSystem<idk::LightSys>();
    ecs.registerSystem<idk::PhysXSys>();
    ecs.registerSystem<idk::PlanetSys>();
    ecs.registerSystem<idk::AudioSys>();
    ecs.registerSystem<idk::GameStateSys>();
    ecs.registerSystem<idk::PlayerSys>();
}