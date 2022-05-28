extends GridContainer


# Declare member variables here. Examples:
# var a = 2
# var b = "text"
var views = []
var pendulum_scene = preload("res://pendulum/Pendulum.tscn")

# Called when the node enters the scene tree for the first time.
func _ready():
	columns = 2
	
	for _i in range(0, 4):
		var view_container = ViewportContainer.new()
		view_container.stretch = true
		view_container.size_flags_horizontal |= SIZE_EXPAND
		view_container.size_flags_vertical |= SIZE_EXPAND
		
		var view = Viewport.new()
		views.append(view)
		
		view.world = World.new()
		view.world_2d = World2D.new()
		view.own_world = true
		view.global_canvas_transform = view.global_canvas_transform.scaled(Vector2(0.5, 0.5))
		
		add_child(view_container)
		view_container.add_child(view)
		view.add_child(pendulum_scene.instance())

# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
