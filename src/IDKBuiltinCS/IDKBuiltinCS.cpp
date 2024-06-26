#include "IDKBuiltinCS.hpp"


void
idk::registerComponents()
{
    ECS2::registerComponent<IconCmp>                ("Icon");
    ECS2::registerComponent<TransformCmp>           ("Transform");
    ECS2::registerComponent<ModelCmp>               ("Model");
    ECS2::registerComponent<StaticHeightmapCmp>     ("Heightmap");
    ECS2::registerComponent<CameraCmp>              ("Camera");
    ECS2::registerComponent<DirlightCmp>            ("Dirlight");
    ECS2::registerComponent<PointlightCmp>          ("Pointlight");
    ECS2::registerComponent<SpotlightCmp>           ("Spotlight");
    ECS2::registerComponent<StaticRectCmp>          ("StaticRect");
    ECS2::registerComponent<KinematicRectCmp>       ("KinematicRect");
    ECS2::registerComponent<KinematicCapsuleCmp>    ("KinematicCapsule");
    ECS2::registerComponent<RenderSettingCmp>       ("RenderSetting");
    ECS2::registerComponent<AnchorCmp>              ("Anchor");
    ECS2::registerComponent<SmoothFollowCmp>        ("SmoothFollow");
    ECS2::registerComponent<IKCmp>                  ("IK");
    ECS2::registerComponent<LookTowardCmp>          ("LookToward");
    ECS2::registerComponent<ParticleCmp>            ("ParticleEmitter");

}




void
idk::registerSystems()
{
    ECS2::registerSystem<TransformSys>();
    ECS2::registerSystem<ModelSys>();
    ECS2::registerSystem<CameraSys>();
    ECS2::registerSystem<LightSys>();
    ECS2::registerSystem<PhysicsSys>();
    ECS2::registerSystem<RenderSettingSys>();
    ECS2::registerSystem<ParticleSys>();

}