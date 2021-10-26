extends Node2D


var pendulum_reset_position = Vector2()
var arm_length = 0


# Called when the node enters the scene tree for the first time.
func _ready():
	pendulum_reset_position = $Pendulum/Arm.position
	arm_length = ($Pendulum/Arm.position - $Pendulum/Pin.position).length() + 0.005

func compute_observation(Env):
	var arm = $Pendulum/Arm.position - $Pendulum/Pin.position
	var x = clamp(arm.x / arm_length, -1.0, 1.0)
	var y = clamp(-arm.y / arm_length, -1.0, 1.0)
	
	Env.set_observation("x", x)
	Env.set_observation("y", y)
	Env.reward = y
	Env.done = false

func step(Env):
	compute_observation(Env)
	
	var force = Env.get_action("force")
	$Pendulum/Arm.angular_velocity += force

func reset(Env):
	$Pendulum/Arm.angular_velocity = 0
	$Pendulum/Arm.linear_velocity = Vector2(0,0)
	$Pendulum/Arm.rotation = 0.0
	$Pendulum/Arm.position = pendulum_reset_position
	
	compute_observation(Env)
