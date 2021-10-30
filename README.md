# Q_dxl

This example is created for testing:
-  Serial connection
-  Testing two dynamixel motors (eg. MX-28AT)
-  Doing the conversion from absolute encoder (of dynamixel motors) to incremental encoder

With this, one can use the dynamixel motors for several applications:
-  Control robotic arms (serial or parallel types) - which often use absolute encoders
-  Control autonomous mobile robots - which often use incremental encoders
   (eg. publishing odometry in ROS-based applications)

- **Author: N.D.Quan - 30/10/2021**

# Environment / Requirements:
- Qt C++ (eg. 5.14)
- OpenCm9.04 board : for interfacing with Robotis dynamixel motors
- TWO dynamixel motors (this example is using two motors)

# Usage:
- Press "Connect" button for open serial port (the one of OpenCM board)
- Press "Send Cmd" to send command (input to LineEdit). The format data is: "Value1#Value2#" , where Value1, Value2 are the goal values (desired position or speed) in Position Mode or Wheel Mode.
- Press corresponding buttons for the working modes. Free Drive Mode is for collecting encoder values only (eg. teaching mode)
- The feedback sent from OpenCM board has the format: "Q1,Q2,QP1,QP2\r". Where Q1,Q2 are absolute encoder values in position of dynamixel motors. QP1,QP2 are velocities of the two motors.
- Incremental encoder values will start at (0,0) when a serial port is openned. 

> Q1,Q2 are in the range of [0, 4095] (12 bit absolute encoder). For MX-28AT motors, unit in position = 0.088 deg

> QP1, QP2 are in the range of [0, 1023] for positive direction (CCW) and ([1024,2047] for negative direction (CW). For MX-28AT motors, unit in speed = 0.114 rpm
