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

9. 터틀심 방향키 조종 `ros2 run turtlesim turtle_teleop_key`

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
   영어가 부담스러우면 https://pinkwink.kr/1319 블로그에 친절히 나와있다.  
   우분투20.04에서는 foxy를 설치하라고 하지만, Noetic을 설치해도 상관없다.

5. 이후 `roscore`명령어로 마스터 실행  
   새 터미널창을 띄워서 `rosrun turtlesim turtlesim_node`로 터틀심 윈도우 창이 뜨고  
   다시 새 창을 띄워서 `rosrun turtlesim turtle_teleop_key`로 방향키로 조종이 가능하다.  
   여기서 추가로 새 창을 켜서 `rosrun rqt_graph rqt_graph`르 gui창을 볼 수 있다.  
   여러 터미널창을 띄우기 번거롭다면 [Terminator](https://shanepark.tistory.com/313)를 사용하자.

# ROS 용어

## Node

최소 단위의 실행 가능한 프로세스로 하나의 실행 가능한 프로그램으로 생각하면 된다.  
ROS에서는 최소한의 실행단위로 프로그램을 나누어 작업. 각 노드는 메세지 통신으로 데이터를 주고 받는다.

## Package

하나 이상의 노드, 노드 실행을 위한 정보 등을 묶어 놓은 것.  
또한, 패키지의 묶음을 메타패키지라 하여 따로 분리.

## Message

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

- GUI없이ROS에서제공되는명령어로만로봇억􏰀스및거의모든ROS기능소화

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
