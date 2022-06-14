# include <ros/ros.h>
  # include <tf/transform_broadcaster.h>
  # include <nav_msgs/Odometry.h>

 int main(int argc, char ** argv){
    6   ros: : init(argc, argv, "odometry_publisher")
    7
    8   ros: : NodeHandle n
    9   ros: : Publisher odom_pub = n.advertise < nav_msgs: : Odometry > ("odom", 50)
    10   tf: : TransformBroadcaster odom_broadcaster
    11
    12   double x = 0.0
    13   double y = 0.0
    14   double th = 0.0
    15
    16   double vx = 0.1
    17   double vy = -0.1
    18   double vth = 0.1
    19
    20   ros:: Time current_time, last_time
    21   current_time = ros:: Time: : now()
    22   last_time = ros:: Time: : now()
    23
    24   ros:: Rate r(1.0)
    25 while(n.ok()){
        26
        27     ros:: spinOnce()
        // check for incoming messages
        28     current_time = ros: : Time: : now()
        29
        30 // compute odometry in a typical way given the velocities of the robot
        31     double dt = (current_time - last_time).toSec()
        32     double delta_x = (vx * cos(th) - vy * sin(th)) * dt
        33     double delta_y = (vx * sin(th) + vy * cos(th)) * dt
        34     double delta_th = vth * dt
        35
        36     x += delta_x
        37     y += delta_y
        38     th += delta_th
        39
        40 // since all odometry is 6DOF we'll need a quaternion created from yaw
        41     geometry_msgs: : Quaternion odom_quat = tf: : createQuaternionMsgFromYaw(th)
        42
        43 // first, we'll publish the transform over tf
        44     geometry_msgs:: TransformStamped odom_trans
        45     odom_trans.header.stamp = current_time
        46     odom_trans.header.frame_id = "odom"
        47     odom_trans.child_frame_id = "base_link"
        48
        49     odom_trans.transform.translation.x = x
        50     odom_trans.transform.translation.y = y
        51     odom_trans.transform.translation.z = 0.0
        52     odom_trans.transform.rotation = odom_quat
        53
        54 // send the transform
        55     odom_broadcaster.sendTransform(odom_trans)
        56
        57 // next, we'll publish the odometry message over ROS
        58     nav_msgs:: Odometry odom
        59     odom.header.stamp = current_time
        60     odom.header.frame_id = "odom"
        61
        62 // set the position
        63     odom.pose.pose.position.x = x
        64     odom.pose.pose.position.y = y
        65     odom.pose.pose.position.z = 0.0
        66     odom.pose.pose.orientation = odom_quat
        67
        68 // set the velocity
        69     odom.child_frame_id = "base_link"
        70     odom.twist.twist.linear.x = vx
        71     odom.twist.twist.linear.y = vy
        72     odom.twist.twist.angular.z = vth
        73
        74 // publish the message
        75     odom_pub.publish(odom)
        76
        77     last_time = current_time
        78     r.sleep()
        79}
    80}
