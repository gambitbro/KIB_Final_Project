# [SSR Smart Farm]

## 홍민이 : **"우린 미래를 경작한다."**

- 고려대학교 HRD IoT 개발자과정 파이널 프로젝트입니다.
- [slide(보기 전용)](https://www.canva.com/design/DAGIo3UenlM/_KW2WdmP_S_bjc8OKEcd7A/view?utm_content=DAGIo3UenlM&utm_campaign=designshare&utm_medium=link&utm_source=editor)
- [slide(계획 발표)](https://docs.google.com/presentation/d/1gJG-Qnbx-0N8gWtX3mH1qRT7-PApX_3W-6SjmxGCNGo/edit#slide=id.p)
- [여기를 클릭하세요](https://google.com)
-------------------------------
<br>

## 👋 **Introduction**
<br/>
<table>
    <tr>
        <th>프로젝트 명 </th>
        <th>가정용 스마트 팜</th>
        <th>개발기간</th>
        <th>24.06.20. - 24.07.09.</th>
    </tr>
    <tr>
        <th>프로젝트 성격</th>
        <th>Final project</th>
        <th>개발인원</th>
        <th>팀 / 3명<br>
          (<a href="https://github.com/hongsola92">홍솔아</a>｜
          <a href="https://github.com/gambitbro">민병근</a> |
          <a href="https://github.com/MrTwee">이진우</a>)
      </th>
    </tr>
      <tr>
        <th>프로젝트 개요</th>
        <th>가정용 스마트팜 만들기 </th>
        <th>개발환경&nbsp;</th>
        <th>Arduino IDE,<br> VScode,<br> Linux </th>
    </tr>
    <tr>
        <th>사용 센서</th>
        <th colspan="3">온 습도 센서, 토양습도 센서, 물 수위 센서 </th>
    </tr>
    <tr>
        <th>사용 하드웨어</th>
        <th colspan="3">Arduino, RaspberryPI, Jetson Nano
        </th>
    </tr>    
    <tr>
        <th>개발언어</th>
        <th colspan="3">C/C++, Python</th>
    </tr>
    <tr>
        <th>사용기술</th>
        <th colspan="3">
            <br> (데이터베이스) SQLite or Google CLoud Database
            <br> (Web) Flask
            <br> (온습도 측정) Adafruit_DHT
            <br> (GPIO 제어) RPi.GPIO
            <br> (물 수위 센서, 토양 수분 센서) spidev
            <br>
            <br>
        </th>
    </tr>
</table>
<br>
<br>
<br>

## 📑 **Role & Member**
<br/>
<table>
    <tr>
        <th width="16%">업무 / 구성원</th>
        <th width="17%">홍솔아</th>
        <th width="17%">민병근</th>        
        <th width="14%">이진우</th>        
    </tr>
    <tr>
        <th>프로젝트 기획</th>
        <th colspan="3"> <center>개요작성, 회의, 의견제안 </center> </th>
    </tr>
    <tr>
        <th>요구분석</th>
        <th colspan="3"> <center> 문서작성, 회의, 의견제안 </center> </th>
    </tr>
        <th>소스(코딩)</th>
        <th>
            <br>RaspberryPI에서 Cloud DB 구현, 데이터 저장 및 관리
            <br>PPT, 계획서, 기술서 작성
            <br>Data Wrangling
            <br>스마트 팜 구조 조립 및 완성
        <th>
            <br>Arduino senser 구현 및 데이터 수집
            <br>Arduino와 RaspberryPI 블루투스 연결(or 시리얼 연결) 및 데이터 전송
            <br>Data Wrangling
            <br>PPT, 계획서, 기술서 작성
            <br>스마트 팜 구조 조립 및 완성
        <th>
            <br>식물 관찰을 위한 카메라 OpenCV 코드 작성
            <br>PPT, 계획서, 기술서 작성
            <br>Data Wrangling
            <br>스마트 팜 구조 조립 및 완성
        </th>
    </tr>
</table>

----------------------
# 프로젝트 일정 🗓️
## 주요 일정 
    ● **개발 시작**: 2024-06-20 🕒
    ● **중간 점검**: 2024-07-01 📋
    ● **최종 완료**: 2024-07-09 ⏰
--------------------
# 일별 일정 🗓️
## 2024-06-20(목)
    ● 아이디어 회의 🤔
    ● 프로젝트 일정 작성
    ● 구글 슬라이드 제작
    ● project github 설정
    ● 역할 분담
    ● 재료 선정

## 2024-06-21(금)
    ● 아이디어 회의 🤔
    ● 프로젝트 일정 작성
    ● 구글 슬라이드 제작
    ● 역할 분담
    ● 재료 선정

## 2024-06-24(월)
    ● 아이디어 회의 🤔
        - 작물 변경 논의 ==> '상추' 하기로 결정.
        - 재배 방식 변경 논의 ==> '수경'-> '양액' 으로 변경.
        - 추가 기능 고려: chatGPT 기능 삽입 고려.(재배 하면서 궁금한 것이 있으면 바로 물어볼 수 있게)
        - db 변경 논의 ==> 클라우드DB + MySQL 같이 하기. 
    ● 품의서 작성 및 제출
    ● 재료 구매
    ● 가상환경에서 MySQL 데이터베이스 테스트

## 2024-06-25(화)
    ● 회의
    ● google cloud_mysql table 생성.
    ● 추가 재료 확인 및 아이디어 나누기
    ● 아두이노 메가 <--> 라즈베리파이4 or 5 코드 임시작성
    ● 스마트팜 IoT 전체적 구조 파악 및 설계 방향 논의

## 2024-06-26(수)
    ● Arduino UNO와 RaspberryPi4 => 블루투스 연결하여 신호 왔다갔다 하는지 테스트.
    ● UNO보드의 모듈은 cc2541(data sheet 참고), RaspberryPi4에서 connect error, 연결되었다고 나오는 즉시 에러뜨며 끊김 -> 블루투스 연결 테스트 실패
        해결방안 -> 아두이노 메가 보드의 와이파이 모듈 사용이 어떨까(Cloud SQL과 직접 연동)
    ● 스마트팜 외형 1차 제작
- ![1차 제작](img/KakaoTalk_20240626_173626872.jpg)

## 2024-06-27(목)
    ● 회의
    ● 
    ● 
    ● 
----------------------------
- 📚 스마트팜 외부(사진)

- 📚 스마트팜 내부(사진)
