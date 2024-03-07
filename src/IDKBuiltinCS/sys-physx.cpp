#include "sys-physx.hpp"

#include "../../thirdparty/physx/include/PxPhysicsAPI.h"
#include "sys-planet.hpp"


glm::vec3
idk::toGLM( const physx::PxVec3 &v )
{
    return glm::vec3(v.x, v.y, v.z);
}


glm::vec3
idk::toGLM( const physx::PxExtendedVec3 &v )
{
	return glm::vec3(v.x, v.y, v.z);
}


glm::quat
idk::toGLM( const physx::PxQuat &pq )
{
    return glm::quat(pq.w, pq.x, pq.y, pq.z);
}


glm::mat4
idk::toGLM( const physx::PxTransform &P )
{
    glm::mat4 M = glm::mat4(1.0f);

    glm::vec3 position = toGLM(P.p);
    glm::quat rotation = toGLM(P.q);

    M[3] = glm::vec4(position, 1.0f);

    M = M * glm::mat4_cast(rotation);

    return M;
}



physx::PxVec3
idk::toPx( const glm::vec3 &v )
{
	return physx::PxVec3(v.x, v.y, v.z);
}


physx::PxQuat
idk::toPx( const glm::quat &r )
{
	return physx::PxQuat(r.x, r.y, r.z, r.w);
}


physx::PxTransform
idk::toPx( const glm::vec3 &position, const glm::quat &rotation )
{
	return physx::PxTransform(toPx(position), toPx(rotation));
}


physx::PxTransform
idk::toPx( const glm::mat4 &M )
{
	physx::PxTransform P;

	P.p = toPx(glm::vec3(M[3]));
	P.q = toPx(glm::normalize(glm::quat_cast(M)));

    return P;
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
static physx::PxControllerManager*		gControllerManager = NULL;


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
	auto &ren = api_ptr->getRenderer();
	int  model_id = api_ptr->getECS().getComponent<idk::ModelCmp>(obj_id).model_id;

	
	size_t num_vertices;
	std::unique_ptr<idk::Vertex_P_N_T_UV[]> vertices;
	ren.getVertices(model_id, num_vertices, vertices);

	size_t num_indices;
	std::unique_ptr<uint32_t[]> indices;
	ren.getIndices(model_id, num_indices, indices);


	physx::PxTriangleMeshDesc desc;
	desc.points.count  = num_vertices;
	desc.points.stride = sizeof(idk::Vertex_P_N_T_UV);
	desc.points.data   = vertices.get();

	desc.triangles.count  = num_indices;
	desc.triangles.stride = 3*sizeof(uint32_t); 
	desc.triangles.data   = indices.get();

	physx::PxDefaultMemoryOutputStream writeBuffer;
	physx::PxTriangleMeshCookingResult::Enum result;

	physx::PxTolerancesScale scale;
	physx::PxCookingParams params(scale);

	physx::PxTriangleMesh *mesh = PxCreateTriangleMesh(params, desc);
	physx::PxTriangleMeshGeometry geometry(mesh);
	geometry.scale.scale = physx::PxVec3(TransformSys::getData(obj_id).scale);

	auto &cmp = api_ptr->getECS().getComponent<idk::TransformCmp>(obj_id);
	physx::PxTransform t = toPx(TransformSys::getPositionWorldspace(obj_id), cmp.rotation);

	physx::PxRigidStatic *body = physx::PxCreateStatic(*gPhysics, t, geometry, *gMaterial);
	gScene->addActor(*body);

	return body;
}





physx::PxRigidDynamic *
idk::PhysXSys::createSphereCollider( int obj_id )
{
	glm::vec3 p = TransformSys::getPositionWorldspace(obj_id);

	PxVec3 position = toPx(p);
	PxQuat rotation = toPx(glm::quat(glm::vec3(0.0f)));
	physx::PxTransform transform = physx::PxTransform(position, rotation);

    return createDynamic(transform, PxSphereGeometry(0.5f), physx::PxVec3(0.0f));
}



physx::PxRigidStatic *
idk::PhysXSys::createRectStatic( int obj_id )
{
	glm::mat4 M     = TransformSys::getModelMatrix(obj_id);
	glm::vec3 scale = 0.5f * TransformSys::getData(obj_id).scale * TransformSys::getData(obj_id).scale3;

	physx::PxTransform   transform = toPx(M);
	physx::PxBoxGeometry geometry  = physx::PxBoxGeometry(scale.x, scale.y, scale.z);

	physx::PxRigidStatic *body = physx::PxCreateStatic(*gPhysics, transform, geometry, *gMaterial);
	gScene->addActor(*body);

	return body;
}




physx::PxController *
idk::PhysXSys::createCapsuleKinematic( int obj_id, float height, float radius )
{
	glm::vec3 position = TransformSys::getPositionWorldspace(obj_id);

	physx::PxCapsuleControllerDesc desc;

	desc.material   = gPhysics->createMaterial(0.25f, 0.25f, 0.85f);
	desc.position.x = position.x;
	desc.position.y = position.y;
	desc.position.z = position.z;
	desc.height     = height; // 1.75f;
	desc.radius     = radius; // 0.25f;
	desc.upDirection = toPx(TransformSys::getSurfaceUp(obj_id));

	physx::PxController *controller = gControllerManager->createController(desc);
	controller->setSlopeLimit(3.14159f / 4);

	return controller;
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
	sceneDesc.gravity = PxVec3(0.0f, 0.0f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher	= gDispatcher;
	sceneDesc.filterShader	= PxDefaultSimulationFilterShader;
	gScene = gPhysics->createScene(sceneDesc);
	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
	gControllerManager = PxCreateControllerManager(*gScene);

	is_initialized = true;
}



void
idk::PhysXSys::updateRigidDynamic( idk::EngineAPI &api )
{
    auto &ecs = api.getECS();

    for (auto &[obj_id, enabled, radius, body]: ecs.getComponentArray<idk::PhysXSphereRigidDynamicCmp>())
    {
        if (obj_id == -1 || !enabled)
        {
            continue;
        }

        physx::PxTransform P = body->getGlobalPose();

		glm::vec3 pos = toGLM(P.p);
		glm::vec3 G   = PlanetSys::averageGravity(pos);

		TransformSys::setPositionWorldspace(obj_id, pos);
		TransformSys::getData(obj_id).rotation = toGLM(P.q);

		body->addForce(toPx(G));
    }
}


void
idk::PhysXSys::updateRigidStatic( idk::EngineAPI &api )
{
    auto &ecs = api.getECS();

    for (auto &[obj_id, enabled, body]: ecs.getComponentArray<idk::PhysXMeshRigidStaticCmp>())
	{
		body->setGlobalPose(toPx(TransformSys::getModelMatrix(obj_id)));
    }

    for (auto &[obj_id, enabled, body]: ecs.getComponentArray<idk::PhysXRectRigidStaticCmp>())
	{
		physx::PxShape *shapes[1];
		body->getShapes(shapes, 1, 0);

		glm::mat4 M     = TransformSys::getModelMatrix(obj_id);
		glm::vec3 scale = 0.5f * TransformSys::getData(obj_id).scale * TransformSys::getData(obj_id).scale3;

		physx::PxTransform   transform = toPx(M);
		physx::PxBoxGeometry geometry  = physx::PxBoxGeometry(scale.x, scale.y, scale.z);

		shapes[0]->setGeometry(geometry);
		body->setGlobalPose(transform);
	}

}


void
idk::PhysXSys::updateKinematicController( idk::EngineAPI &api )
{
	auto &engine = api.getEngine();
    auto &ecs    = api.getECS();

	float dtime = engine.deltaTime();

    for (auto &[obj_id, enabled, height, radius, velocity, controller]: ecs.getComponentArray<idk::PhysXKinematicControllerCmp>())
    {
		if (enabled == false)
		{
			continue;
		}

		glm::vec3 position = toGLM(controller->getPosition());

		float 	  G     = 0.0f;
		glm::vec3 up    = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 delta = glm::vec3(0.0f);


		if (PlanetSys::actorHasPlanet(obj_id))
		{
			G  = ecs.getComponent<PlanetCmp>(ecs.getParent(obj_id)).gravity;
			up = TransformSys::getSurfaceUp(obj_id);

			velocity *= 0.985f;
			velocity += G * -up * dtime;
		}

		else
		{
			G = 0.0f;
			up = TransformSys::getUp(obj_id);
		}


		physx::PxVec3 px_up = toPx(up);
		controller->setUpDirection(px_up);

		physx::PxControllerFilters filter;
		controller->move(toPx(velocity), 0.0f, dtime, filter);

		TransformSys::setPositionWorldspace(obj_id, position);
    }
}



void
idk::PhysXSys::update( idk::EngineAPI &api )
{
	auto &engine = api.getEngine();
	const float dtime = engine.deltaTime();

	PhysXSys::updateRigidDynamic(api);
	PhysXSys::updateRigidStatic(api);
	PhysXSys::updateKinematicController(api);
	_updateCallbacks();

	gScene->simulate(dtime);
	gScene->fetchResults(true);

}



void
idk::PhysXSys::exposeToLua( lua_State *LS )
{
	luaaa::LuaModule mod(LS, "PhysXSys");

	mod.fun("moveCCT", [](int id, float x, float y, float z) {
		PhysXSys::moveCCT(id, x, y, z);
	});

	mod.fun("moveTowardsCCT", PhysXSys::moveTowardsCCT);

	mod.fun("hasCCT", PhysXSys::hasCCT);

}



bool
idk::PhysXSys::initialized()
{
	return is_initialized;
}



bool
idk::PhysXSys::isGrounded( int obj_id )
{
	auto &ecs = api_ptr->getECS();
	auto &cmp = ecs.getComponent<idk::PhysXKinematicControllerCmp>(obj_id);

	glm::vec3 position = TransformSys::getPositionWorldspace(obj_id);
			  position -= (cmp.height / 2.0f + 0.1f) * TransformSys::getSurfaceUp(obj_id);

	physx::PxVec3 origin  = toPx(position);
	physx::PxVec3 dir     = toPx(-TransformSys::getSurfaceUp(obj_id));
	physx::PxReal maxdist = 32.0f;

	physx::PxRaycastBuffer callback;
	bool hit = gScene->raycast(origin, dir, maxdist, callback);

	if (hit)
	{
		return callback.block.distance < 0.2f;
	}

	return false;
}


void
idk::PhysXSys::addForce( int obj_id, const glm::vec3 &force )
{
	auto &ecs = api_ptr->getECS();
	auto &cmp = ecs.getComponent<idk::PhysXSphereRigidDynamicCmp>(obj_id);
	cmp.body->addForce(toPx(force));
}


bool
idk::PhysXSys::hasCCT( int obj_id )
{
	return api_ptr->getECS().hasComponent<idk::PhysXKinematicControllerCmp>(obj_id);
}


void
idk::PhysXSys::moveCCT( int obj_id, float x, float y, float z )
{
	auto &engine = api_ptr->getEngine();
	auto &ecs    = api_ptr->getECS();
	auto &cmp    = ecs.getComponent<idk::PhysXKinematicControllerCmp>(obj_id);

	glm::vec3 up    = TransformSys::getSurfaceUp(obj_id);
	glm::vec3 right = TransformSys::getSurfaceRight(obj_id);
	glm::vec3 front = TransformSys::getSurfaceFront(obj_id);

	glm::vec3 delta = y*up + x*right + z*front;
	cmp.velocity += delta * engine.deltaTime();

}


void
idk::PhysXSys::enableCCT( int obj_id )
{
	api_ptr->getECS().getComponent<idk::PhysXKinematicControllerCmp>(obj_id).enabled = true;
}


void
idk::PhysXSys::disableCCT( int obj_id )
{
	api_ptr->getECS().getComponent<idk::PhysXKinematicControllerCmp>(obj_id).enabled = false;
}



float
idk::PhysXSys::moveTowardsCCT( int subject, int target, float alpha )
{
	auto &engine = api_ptr->getEngine();
	auto &ecs    = api_ptr->getECS();
	auto &cmp    = ecs.getComponent<idk::PhysXKinematicControllerCmp>(subject);

	glm::vec3 dir = TransformSys::getPositionWorldspace(target) - TransformSys::getPositionWorldspace(subject);
			  dir = glm::normalize(dir);

	cmp.velocity += alpha * engine.deltaTime() * dir;

    return TransformSys::getDistanceWorldspace(subject, target);
}



void
idk::PhysXSys::moveTowardsCallbackCCT( int subject, int target, float alpha, float stop,
									   std::function<void()> callback )
{
	PhysXCallback PC = { subject, target, alpha, stop, callback };
	m_move_callbacks[subject] = PC;
}


void
idk::PhysXSys::_updateCallbacks()
{
    static std::unordered_set<int> finished_callbacks;
    finished_callbacks.clear();

    for (auto &[obj_id, TC]: m_move_callbacks)
    {
        float delta = moveTowardsCCT(TC.subject, TC.target, TC.alpha);

        if (delta <= TC.stop)
        {
            TC.callback();
            finished_callbacks.insert(obj_id);
        }
    }

    for (int obj_id: finished_callbacks)
    {
        m_move_callbacks.erase(obj_id);
    }
}
