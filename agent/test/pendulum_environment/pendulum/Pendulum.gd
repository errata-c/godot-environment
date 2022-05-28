extends Node2D

var episode = -1
var step_count = 0

var angle = 0.0
var angular_velocity = 0.0
var torque = 0.0

# Method to compute the observation for the scene, not a part of the interface
func compute_observation(Env):
	var x = cos(angle)
	var y = sin(angle)
	var vel = angular_velocity
	
	Env.set_observation("x", x)
	Env.set_observation("y", y)
	Env.set_observation("angular_velocity", vel)
	
	var norm_angle = atan2(y,x)
	
	# This imposes an action cost for the torque
	# It also makes it so that only a still upright position is 
	# high reward, not just passing by the upright spot.
	Env.reward = -16.2736044 + (norm_angle * norm_angle + 0.1 * vel * vel + 0.001 * torque * torque)
	Env.done = step_count > 600

# Interface method, called by the EnvironmentNode
func step(Env):
	var dt = 1.0 / 60.0
	var g = -10.0
	
	torque = Env.get_action("force") * 2.0
	torque = clamp(torque, -2.0, 2.0)
	
	var nthdot = angular_velocity + (3.0 * g / 2.0 * sin(angle) + 3.0 * torque) * dt
	nthdot = clamp(nthdot, -8.0, 8.0)
	
	var nth = angle + nthdot * dt
	
	angle = nth
	angular_velocity = nthdot
	
	$Arm.rotation = angle
	
	step_count += 1
	
	compute_observation(Env)

# Interface method, called by the EnvironmentNode
func reset(Env):
	step_count = 0
	episode += 1
	
	angular_velocity = randf() * 16.0 - 8.0
	angle = randf() * PI * 2.0 - PI
	$Arm.rotation = angle
	
	compute_observation(Env)
