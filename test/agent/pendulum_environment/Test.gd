extends Node

var builtin_type_names = ["nil", "bool", "int", "real", "string", "vector2", "rect2", "vector3", "maxtrix32", "plane", "quat", "aabb",  "matrix3", "transform", "color", "image", "nodepath", "rid", null, "inputevent", "dictionary", "array", "rawarray", "intarray", "realarray", "stringarray", "vector2array", "vector3array", "colorarray", "unknown"]

# Called when the node enters the scene tree for the first time.
func _ready():
	var cls = load("res://bin/value_wrapper.gdns")
	var wrapper = cls.new()
	
	wrapper.reset([5, 2], "f32", range(10))
	
	print(wrapper)
	
	wrapper.fill(0.5)
	
	print(wrapper)

	get_tree().quit(0)
