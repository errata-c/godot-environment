extends Node2D

var thread: Thread

# Called when the node enters the scene tree for the first time.
func _ready():
	$Pendulum.define_spaces($Env)
	$Env.register_scene($Pendulum)
	$Env.set_process(false)
	
	pause_mode = Node.PAUSE_MODE_PROCESS
	get_tree().paused = true

func _input(event):
	if !get_tree().paused:
		return
	
	if event is InputEventKey:
		get_tree().paused = false	
		setup()

func setup():
	$Env.set_process(true)
	
	$Env.set_recv_timeout(10*60*1000)
	$Env.set_send_timeout(10*60*000)
	$Env.set_connect_timeout(10*60*1000)
	$Env.connect_agent(2048)
	
	thread = Thread.new()
	thread.start(self, "_thread_func")

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _thread_func(udata):
	var agent = load("res://bin/test_agent.gdns").new()
	var value_wrapper = load("res://bin/value_wrapper.gdns")
	
	agent.connect_environment(2048)
	
	var step = agent.reset()
	if step.empty():
		return
		
	var action_space = {
		"force": value_wrapper.new()
	}
	action_space.force.reset(1, "f64", 1.0)
	
	var start = Time.get_ticks_msec()
	while (Time.get_ticks_msec() - start) < 30000:
		step = agent.step(action_space)
		if step.empty():
			return

func _exit_tree():
	if thread != null:
		thread.wait_to_finish()
