# Design

#### Agent:

​	Creates an instance of the environment and communicates with it in a request reply loop. The Agent class must first connect to the actual environment instance that was created, then it receives data describing the action space and the observation space. It follows the form setup by openai gym.

```
```



#### Environment:

​	Instance of the Godot game engine running the actual environment itself. The environment must define the action space and the observation space, then register itself with the EnvironmentNode. It must also have the methods 'reset' and 'step'.

```python
var Env = get_node("EnvironmentNode")

# The reward value for the current step. Set this every step to drive the learning process.
Env.reward = 0.0

# Set this to true when you determine the environment should be reset
Env.done = false

Env.define_action_space({
		# Force to apply, negative -1 is clockwise, 1 is counter clockwise
		"force": {
			"type": "real",
			"range": [-1,1],
			"dims": [1,1,1,1]
		}
	})

Env.define_observation_space({
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
	})

# Used to get the action data sent by the agent.
var force = Env.get_action("force")

# Used to write the observation data to return to the agent.
Env.set_observation("x", 0.0)
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
- [ ] Test the exec function

