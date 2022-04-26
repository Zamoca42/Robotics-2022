# ROS

M1 Macbookair에서 사용

## docker를 이용한 구동 방법

>결론 : 구동 과정이 복잡하다  
>참고 : https://velog.io/@mond210/Docker-with-Ros2

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

## UTM을 이용한 ubuntu 20.04에서 ROS

>결론 : 가상화머신안에서 돌아가기 때문에 초기설정만 하면 편하다  

현재 turtlesim, rqt, Gazebo가 오류없이 돌아가는 것을 확인 UTM이 나를 살렸다.  

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
