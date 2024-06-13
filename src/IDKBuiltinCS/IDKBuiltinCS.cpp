#include "IDKBuiltinCS.hpp"


void
idk::registerComponents( idk::ecs::ECS &ecs )
{
    ecs.registerComponentMandatory<idk::IconCmp>        ("Icon");
    ecs.registerComponent<TransformCmp>                 ("Transform");
    ecs.registerComponent<ModelCmp>                     ("Model");
    ecs.registerComponent<ScriptCmp>                    ("Script");
    ecs.registerComponent<CameraCmp>                    ("Camera");
    ecs.registerComponent<SkeletonCmp>                  ("Skeleton");

    ecs.registerComponent<PlayerControllerCmp>          ("PlayerController");
    ecs.registerComponent<ProgressionEventCmp>          ("ProgressionEvent");
    ecs.registerComponent<ProgressionStateCmp>          ("ProgressionState");

    ecs.registerComponent<SunCmp>                       ("Sun");
    ecs.registerComponent<PlanetCmp>                    ("Planet");
    ecs.registerComponent<AtmosphereCmp>                ("Atmosphere");
    ecs.registerComponent<PlanetActorCmp>               ("PlanetActor");
    ecs.registerComponent<LocalGravityCmp>              ("LocalGravity");

    ecs.registerComponent<DirlightCmp>                  ("Dirlight");
    ecs.registerComponent<PointlightCmp>                ("Pointlight");
    ecs.registerComponent<SpotlightCmp>                 ("Spotlight");

    ecs.registerComponent<AudioEmitterCmp>              ("AudioEmitter");
    ecs.registerComponent<AudioListenerCmp>             ("AudioListener");

    ecs.registerComponent<PhysicsCmp>                   ("Physics");
    ecs.registerComponent<idk::StaticRectCmp>           ("StaticRect");
    ecs.registerComponent<idk::KinematicRectCmp>        ("KinematicRect");
    ecs.registerComponent<idk::KinematicCapsuleCmp>     ("KinematicCapsule");

    ecs.registerComponent<idk::RenderSettingCmp>        ("RenderSetting");
    ecs.registerComponent<idk::AnchorCmp>               ("Anchor");
    ecs.registerComponent<idk::SmoothFollowCmp>         ("SmoothFollow");

    ecs.registerComponent<ArmCmp>                       ("Arm");


    ECS2::registerComponent<IconCmp>                ("Icon");
    ECS2::registerComponent<TransformCmp>           ("Transform");
    ECS2::registerComponent<ModelCmp>               ("Model");
    ECS2::registerComponent<CameraCmp>              ("Camera");
    ECS2::registerComponent<PlayerControllerCmp>    ("PlayerController");
    ECS2::registerComponent<DirlightCmp>            ("Dirlight");
    ECS2::registerComponent<PointlightCmp>          ("Pointlight");
    ECS2::registerComponent<SpotlightCmp>           ("Spotlight");
    ECS2::registerComponent<StaticRectCmp>          ("StaticRect");
    ECS2::registerComponent<KinematicRectCmp>       ("KinematicRect");
    ECS2::registerComponent<KinematicCapsuleCmp>    ("KinematicCapsule");
    ECS2::registerComponent<RenderSettingCmp>       ("RenderSetting");
    ECS2::registerComponent<AnchorCmp>              ("Anchor");
    ECS2::registerComponent<SmoothFollowCmp>        ("SmoothFollow");


}




void
idk::registerSystems( idk::ecs::ECS &ecs )
{
    ecs.registerSystem<idk::TransformSys>();
    ecs.registerSystem<idk::ScriptSys>();
    ecs.registerSystem<idk::ModelSys>();
    ecs.registerSystem<idk::CameraSys>();
    ecs.registerSystem<idk::LightSys>();

    ecs.registerSystem<idk::PhysicsSys>();

    ecs.registerSystem<idk::PlanetSys>();
    ecs.registerSystem<idk::AudioSys>();
    ecs.registerSystem<idk::GameStateSys>();
    ecs.registerSystem<idk::PlayerSys>();
    ecs.registerSystem<idk::SkeletonSys>();

    ecs.registerSystem<idk::RenderSettingSys>();


    ECS2::registerSystem<TransformSys>();
    ECS2::registerSystem<ModelSys>();
    ECS2::registerSystem<CameraSys>();
    ECS2::registerSystem<LightSys>();
    ECS2::registerSystem<PhysicsSys>();
    ECS2::registerSystem<PlayerSys>();
    ECS2::registerSystem<RenderSettingSys>();

}