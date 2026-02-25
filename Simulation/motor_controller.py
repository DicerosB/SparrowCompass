import numpy as np
import matplotlib.pyplot as plt

class StepperMotorEmulator:
    def __init__(self,
                 steps_per_rev=200,
                 inertia=0.0005,
                 max_torque=0.5,
                 damping=0.01,
                 dt=0.001):

        self.steps_per_rev = steps_per_rev
        self.step_angle = 2 * np.pi / steps_per_rev

        self.J = inertia
        self.max_torque = max_torque
        self.damping = damping
        self.dt = dt

        # State variables
        self.theta = 0.0      # position (rad)
        self.omega = 0.0      # velocity (rad/s)

        # Target
        self.target_theta = 0.0

    def step(self, step_frequency):
        """
        step_frequency: commanded steps per second
        """

        # Convert step frequency to target velocity
        omega_target = step_frequency * self.step_angle

        # Velocity error
        velocity_error = omega_target - self.omega

        # Simple proportional torque model
        torque = 5.0 * velocity_error

        # Torque saturation
        torque = np.clip(torque, -self.max_torque, self.max_torque)

        # Apply damping
        torque -= self.damping * self.omega

        # Motor dynamics
        alpha = torque / self.J
        self.omega += alpha * self.dt
        self.theta += self.omega * self.dt

        return self.theta, self.omega


# ===============================
# Simulation
# ===============================

sim_time = 2.0
dt = 0.001
time = np.arange(0, sim_time, dt)

motor = StepperMotorEmulator(dt=dt)

position_log = []
velocity_log = []
command_log = []

# Example command: trapezoidal ramp
def step_command(t):
    if t < 0.5:
        return 400 * t      # ramp up
    elif t < 1.5:
        return 200          # constant speed
    else:
        return 200 - 400*(t-1.5)  # ramp down


for t in time:
    cmd = step_command(t)
    theta, omega = motor.step(cmd)

    position_log.append(theta)
    velocity_log.append(omega)
    command_log.append(cmd * motor.step_angle)

# ===============================
# Plotting
# ===============================

plt.figure(figsize=(12,8))

plt.subplot(2,1,1)
plt.plot(time, position_log)
plt.title("Stepper Motor Position")
plt.ylabel("Position (rad)")
plt.grid()

plt.subplot(2,1,2)
plt.plot(time, velocity_log, label="Actual velocity")
plt.plot(time, command_log, '--', label="Commanded velocity")
plt.title("Stepper Motor Velocity")
plt.ylabel("Velocity (rad/s)")
plt.xlabel("Time (s)")
plt.legend()
plt.grid()

plt.tight_layout()
plt.show()