# Design

#### Agent:

​	Creates an instance of the environment and communicates with it in a request reply loop. The Agent class must first connect to the actual environment instance that was created, then it receives data describing the action space and the observation space. To interact with Godot, the agent instances the action space, and fills it with the desired actions for the current time step. The Space object is then passed over using the `Step step(const Space& action)` method.

#### Environment:

​	Instance of the Godot game engine running the actual environment itself. In Godot, it is implemented as the EnvironmentNode class. This class receives requests from the agent and generates signals for type of request.

```python
#This code is from the godot-environment-test repo.

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

- [ ] Prepare for creating multiple instances of an environment for faster training
- [ ] Decide how the Environment class will interact with the agent
- [ ] Test the exec function

