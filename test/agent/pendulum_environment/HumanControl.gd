extends Node

var node

# Called when the node enters the scene tree for the first time.
func _ready():
	$Env.human_control = true
	
	$Pendulum.define_spaces($Env)
	$Pendulum.reset($Env)

func _process(delta):
	$Pendulum.step($Env)
	
	$RewardLabel.text = "reward: %.2f" % $Env.reward
	$DoneLabel.text = "done: {0}".format([$Env.done])
	$VelocityLabel.text = "velocity: %.2f" % $Env.observation_space["angular_velocity"].get_flat(0)

var leftHeld = false
var rightHeld = false
func _input(event):
	if event.is_action_pressed("ui_left"):
		leftHeld = true
	elif event.is_action_pressed("ui_right"):
		rightHeld = true
	
	if event.is_action_released("ui_left"):
		leftHeld = false
	elif event.is_action_released("ui_right"):
		rightHeld = false
	
	var force = 0.0
	if leftHeld:
		force += 1.0
	elif rightHeld:
		force -= 1.0
	
	$Env.action_space["force"].set_flat(0, force * 0.5)
