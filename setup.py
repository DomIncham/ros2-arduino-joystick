from setuptools import find_packages, setup

package_name = 'mecanum_joy_control'

setup(
    name=package_name,
    version='0.0.1',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='jacknipat',
    maintainer_email='jacknipat@todo.todo',
    description='Mecanum wheel control using ROS2 Joy and Arduino',
    license='MIT',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'joy_arduino_interface = mecanum_joy_control.joy_arduino_interface:main',
        ],
    },
)
