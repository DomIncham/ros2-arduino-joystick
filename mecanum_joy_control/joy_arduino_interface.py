import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Joy
import serial
import time
import struct
import sys 

class JoyArduinoInterface(Node):
    def __init__(self):
        super().__init__('joy_arduino_interface')
        self.subscription = self.create_subscription(
            Joy,
            'joy',
            self.joy_callback,
            10)
        try:
            self.serial_port = serial.Serial('/dev/ttyACM0', 115200, timeout=1)
            time.sleep(2)  # Wait for Arduino to reset
        except serial.SerialException as e:
            self.get_logger().error(f'Failed to open serial port: {e}')
            sys.exit(1)
        
        self.last_send_time = 0
        self.send_interval = 20  # milliseconds


    def joy_callback(self, msg):

        current_time = self.get_clock().now().nanoseconds / 1000000  # แปลงเป็นมิลลิวินาที
        if (current_time - self.last_send_time) < self.send_interval:
            return  # Skip sending to maintain rate

        # Assuming left stick for x and y, right stick for rotation
        Rotate = msg.axes[0]  # analog บนซ้าย จอยเบส
        x = msg.axes[2]       # analog ขวาล่างจอยเบส
        y = msg.axes[3]       # analog ขวาล่างจอยเบส
    
        # Apply dead zone
        dead_zone = 0.1
        Rotate = 0 if abs(Rotate) < dead_zone else Rotate
        x = 0 if abs(x) < dead_zone else x
        y = 0 if abs(y) < dead_zone else y

        # Scale to range -255 to 255
        Rotate = int(Rotate * 255)
        x = int(x * 255)
        y = int(y * 255)
        checksum = 0

        # Calculate checksum (simple sum)
        checksum = (Rotate + x + y) # Ensure it's a short

        # Pack data with checksum
        data = struct.pack('hhhh', Rotate, x, y, checksum)
        try:
            self.serial_port.write(data)
            self.get_logger().debug(f'Sent data: Rotate={Rotate}, x={x}, y={y}, checksum={checksum}')
        except serial.SerialException as e:
            self.get_logger().error(f'Failed to write to serial port: {e}')

        self.last_send_time = current_time


def main(args=None):
    rclpy.init(args=args)
    joy_arduino_interface = JoyArduinoInterface()
    rclpy.spin(joy_arduino_interface)
    joy_arduino_interface.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()