#include "sys-physx.hpp"

#include "../../thirdparty/physx/include/PxPhysicsAPI.h"


glm::vec3
idk::toGLM( const physx::PxVec3 &v )
{
    return glm::vec3(v.x, v.y, v.z);
}


physx::PxVec3
idk::toPX( const glm::vec3 &v )
{
	return physx::PxVec3(v.x, v.y, v.z);
}


glm::quat
idk::toGLM( const physx::PxQuat &pq )
{
    return glm::quat(pq.x, pq.y, pq.z, pq.w);
}


glm::mat4
idk::toGLM( const physx::PxTransform &P )
{
    glm::mat4 M;

    glm::vec3 position = toGLM(P.p);
    glm::quat rotation = toGLM(P.q);

    M[3] = glm::vec4(position, 1.0f);

    M = M * glm::mat4_cast(rotation);

    return M;
}




static idk::EngineAPI *api_ptr;

static bool is_initialized = false;
static physx::PxDefaultAllocator		gAllocator;
static physx::PxDefaultErrorCallback	gErrorCallback;
static physx::PxFoundation*				gFoundation = NULL;
static physx::PxPhysics*				gPhysics	= NULL;
static physx::PxDefaultCpuDispatcher*	gDispatcher = NULL;
static physx::PxScene*					gScene		= NULL;
static physx::PxMaterial*				gMaterial	= NULL;
static physx::PxReal 					stackZ 		= 10.0f;


using namespace physx;


static void createStack(const PxTransform& t, PxU32 size, PxReal halfExtent)
{
	PxShape* shape = gPhysics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *gMaterial);
	for(PxU32 i=0; i<size;i++)
	{
		for(PxU32 j=0;j<size-i;j++)
		{
			PxTransform localTm(PxVec3(PxReal(j*2) - PxReal(size-i), PxReal(i*2+1), 0) * halfExtent);
			PxRigidDynamic* body = gPhysics->createRigidDynamic(t.transform(localTm));
			body->attachShape(*shape);
			PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
			gScene->addActor(*body);
		}
	}
	shape->release();
}



physx::PxRigidDynamic *
idk::PhysXSys::createDynamic( const physx::PxTransform &t, const physx::PxGeometry &geometry,
                              const physx::PxVec3 &velocity )
{
    PxRigidDynamic* dynamic = physx::PxCreateDynamic(*gPhysics, t, geometry, *gMaterial, 1.0f);
	dynamic->setAngularDamping(0.5f);
	dynamic->setLinearVelocity(velocity);
	gScene->addActor(*dynamic);
	return dynamic;
}



physx::PxRigidStatic *
idk::PhysXSys::createMeshCollider( int obj_id )
{
	int  model_id = api_ptr->getECS().getComponent<idk::ModelCmp>(obj_id).model_id;
	auto &MS      = api_ptr->getRenderer().modelSystem();
	auto &model   = MS.getModel(model_id);

	physx::PxTriangleMeshDesc desc;
	desc.points.count  = model.m_vertices->size();
	desc.points.stride = sizeof(idk::Vertex);
	desc.points.data   = model.m_vertices->data();

	desc.triangles.count  = model.m_indices.size();
	desc.triangles.stride = 3*sizeof(uint32_t); 
	desc.triangles.data   = model.m_indices.data();

	physx::PxDefaultMemoryOutputStream writeBuffer;
	physx::PxTriangleMeshCookingResult::Enum result;

	physx::PxTolerancesScale scale;
	physx::PxCookingParams params(scale);

	physx::PxTriangleMesh *mesh = PxCreateTriangleMesh(params, desc);
	physx::PxTriangleMeshGeometry geometry(mesh);

	auto &pos = api_ptr->getECS().getComponent<idk::TransformCmp>(obj_id).position;
	physx::PxTransform t(toPX(pos));

	physx::PxRigidStatic *body = physx::PxCreateStatic(*gPhysics, t, geometry, *gMaterial);
	gScene->addActor(*body);

	return body;
}





physx::PxRigidDynamic *
idk::PhysXSys::createSphereCollider( const glm::vec3 &p, float r, const glm::vec3 &v )
{
    return createDynamic(PxTransform(PxVec3(0,40,100)), PxSphereGeometry(r), PxVec3(0,-50,-100));
}







void
idk::PhysXSys::init( idk::EngineAPI &api )
{
	api_ptr = &api;

	if (initialized())
	{
		return;
	}

	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
	gPhysics    = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true);

	physx::PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -0.81f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher	= gDispatcher;
	sceneDesc.filterShader	= PxDefaultSimulationFilterShader;
	gScene = gPhysics->createScene(sceneDesc);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	PxRigidStatic* groundPlane = physx::PxCreatePlane(
        *gPhysics, physx::PxPlane(physx::PxVec3(0, -15, 0), PxVec3(0, 1, 0)), *gMaterial
    );

	gScene->addActor(*groundPlane);


	is_initialized = true;
}




void
idk::PhysXSys::update( idk::EngineAPI &api )
{
	gScene->simulate(1.0f/60.0f);
	gScene->fetchResults(true);

    auto &ecs = api.getECS();

    for (auto &[obj_id, radius, body]: ecs.getComponentArray<idk::PhysXSphereRigidDynamicCmp>().data())
    {
        if (obj_id == -1 || body == nullptr)
        {
            continue;
        }

        physx::PxTransform P = body->getGlobalPose();
        ecs.getComponent<idk::TransformCmp>(obj_id).position = toGLM(P.p);
    }
}

void
idk::PhysXSys::exposeToLua( lua_State *LS )
{

}



bool
idk::PhysXSys::initialized()
{
	return is_initialized;
}



bool
idk::PhysXSys::isGrounded( int obj_id )
{
	return false;
}
