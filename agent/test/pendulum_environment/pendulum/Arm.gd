extends RigidBody2D


export var external_force = 0.0

func _integrate_forces(state):
	state.angular_velocity = clamp(
		state.angular_velocity + external_force, 
		-8.0, 
		8.0)
	
	external_force = 0.0
