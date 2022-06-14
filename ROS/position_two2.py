#!/usr/bin/env python
#################################################################################
# Copyright 2018 ROBOTIS CO., LTD.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#################################################################################

# Authors: Gilbert #

import rospy
from geometry_msgs.msg import Twist, Point, Quaternion
import tf
from math import radians, copysign, sqrt, pow, pi, atan2
from tf.transformations import euler_from_quaternion
import numpy as np
import time

msg = """
control your Turtlebot3!
-----------------------
Insert xyz - coordinate.
x : position x (m)
y : position y (m)
z : orientation z (degree: -180 ~ 180)
If you want to close, insert 's'
-----------------------path_angle
"""


class GotoPoint():
    test =0
    def __init__(self):
        
        rospy.init_node('turtlebot3_pointop_key', anonymous=False)
        rospy.on_shutdown(self.shutdown)
        self.cmd_vel = rospy.Publisher('cmd_vel', Twist, queue_size=5)

        #tetrix 용 pub
        self.robot_odr_cmd= rospy.Publisher('robot_odr_cmd', Twist, queue_size=5)
        position = Point()
        move_cmd = Twist()

        #tetrix 용 pub
        robot_odr = Twist()
        r = rospy.Rate(10)
        self.tf_listener = tf.TransformListener()
        self.odom_frame = 'odom'

        try:
            
            self.tf_listener.waitForTransform(
                self.odom_frame, 'base_footprint', rospy.Time(), rospy.Duration(1.0))
            self.base_frame = 'base_footprint'
        except (tf.Exception, tf.ConnectivityException, tf.LookupException):
            try:
                self.tf_listener.waitForTransform(
                    self.odom_frame, 'base_link', rospy.Time(), rospy.Duration(1.0))
                self.base_frame = 'base_link'
            except (tf.Exception, tf.ConnectivityException, tf.LookupException):
                rospy.loginfo(
                    "Cannot find transform between odom and base_link or base_footprint")
                rospy.signal_shutdown("tf Exception")

        (position, rotation) = self.get_odom()
        last_rotation = 0
        linear_speed = 1
        angular_speed = 1

        linear_speed_robot = 1
        angular_speed_robot = 1
        (goal_x, goal_y, goal_z) = self.getkey()
        if goal_z > 180 or goal_z < -180:
            print("you input wrong z range.")
            self.shutdown()
        goal_z = np.deg2rad(goal_z)
        goal_distance = sqrt(pow(goal_x - position.x, 2) +
                             pow(goal_y - position.y, 2))
        distance = goal_distance
        
        
        (position, rotation) = self.get_odom()
        x_start = position.x
        y_start = position.y
        
        path_angle = atan2(goal_y - y_start, goal_x - x_start)
        # path_angle = np.rad2deg(path_angle)
        # rotation = np.rad2deg(rotation)
        print("path_angle = ",path_angle)
        print("rotation = ",rotation)

        
        if(path_angle*rotation < 0):
            (print("minus"))
            path_angle = -path_angle

            while abs((path_angle + rotation)) >= 0.05:
                
                
                print("In1")
               

                (position, rotation) = self.get_odom()
                # rotation = np.rad2deg(rotation)
                
                print("path_angle = ",path_angle)
                print("rotation = ",rotation)

                move_cmd.linear.x = 0.00
                move_cmd.angular.z = -0.25

                
                robot_odr.linear.x = 0.00
                robot_odr.angular.z= -0.25

                self.cmd_vel.publish(move_cmd)

                #tetrix 용 pub
                self.robot_odr_cmd.publish(robot_odr)
                print("=",abs((path_angle + rotation)))
                r.sleep()
            
        
        else:


    
            
            while (abs(path_angle + rotation)) >= 0.05:
            
                print("In2")
                
                
                (position, rotation) = self.get_odom()
                # rotation = np.rad2deg(rotation)
                
                print("path_angle = ",path_angle)
                print("rotation = ",rotation)

                move_cmd.linear.x = 0.00
                move_cmd.angular.z = -0.25

                robot_odr.linear.x = 0.00
                robot_odr.angular.z= -0.25
                

                self.cmd_vel.publish(move_cmd)

                #tetrix 용 pub
                self.robot_odr_cmd.publish(robot_odr)
                print("=",abs(path_angle + rotation))
            
            
                r.sleep()

        #탈출후 멈춤 -179.6968648451245


       # move_cmd.linear.x = 0
       # move_cmd.angular.z = 0
        #self.robot_odr_cmd.publish(robot_odr)
       
        #time.sleep(2)

        while distance > 0.1:
            (position, rotation) = self.get_odom()
            x_start = position.x
            y_start = position.y
            print("distance = ",distance)
            501454810
            move_cmd.angular.z=0
            move_cmd.linear.x = 0.1

            robot_odr.angular.z=0
            robot_odr.linear.x = 0.1




            distance = sqrt(pow((goal_x - x_start), 2) +
                            pow((goal_y - y_start), 2))
            



            self.cmd_vel.publish(move_cmd)

             #tetrix 용 pub
            self.robot_odr_cmd.publish(robot_odr)
            r.sleep()

        rospy.loginfo("Stopping the robot...")
        self.cmd_vel.publish(Twist())
        self.robot_odr_cmd.publish(robot_odr)
        self.robot_odr_cmd.publish(Twist())

    def getkey(self):
        
        num3 = int(input("Goal\n"))
        print("what")
        if num3 == 1:  
            x = str(-0.298)
            y = str(0.4)
            z = str(0)

        elif num3 == 2:
            x = str(-0.298)
            y = str(0.000054)
            z = str(0)
        elif num3 == 3:
            x = str(-0.298)
            y = str(-0.4)
            z = str(0)
        
        elif num3 == 4:
            x = str(0.1052)
            y = str(0.4)
            z = str(0)
        
        elif num3 == 5:
            x = str(0.1052)
            y = str(0.000054)
            z = str(0)
        
        elif num3 == 6:
            x = str(0.1052)
            y = str(-0.4)
            z = str(0)

        elif num3 == 7:
            x = str(0.5)
            y = str(0.4)
            z = str(0)
        elif num3 == 8:
            x = str(0.5)
            y = str(0.000054)
            z = str(0)
        elif num3 == 9:
            x = str(0.5)
            y = str(0-.4)
            z = str(0)
        elif num3 == 0:
            x = str(-0.97)
            y = str(-0.02)
            z = str(0)
        
        x, y, z = [float(x), float(y), float(z)]
        print(x,y,z)
        return x, y, z




    def get_odom(self):
        try:
            (trans, rot) = self.tf_listener.lookupTransform(
                self.odom_frame, self.base_frame, rospy.Time(0))
            rotation = euler_from_quaternion(rot)

        except (tf.Exception, tf.ConnectivityException, tf.LookupException):
            rospy.loginfo("TF Exception")
            return

        return (Point(*trans), rotation[2])

    def shutdown(self):
        self.cmd_vel.publish(Twist())
        
        rospy.sleep(1)


if __name__ == '__main__':
    try:
        while not rospy.is_shutdown():
            print(msg)
            GotoPoint()

    except:
        rospy.loginfo("shutdown program.")
