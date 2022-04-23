extends Node

# Called when the node enters the scene tree for the first time.
func _ready():	
	# The definition of the actions that can be taken by the agent
	var act = {
		# Force to apply, negative -1 is clockwise, 1 is counter clockwise
		"force": {
			"type": "real",
			"range": [-1,1],
			"dims": [1,1,1,1]
		}
	}
	
	# The definition of the observations that will be sent to the agent
	var obs = {
		"x": {
			"type": "real",
			"range": [-1, 1],
			"dims": [1,1,1,1]
		},
		"y": {
			"type": "real",
			"range": [-1, 1],
			"dims": [1,1,1,1]
		},
		"angular_velocity": {
			"type": "real",
			"range": [-8, 8],
			"dims": [1,1,1,1]
		}
	}
	
	$Env.define_action_space(act)
	$Env.define_observation_space(obs)
	
	print("Actions:")
	$Env.print_action_space_def()
	
	print("\nObservations:")
	$Env.print_observation_space_def()
	
	$Env.register_scene("res://pendulum/Pendulum.tscn")
