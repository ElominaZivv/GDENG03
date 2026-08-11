#include <JAZZY/Components/RigidBodyComponent.h>

jazzy::RigidBodyComponent::RigidBodyComponent(const ComponentDesc& data) : Component(data)
{
	Vector3 position(0.0, 0.0, 0.0);
	Quaternion orientation = Quaternion::identity();
	Transform transform(position, orientation);
	m_rigidBody = data.worldPhysics->createRigidBody(transform);
	//DX3DLogInfo("Rigidbody successfully instantiated. Transform: {}", m_rigidBody->getTransform().to_string().c_str());
}
