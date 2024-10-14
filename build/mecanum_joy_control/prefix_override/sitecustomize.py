import sys
if sys.prefix == '/usr':
    sys.real_prefix = sys.prefix
    sys.prefix = sys.exec_prefix = '/home/jacknipat/mecanum_joy_control/install/mecanum_joy_control'
