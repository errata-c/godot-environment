extends Node2D

# Called when the node enters the scene tree for the first time.
func _ready():
	$Pendulum.define_spaces($Env)
	$Env.register_scene($Pendulum)
	$Env.setup()
