extends Node

export var reward = 0.0
export var done = false

var actions = {
	"force": 0.0
}
var node

# Called when the node enters the scene tree for the first time.
func _ready():
	node = load("pendulum/Pendulum.tscn").instance()
	add_child(node)
	
	node.reset(self)

func _physics_process(delta):
	node.step(self)
	$RewardLabel.text = "reward: %.2f" % reward
	$DoneLabel.text = "done: {0}".format([done])
	

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
	
	actions["force"] = force * 0.5

func set_observation(name, value):
	if name == "angular_velocity":
		$VelocityLabel.text = "velocity: %.2f" % value
		pass

func get_action(name):
	return actions[name]
