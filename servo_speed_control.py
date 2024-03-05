import machine
from machine import PWM, Pin

# Assuming the speed_servos are initialized in init_microcontroller.py and are globally accessible
# Import the global speed_servos variable
from init_microcontroller import speed_servos

# Function to set the speed of the car
# Speed value should be between -100 to 100, where negative values mean reverse direction
def set_speed(speed: int):
    # Constrain speed to be within -100 to 100
    speed = max(-100, min(100, speed))
    
    # Convert speed to servo duty. Assuming 0% duty cycle is full reverse, 50% is stop, and 100% is full forward.
    # These values might need to be adjusted based on the servo's behavior and the mechanical setup.
    if speed < 0:
        # Reverse direction
        duty = int(((50 - 0) / 100) * abs(speed) + 0)  # Map speed to 0-50 duty cycle range for reverse
    else:
        # Forward direction
        duty = int(((100 - 50) / 100) * speed + 50)  # Map speed to 50-100 duty cycle range for forward
    
    # Apply the duty cycle to both speed control servos
    for servo in speed_servos:
        servo.duty(duty)

# Example usage:
# set_speed(50)  # Move forward at 50% speed
# set_speed(-30)  # Move backward at 30% speed
# set_speed(0)  # Stop the car

