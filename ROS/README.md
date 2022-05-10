# ROS

M1 Macbookair에서 사용

# docker를 이용한 구동 방법

> 결론 : 구동 과정이 복잡하다. 구동하기까지 매번 xquartz터미널을 설정해줘야하는데 번거롭다.
> 참고 : https://velog.io/@mond210/Docker-with-Ros2

요약하면

1. 맥 터미널에서 `open -a XQuartz`

2. XQuartz 터미널에서 `ps aux | grep Xquartz` 명령어로 구동확인

3. XQuartz에서 `xhost +` 명령어로 포워딩 권한 허용

4. 맥 터미널에서 접속하기위한 en0 포트 ip를 확인

   ```
   ip=$(ifconfig en0 | grep inet | awk '$1=="inet" {print $2}')
   print $ip
   ```

5. 컨테이너 실행 `docker start robotics`

6. 환경변수로 컨테이너 CLI접속 `docker exec -e DISPLAY=$ip:0 -it robotics /ros_entrypoint.sh bash`

7. 실행하고싶은 ros패키지 ros2 run으로 실행 ex) `ros2 run turtlesim turtlesim_node`

8. 다른 터미널창 띄우기 `cmd + D`

9. 터틀심 방향키 노드 `ros2 run turtlesim turtle_teleop_key`

# UTM을 이용한 ubuntu 20.04에서 ROS

> 결론 : 가상화머신안에서 돌아가기 때문에 초기설정만 하면 편하다.  
> 하지만 m1맥의 리소스를 전부 쓸 수는 없기 때문에 성능의 한계가 있다.

turtlesim, rqt, Gazebo가 에러없이 돌아가는 것을 확인

1. UTM 설치 https://getutm.app/
   맥스토어에 유료버전도 존재하지만 무료버전과의 차이는 유지보수정도라고 함.

2. 원하는 ubuntu버전 다운로드 `arm64` 또는 `amd64`가 있는데  
   M1 맥북은 `arm64`를 받아야한다 `amd64`는 x64아키텍쳐용이라  
   utm을 virtualize로 올리면 아키텍쳐가 맞지않아 실행되지 않는다.

3. UTM 가상머신 추가한다. virtualize와 emulate가 있는데  
   emulate는 기기와 다른 아키텍쳐를 쓸 수 있는 대신 느리다..  
   설정 시 메모리는 4GB이상을 추천한다.

4. ubuntu 설치가 완료되면 ROS를 설치하면 되는데  
   [ROS WIKI 가이드](http://wiki.ros.org/noetic/Installation/Ubuntu#noetic.2FInstallation.2FPostInstall.Tutorials)
   를 보고 그대로 따라하면된다.  
   영어가 부담스러우면 [블로그](https://pinkwink.kr/1319)에 친절히 나와있다.  
   우분투20.04에서는 foxy를 설치하라고 하지만, Noetic을 설치해도 상관없다.

5. 이후 `roscore`명령어로 마스터 실행  
   새 터미널창을 띄워서 `rosrun turtlesim turtlesim_node`로 터틀심 윈도우 창이 뜨고  
   다시 새 창을 띄워서 `rosrun turtlesim turtle_teleop_key`로 방향키로 조종이 가능하다.  
   여기서 추가로 새 창을 켜서 `rosrun rqt_graph rqt_graph`르 gui창을 볼 수 있다.  
   여러 터미널창을 띄우기 번거롭다면 [Terminator](https://shanepark.tistory.com/313)를 사용하자.

# ROS 용어

- Node

  최소 단위의 실행 가능한 프로세스로 하나의 실행 가능한 프로그램으로 생각하면 된다.  
  ROS에서는 최소한의 실행단위로 프로그램을 나누어 작업. 각 노드는 메세지 통신으로 데이터를 주고 받는다.

- Package

  하나 이상의 노드, 노드 실행을 위한 정보 등을 묶어 놓은 것.  
  또한, 패키지의 묶음을 메타패키지라 하여 따로 분리.

- Message

  메세지를 통해 노드간의 데이터를 주고받게 된다. 메시지는 integer, floating point, boolean과 같은 변수형태이다

# ROS 메시지

- 메시지는 노드 간에 데이터를 주고받을 때 사용하는 데이터의 형태
  - 토픽, 서비스, 액션은 모두 메시지를 사용
    - http://wiki.ros.org/msg
    - http://wiki.ros.org/common_msgs
- 단순 자료형
  - 정수(integer), 부동 소수점(floating point), 불(boolean)
  - http://wiki.ros.org/std_msgs
- 메시지 안에 메시지를 품고 있는 간단한 데이터 구조
  - 예) geometry_msgs/PoseStamped
  - http://docs.ros.org/api/geometry_msgs/html/msg/PoseStamped.html
- 메시지들이 나열된 배열과 같은 구조
  - 예) float32[ ] ranges
  - 예) sensor_msgs/LaserScan
  - http://docs.ros.org/api/sensor_msgs/html/msg/LaserScan.html
- 제일 핵심은 [Twist](http://docs.ros.org/en/api/geometry_msgs/html/msg/Twist.html)
  - Vector3 linear
    - float64 x
    - float64 y
    - float64 z
  - Vector3 angular
    - float64 x
    - float64 y
    - float64 z

# ROS 명령어

- ROS를 사용하는 동안 자주사용
  - [ROS Cheatsheet](https://github.com/oroca/oroca_ros_tutorials/raw/master/ROScheatsheet_indigo_catkin.pdf)

## 셸 명령어

| 명령어 | 중요도 | 세부설명                            |
| ------ | ------ | ----------------------------------- |
| roscd  | ★★★    | 지정한 ROS 패키지의 디렉터리로 이동 |
| fosls  | ★☆☆    | ROS 패키지의 파일 목록 확인         |
| rosed  | ★☆☆    | ROS 패키지의 파일 편집              |
| roscp  | ★☆☆    | ROS 패키지의 파일 복사              |

## 실행 명령어

| 명령어    | 중요도 | 세부설명                                                                    |
| --------- | ------ | --------------------------------------------------------------------------- |
| roscore   | ★★★    | master(ROS 네임 서비스), rosout(로그 기록), parameter server(파라미터 관리) |
| rosrun    | ★★★    | 노드 실행                                                                   |
| roslaunch | ★★★    | 노드를 여러 개 실행 및 실행 옵션 설정                                       |
| rosclean  | ★★☆    | ROS 로그 파일을 검사하거나 삭제                                             |

## 정보 명령어

| 명령어     | 중요도 | 세부설명                     |
| ---------- | ------ | ---------------------------- |
| rostopic   | ★★★    | ROS 토픽 정보 확인           |
| rosservice | ★★★    | ROS 서비스 정보 확인         |
| rosparam   | ★★★    | ROS 파라미터 정보 확인, 수정 |

## catkin 명령어

| 명령어            | 중요도 | 세부설명                          |
| ----------------- | ------ | --------------------------------- |
| catkin_create_pkg | ★★★    | 패키지 자동 생성                  |
| catkin_make       | ★★★    | 캐킨 빌드 시스템에 기반을 둔 빌드 |

## 패키지 명령어

| 명령어     | 중요도 | 세부설명                       |
| ---------- | ------ | ------------------------------ |
| rospack    | ★★★    | ROS 패키지와 관련된 정보 보기  |
| rosinstall | ★★☆    | ROS 추가 패키지 설치           |
| rosdep     | ★★☆    | 해당 패키지의 의존성 파일 설치 |

# ROS의 개발 도구

- 로봇 개발에 필요한 다양한 개발 도구를 제공
- 로봇 개발의 효율성 향상

## Command-Line Tools

- GUI없이 ROS에서 제공하는 명령어로만 거의 모든 ROS기능 소화

## RViz

- 강력한 3D 시각화툴 제공
- 레이저, 카메라 등의 센서 데이터를 시각화
- 로봇 외형과 계획된 동작을 표현

## RQT

- 그래픽 인터페이스 개발을 위한 Qt 기반 프레임워크 제공
- 노드와 그들 사이의 연결 정보 표시(rqt_graph)
- 인코더, 전압, 또는 시간이 지남에 따라 변화하는 숫자를 플로팅(rqt_plot)
- 데이터를 메시지 형태로 기록하고 재생(rqt_bag)

## Gazebo

- 물리엔진을탑재,로봇,센서,환경모델등을지원,3차원시뮬레이터
- ROS와의 높은 호완성

# ROS 기본 프로그래밍

메세지 통신 Topic의 퍼플리셔, 서브스크라이버 작성해보기

<img src="https://user-images.githubusercontent.com/96982072/167605806-aa5b9982-52c1-4993-a0ca-8b20f4ca1f2b.png">

`gedit` 보다는 `code .` 명령어를 사용해 vscode로 수정하면 편리함

## Topic / Publisher / Subscriber

ROS에서는 단방향 통신일때 ‘Topic’ 이라는 메시지 통신을 사용한다. 이때 송신 측을 ‘Publisher’, 수신 측을 ‘Subscriber’라고 부른다.

1. 패키지 생성

   ```bash
   $cd ~/catkin_ws/src
   $catkin_create_pkg ros_tutorials_topic message_generation std_msgs roscpp
   $cd ros_tutorials_topic
   $ls
   include → 헤더 파일 폴더
   src → 소스 코드 폴더
   CMakeLists.txt → 빌드 설정 파일
   package.xml → 패키지 설정 파일
   ```

2. 패키지 설정 파일(package.xml) 수정

   ROS의 필수 설정 파일 중 하나인 package.xml은 패키지 정보를 담은 XML 파일로서 패키지 이름, 저작자, 라이선스, 의존성 패키지 등을 기술하고 있다.

   package.xml

   ```xml
   <?xml version="1.0"?> <package format="2">
   <name>ros_tutorials_topic</name>
   <version>0.1.0</version>
   <description>ROS turtorial package to learn the topic</description>
   <license>Apache 2.0</license>
   <author email="pyo@robotis.com">Yoonseok Pyo</author>
   <maintainer email="pyo@robotis.com">Yoonseok Pyo</maintainer>
   <url type="website">http://www.robotis.com</url>
   <url type="repository">https://github.com/ROBOTIS-GIT/ros_tutorials.git</url>
   <url type="bugtracker">https://github.com/ROBOTIS-GIT/ros_tutorials/issues</url >

   <buildtool_depend>catkin</buildtool_depend>
   <depend>roscpp</depend>
   <depend>std_msgs</depend>
   <depend>message_generation</depend>
   <export></export >
   </package>
   ```

   Package Format v2

   - package.xml의 태그는 2가지의 다른 버전의 포맷이 있다.
   - 최근 제안된 package 포맷2는 기능적으로 더 많은 내용을 포함하고 있고 편리하기에
     이 강의에서는 package 포맷2를 기준으로 설명한다. 책에는 package 포맷1을 기준으로 설명하였다.
   - Package Format v2에 대한 내용은 하기 링크 참조.
   - http://docs.ros.org/jade/api/catkin/html/howto/format2/index.html

3. 빌드 설정 파일(CMakeLists.txt) 수정

   CMakeLists.txt

   ```cpp
   cmake_minimum_required(VERSION 3.0.2)
   project(ros_tutorials_topic)

   ## 캐킨 빌드를 할 때 요구되는 구성요소 패키지이다.
   ## 의존성 패키지로 message_generation, std_msgs, roscpp이며 이 패키지들이 존재하지 않으면 빌드 도중에 에러가 난다.
   find_package(catkin REQUIRED COMPONENTS
     message_generation
     roscpp
     std_msgs
   )

   ## Generate messages in the 'msg' folder
   add_message_files(
      FILES
      MsgTutorial.msg
    )

   ## 의존하는 메시지를 설정하는 옵션이다.
   ## std_msgs가 설치되어 있지 않다면 빌드 도중에 에러가 난다.
   ## Generate added messages and services with any dependencies listed here
   generate_messages(
      DEPENDENCIES
      std_msgs
    )

   ## 캐킨 패키지 옵션으로 라이브러리, 캐킨 빌드 의존성, 시스템 의존 패키지를 기술한다.
   catkin_package(
   #  INCLUDE_DIRS include
     LIBRARIES ros_tutorials_topic
     CATKIN_DEPENDS roscpp std_msgs
   #  DEPENDS system_lib
   )

   ###########
   ## Build ##
   ###########

   include_directories(
   # include
     ${catkin_INCLUDE_DIRS}
   )

   ## topic_publisher 노드에 대한 빌드 옵션이다.
   ##실행파일,타깃링크라이브러리,추가의존성등을설정한다.
   add_executable(topic_pub src/topic_pub.cpp)
   add_dependencies(topic_pub ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
   target_link_libraries(topic_pub
     ${catkin_LIBRARIES}
   )

   ## topic_subscriber 노드에 대한 빌드 옵션이다.
   add_executable(topic_sub src/topic_sub.cpp)
   add_dependencies(topic_sub ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
   target_link_libraries(topic_sub
     ${catkin_LIBRARIES}
   )
   ```

4. 메시지 파일 작성

   앞서 CMakeLists.txt 파일에 다음과 같은 옵션을 넣었다.

   ```
   add_message_files(FILES MsgTutorial.msg)
   ```

   노드에서 사용할 메시지인 MsgTutorial.msg를 빌드할 때 포함하라는 이야기

   ```bash
   $ mkdir msg               → ros_tutorials_topic 패키지에 msg라는 메시지 폴더를 신규 작성
   $ cd msg                  → 작성한 msg 폴더로 이동
   $ gedit MsgTutorial.msg   → MsgTutorial.msg 파일 신규 작성 및 내용 수정
   $ cd ..                   → ros_tutorials_topic 패키지 폴더로 이동
   ```

   - Time (메시지 형식), stamp (메시지 이름)
   - int32 (메시지 형식), data (메시지 이름)
   - 메시지 타입은 time과 int32 이외에도 bool, int8, int16, float32, string, time, duration 등의  
     메시지 기본 타입과 ROS 에서 많이 사용되는 메시지를 모아놓은 common_msgs 등도 있다.  
     여기서는 간단한 예제를 만들어 보기 위한 것으로 time과 int32를 이용하였다.  
     (부록C 및 http://wiki.ros.org/msg 를 참고 할 것!)

   msg/MsgTutorial.msg

   ```
   time stamp
   int32 data
   ```

5. 퍼블리셔 노드 작성

   ros_tutorials_topic 폴더 밑에 src 폴더를 만들고 topic_pub.cpp 파일을 만들어주자

   ```cpp
   #include "ros/ros.h"                         //ROS header
   #include "ros_tutorials_topic/MsgTutorial.h" // MsgTutorial 메시지 파일 헤더(빌드 후 자동 생성됨)
   int main(int argc, char **argv)
   {
       ros::init(argc, argv, "topic_pub");
       ros::NodeHandle nh;
       // 퍼블리셔 선언, ros_tutorials_topic 패키지의 MsgTutorial 메시지 파일을 이용한
       // 퍼블리셔 ros_tutorial_pub 를 작성한다. 토픽명은 "ros_tutorial_msg" 이며,
       // 퍼블리셔 큐(queue) 사이즈를 100개로 설정한다는 것이다
       ros::Publisher ros_tutorial_pub = nh.advertise<ros_tutorials_topic::MsgTutorial>("ros_tutorial_msg", 100);
       // 루프 주기를 설정한다. "10" 이라는 것은 10Hz를 말하는 것으로 0.1초 간격으로 반복된다
       ros::Rate loop_rate(10);
       // MsgTutorial 메시지 파일 형식으로 msg 라는 메시지를 선언
       ros_tutorials_topic::MsgTutorial msg;
       // 메시지에 사용될 변수 선언
       int count = 0;

       while (ros::ok())
       {
           msg.stamp = ros::Time::now(); // 현재 시간을 msg의 하위 stamp 메시지에 담는다
           msg.data = count;             // count라는 변수 값을 msg의 하위 data 메시지에 담는다

           ROS_INFO("send msg = %d", msg.stamp.sec);  // stamp.sec 메시지를 표시한다
           ROS_INFO("send msg = %d", msg.stamp.nsec); // stamp.nsec 메시지를 표시한다
           ROS_INFO("send msg = %d", msg.data);       // data 메시지를 표시한다

           ros_tutorial_pub.publish(msg); // 메시지를 발행한다

           loop_rate.sleep(); // 위에서 정한 루프 주기에 따라 슬립에 들어간다

           ++count; // count 변수 1씩 증가
       }

       return 0;
   }

   ```

6. 서브스크라이버 노드 작성

   마찬가지로 ros_tutorials_topic 폴더 밑에 있는 src에 topic_sub.cpp 파일을 만들어서 작성

   ```cpp
     #include "ros/ros.h"                         // ROS 기본 헤더파일
     #include "ros_tutorials_topic/MsgTutorial.h" // MsgTutorial 메시지 파일 헤더 (빌드 후 자동 생성됨)
     // 메시지 콜백 함수로써, 밑에서 설정한 ros_tutorial_msg라는 이름의 토픽
     // 메시지를 수신하였을 때 동작하는 함수이다
     // 입력 메시지로는 ros_tutorials_topic 패키지의 MsgTutorial 메시지를 받도록 되어있다
     void msgCallback(const ros_tutorials_topic::MsgTutorial::ConstPtr &msg)
     {
         ROS_INFO("recieve msg = %d", msg->stamp.sec);
         ROS_INFO("recieve msg = %d", msg->stamp.nsec);
         ROS_INFO("recieve msg = %d", msg->data);
     }
     int main(int argc, char **argv) // 노드 메인 함수
     {
         ros::init(argc, argv, "topic_sub"); // 노드명 초기화
         ros::NodeHandle nh; // ROS 시스템과 통신을 위한 노드 핸들 선언
         // 서브스크라이버 선언, ros_tutorials_topic 패키지의 MsgTutorial 메시지 파일을 이용한
         // 서브스크라이버 ros_tutorial_sub 를 작성한다. 토픽명은 "ros_tutorial_msg" 이며,
         // 서브스크라이버 큐(queue) 사이즈를 100개로 설정한다는 것이다
         ros::Subscriber ros_tutorial_sub = nh.subscribe("ros_tutorial_msg", 100, msgCallback);
         // 콜백함수 호출을 위한 함수로써, 메시지가 수신되기를 대기,
         // 수신되었을 경우 콜백함수를 실행한다
         ros::spin();
         return 0;
     }
   ```

7. ROS 노드 빌드

- 다음 명령어로 ros_tutorials_topic 패키지의 메시지 파일, 퍼블리셔 노드, 서브스크라이버 노드를 빌드하자.

  ```bash
  $ cd ~/catkin_ws → catkin 폴더로 이동
  $ catkin_make → catkin 빌드 실행
  ```

- [참고] 파일시스템
  - ros_tutorials_topic 패키지의 **소스 코드 파일**: ~/catkin_ws/src/ros_tutorials_topic/src
  - ros_tutorials_topic 패키지의 **메시지 파일**: ~/catkin_ws/src/ros_tutorials_topic/msg
  - 빌드된 결과물은 /catkin_ws의 /build와 /devel 폴더에 각각 생성
    - /build 폴더에는 캐킨 빌드에서 사용된 설정 내용이 저장
    - /devel/lib/ros_tutorials_topic 폴더에는 실행 파일이 저장
    - /devel/include/ros_tutorials_topic 폴더에는 메시지 파일로부터 자동 생성된 메시지 헤더 파일이 저장

8. 퍼블리셔/서브스크라이버 실행

   1. ROSCORE 실행
      ```
      roscore
      ```
   2. 다른 터미널창에서 퍼블리셔 실행
      ```
      rosrun ros_tutorials_topic topic_publisher
      ```
   3. 다른 터미널창에서 서브스크라이버 실행
      ```
      rosrun ros_tutorials_topic topic_subscriber
      ```
   4. 다른 터미널창에서 실행된 노드들의 상태 확인
      ```
      rqt_graph
      ```

## 정리

- 토픽에서 사용되는 퍼블리셔와 서브스크라이버 노드를 작성하고 이를 실행해서 노드 간 토픽 통신 방법을 알아보았다.  
  관련 소스는 하기의 github 주소에서 확인할 수 있다.

- https://github.com/ROBOTIS-GIT/ros_tutorials/tree/master/ros_tutorials_topic

- 바로 적용해보고 싶다면 catkin_ws/src 폴더에서 다음 명령어로 소스 코드를 클론한 후
  빌드를 실행하면 된다.  
  그 뒤 topic_publisher 및 topic_subscriber 노드를 실행하면 된다.

- 이 과정이 핵심이고 나머지 서비스나 파라미터도 같은 과정을 거친다.

```
$ cd ~/catkin_ws/src
$ git clone https://github.com/ROBOTIS-GIT/ros_tutorials.git $ cd ~/catkin_ws
$ catkin_make
$ rosrun ros_tutorials_topic topic_publisher
$ rosrun ros_tutorials_topic topic_subscriber
```
