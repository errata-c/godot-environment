extends Node2D

var episode = -1
var step_count = 0
var torque = 0.0

# Method to compute the observation for the scene, not a part of the interface
func compute_observation(Env):
	var arm = $Arm.position - $Pin.position
	arm = arm.normalized()
	var x = arm.x
	var y = -arm.y
	
	var vel = $Arm.angular_velocity
	
	Env.set_observation("x", x)
	Env.set_observation("y", y)
	Env.set_observation("angular_velocity", vel)
	
	# This imposes an action cost for the torque
	# It also makes it so that only a still upright position is 
	# high reward, not just passing by the upright spot.
	Env.reward = y - (vel * vel * 0.1 + 0.001 * torque * torque)
	Env.done = step_count > 600

# Interface method, called by the EnvironmentNode
func step(Env):
	compute_observation(Env)
	
	torque = Env.get_action("force") * 0.125
	$Arm.external_force = torque
	step_count += 1

# Interface method, called by the EnvironmentNode
func reset(Env):
	step_count = 0
	episode += 1
	
	$Arm.angular_velocity = randf() * 16.0 - 8.0
	$Arm.rotation = randf() * PI * 2.0 - PI
	
	compute_observation(Env)
