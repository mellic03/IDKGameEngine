// btRigidBody* BulletWrap::addTriangleMesh( btVector3 &p0, btVector3 &p1, btVector3 &p2,btVector3 &position,
// 									bool useQuantizedBvhTree,btVector3 &inertia, btScalar mass,
// 									btScalar restitution, bool collision )
// {
// 	btTriangleMesh* trimesh = new btTriangleMesh();
// 	trimesh->addTriangle( p0, p1, p2 );
	
// 	btTransform	trans;
// 	trans.setIdentity();
// 	trans.setOrigin( position );

// 	btCollisionShape* trimeshShape  = new btBvhTriangleMeshShape( trimesh, useQuantizedBvhTree );
// 	trimeshShape->calculateLocalInertia( mass, inertia ); //gives error
	
// 	btDefaultMotionState* motionstate = new btDefaultMotionState( trans );

// 	btRigidBody* body= new btRigidBody( mass, motionstate, trimeshShape, inertia );
// 	body->setRestitution( restitution );
	
// 	m_trianglemeshs.push_back( trimesh );
// 	m_triangleMeshBodies.push_back( body );
// 	m_world->addRigidBody( body );
	
// 	std::vector< btVector3* > tmp;
// 	m_vertices.push_back(tmp);
// 	m_vertices[ m_triangleMeshBodies.size() - 1 ].push_back( &p0 );
// 	m_vertices[ m_triangleMeshBodies.size() - 1 ].push_back( &p1 );
// 	m_vertices[ m_triangleMeshBodies.size() - 1 ].push_back( &p2 );
	
// 	if( collision )
// 		m_collisionShapes.push_back(trimeshShape);
	
// 	//return m_triangleMeshBodies.size() - 1;
// 	return body;
// }