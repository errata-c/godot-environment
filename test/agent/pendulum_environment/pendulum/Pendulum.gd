extends Node2D

var episode = -1
var step_count = 0

var angle = 0.0
var angular_velocity = 0.0
var torque = 0.0

func define_spaces(Env):
	# The definition of the actions that can be taken by the agent
	var act = {
		# Force to apply, negative -1 is clockwise, 1 is counter clockwise
		"force": {
			"type": "f64",
			"range": [-1,1],
			"dims": [1,1,1,1]
		}
	}
	
	# The definition of the observations that will be sent to the agent
	var obs = {
		"x": {
			"type": "f64",
			"range": [-1, 1],
			"dims": [1,1,1,1]
		},
		"y": {
			"type": "f64",
			"range": [-1, 1],
			"dims": [1,1,1,1]
		},
		"angular_velocity": {
			"type": "f64",
			"range": [-8, 8],
			"dims": [1,1,1,1]
		}
	}
	
	Env.define_action_space(act)
	Env.define_observation_space(obs)

# Method to compute the observation for the scene, not a part of the interface
func compute_observation(Env):
	var x = cos(angle)
	var y = sin(angle)
	var vel = angular_velocity
	
	var obs = Env.observation_space
	
	obs["x"].set_flat(0, x)
	obs["y"].set_flat(0, y)
	obs["angular_velocity"].set_flat(0, vel)
	
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
	
	torque = Env.action_space["force"].get_flat(0) * 2.0
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
