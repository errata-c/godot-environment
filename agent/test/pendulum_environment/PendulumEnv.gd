extends Node2D


var pendulum_reset_position = Vector2()
var arm_length = 0


func _ready():
	pendulum_reset_position = $Pendulum/Arm.position
	arm_length = ($Pendulum/Arm.position - $Pendulum/Pin.position).length() + 0.005

# Method to compute the observation for the scene, not a part of the interface
func compute_observation(Env):
	var arm = $Pendulum/Arm.position - $Pendulum/Pin.position
	var x = clamp(arm.x / arm_length, -1.0, 1.0)
	var y = clamp(-arm.y / arm_length, -1.0, 1.0)
	
	Env.set_observation("x", x)
	Env.set_observation("y", y)
	Env.reward = y
	Env.done = false

# Interface method, called by the EnvironmentNode
func step(Env):
	compute_observation(Env)
	
	var force = Env.get_action("force")
	$Pendulum/Arm.angular_velocity += force

# Interface method, called by the EnvironmentNode
func reset(Env):
	$Pendulum/Arm.angular_velocity = 0
	$Pendulum/Arm.linear_velocity = Vector2(0,0)
	$Pendulum/Arm.rotation = 0.0
	$Pendulum/Arm.position = pendulum_reset_position
	
	compute_observation(Env)
