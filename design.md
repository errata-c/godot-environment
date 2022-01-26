# Design

​	Everything described here is can be seen in action using the `agent/test/pendulum_environment` godot project. The library must be compiled first, and the native script files must also be installed to the correct location (`agent/test/pendulum_environment/bin`) for it to run properly.

#### Agent:

​	Creates an instance of the environment and communicates with it in a request reply loop. The Agent class must first connect to the actual environment instance that was created, then it receives data describing the action space and the observation space. It follows the form setup by OpenAI gym.

```
```



## Environment:

​	In order to define the environment a specific project structure should be followed. The main scene must have a script in place to define the observation and action spaces, and it must have an `EnvironmentNode` instance.  The `EnvironmentNode` is a native script class defined in this library, so the native library must be installed prior to use.

```python
# This is an example setup script to be put in the main scene.
# The code here is derived from 'agent/test/pendulum_environment/Setup.gd'

# The node heirarchy here is:
# Setup (Node)
# 	|-> Env (EnvironmentNode)

extends Node

# Called when the node enters the scene tree for the first time.
func _ready():	
	# The definition of the actions that can be taken by the agent
	var act = {
		# Force to apply, -1 is clockwise, +1 is counter clockwise
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
		}
	}
	
    # These methods MUST be called prior to registering a scene
	$Env.define_action_space(act)
	$Env.define_observation_space(obs)
	
    # Simple utility/debug methods that print the definitions to the console.
	print("Actions:")
	$Env.print_action_space_def()
	
	print("\nObservations:")
	$Env.print_observation_space_def()
	
    # The actual scene file containing the implementation of the environment.
    # This must be a separate scene file so that the agent can decide how many
    #  instances of it to run.
	$Env.register_scene("res://PendulumEnv.tscn")
```

Once the main scene has been setup, the actual environment must be built. You are free to implement the entirety of it in GDScript, as was done with the pendulum test environment. In order for the scene to be properly registered and used by the `EnvironmentNode`, the root node of the scene must provide a `step` and `reset` method. The `step` method should get the action values from the `EnvironmentNode` using the `get_action` method, and set the observation values using the `set_observation` method. The `reset` method should put the scene into some starting state, and compute the initial observation. Both methods accept only one argument, which will be a reference to the `EnvironmentNode`.

```python
# This is an example environment implementation
# See the 'agent/test/pendulum_environment' godot project for a more complete picture
# The code here is derived from 'agent/test/pendulum_environment/PendulumEnv.gd'
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
```

#### Testing

​	To test the environment scene, a scene can be created that implements the interface of `EnvironmentNode` and mimics its behavior.  That test scene must implement the `set_observation` and `get_action` methods. Then it should instance the actual environment scene, add it as a child to make sure it is actually initialized, and call the `reset` method to prepare the scene. Then the test scene should either call `step` repeatedly in the `_physics_process` method if the environment is a physics based one, or call `step` once the user makes an action if the environment is lockstep like a classic grid world environment.

```python
# This is an example test scene script
# See the 'agent/test/pendulum_environment' godot project for a more complete picture
# The code here is derived from 'agent/test/pendulum_environment/HumanControl.gd'
extends Node

export var reward = 0.0
export var done = false

var actions = {
	"force": 0.0
}
var node

func _ready():
    # Instance the environment scene and add it to the scene tree as a child.
    # If you don't add it to the scene, things will likely break.
	node = load("PendulumEnv.tscn").instance()
	add_child(node)
	
    # Call reset to prepare the scene for the first run.
    # Pass self in place of the EnvironmentNode object.
	node.reset(self)

# This scene is physics based, so the step method should be called here.
func _physics_process(delta):
	node.step(self)
	$RewardLabel.text = "reward: {0}".format([reward])
	$DoneLabel.text = "done: {0}".format([done])

# The inputs must be processed and put into the proper action value.
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
		force += 0.125
	elif rightHeld:
		force -= 0.125
	
	actions["force"] = force

# This method must exist, but it does not have to do anything.
# It may be good to put some test logic here to validate the observation values.
func set_observation(name, value):
	pass

# This method must exist and return the requested action value.
# It is the responsibility of the environment implementation to ask only for 
#  defined actions and observations.
func get_action(name):
	return actions[name]

```





## Communication:

- Agent starts the executable
- Environment connects
- Agent requests definitions of the observation space and the action space
- Agent sends reset, environment returns the initial step data
- (Loop here until done) Agent sends an action, environment returns the resulting step data
- Agent sends close, environment closes the Godot executable



## Todo:

- [x] Prepare for creating multiple instances of an environment for faster training
- [x] Decide how the Environment class will interact with the agent
- [x] Test the exec function on windows
- [ ] Test the exec function on Linux
