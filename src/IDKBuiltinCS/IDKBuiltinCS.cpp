#include "IDKBuiltinCS.hpp"
#include <IDKECS/ecs1/idk_ecs.hpp>

void
idk::registerComponents( idk::ECS &ecs )
{
    ecs.registerComponent<IconCmp>                ("Icon");

    ecs.registerComponent<TransformCmp>           ("Transform");
    ecs.registerComponent<IKCmp>                  ("IK");
    ecs.registerComponent<AnchorCmp>              ("Anchor");
    ecs.registerComponent<SmoothFollowCmp>        ("SmoothFollow");
    ecs.registerComponent<LookTowardCmp>          ("LookToward");
    ecs.registerComponent<RotateCmp>              ("Rotate");

    // ecs.registerComponent<TriggerCmp>             ("Trigger");

    ecs.registerComponent<AudioEmitterCmp>        ("AudioEmitter");
    ecs.registerComponent<AudioListenerCmp>       ("AudioListener");
    // ecs.registerComponent<PhysicsCmp>             ("Physics");

    ecs.registerComponent<ModelCmp>               ("Model");
    ecs.registerComponent<TerrainCmp>             ("Terrain");

    // ecs.registerComponent<StaticHeightmapCmp>     ("Heightmap");

    ecs.registerComponent<CameraCmp>              ("Camera");
    ecs.registerComponent<DirlightCmp>            ("Dirlight");
    ecs.registerComponent<PointlightCmp>          ("Pointlight");
    ecs.registerComponent<SpotlightCmp>           ("Spotlight");
    // ecs.registerComponent<StaticRectCmp>          ("StaticRect");
    // ecs.registerComponent<KinematicRectCmp>       ("KinematicRect");
    // ecs.registerComponent<KinematicCapsuleCmp>    ("KinematicCapsule");
    ecs.registerComponent<RenderSettingCmp>       ("RenderSetting");
    ecs.registerComponent<ScriptCmp>              ("Script");
    ecs.registerComponent<ParticleCmp>            ("ParticleEmitter");

}




void
idk::registerSystems( idk::ECS &ecs )
{
    ecs.registerSystem<TransformSys>();
    // ecs.registerSystem<TriggerSys>();
    ecs.registerSystem<AudioSys>();
    ecs.registerSystem<ModelSys>();
    ecs.registerSystem<CameraSys>();
    ecs.registerSystem<LightSys>();
    // ecs.registerSystem<PhysicsSys>();
    ecs.registerSystem<RenderSettingSys>();
    ecs.registerSystem<ScriptSys>();
    ecs.registerSystem<ParticleSys>();

}