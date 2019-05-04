# 나빌 입력기 2.1

- 2019년 4월 28일

나빌 입력기 2.1을 릴리즈합니다. 사용하면서 발견한 버그를 몇 개 수정했고 인스톨러와 설정툴을 새로 만들어서 패키지에 추가했습니다. 언인스톨러도 만들었으나 릴리즈 패키지에는 추가하지 않고 별도 배포합니다. 나빌 입력기는 64비트 어플리케이션에서만 쓸 수 있었으나 32비트 어플리케이션에서도 쓸 수 있도록 수정했습니다.

## 버그 수정

* Preedit 리스트 처리 버그 수정 [https://github.com/navilera/NavilIME/issues/1] - fehead 님 감사합니다.
* Wordpad에서 정상 동작하게 수정 [https://github.com/navilera/NavilIME/issues/2]
* MS Word에서 정상 동작하게 수정 [https://github.com/navilera/NavilIME/issues/6]
* 한영 전환할 때 영어 문자 입력 위치 버그 수정 [https://github.com/navilera/NavilIME/issues/10]

## 기능 추가

* 32비트 어플리케이션 지원 [https://github.com/navilera/NavilIME/issues/3] [https://github.com/navilera/NavilIME/issues/4]- fehead 님 감사합니다.
* 인스톨러 추가 [https://github.com/navilera/NavilIME/issues/5]
* 언인스톨러 추가 [https://github.com/navilera/NavilIME/issues/7]
* 설정툴 추가 [https://github.com/navilera/NavilIME/issues/8]
* 3-18Na 자판 변경 사항 반영 [https://github.com/navilera/NavilIME/issues/9]

## 인스톨러

아래 링크에서 나빌입력기 2.1의 인스톨러를 내려받을 수 있습니다.

https://github.com/navilera/NavilIME/blob/master/Release/Installer/NavilIme-2.1.0.exe

인스톨러를 내려받고 실행하면 자동으로 나빌입력기 2.1을 설치합니다.

![나빌입력기 인스톨러](https://github.com/navilera/NavilIME/blob/master/Doc/ReleaseNotes_2.1/installer_01.JPG)

Next만 누르면 됩니다.

## 설정툴

나빌 입력기를 설치하고 나면 나빌 입력기 설정툴이 설치됩니다. 현재 나빌 입력기에 설정할만한 내용은 자판 변경 뿐입니다. 그래서 설정 툴을 실행하면 나빌 입력기로 입력할 수 있는 자판 목록이 나옵니다.

![자판 목록](https://github.com/navilera/NavilIME/blob/master/Doc/ReleaseNotes_2.1/config_tool.JPG)

자판 목록에서 자판을 선택하고 확인을 누르면 자판이 변경됩니다. 설정을 확실히 하려면 자판을 변경하고 시스템을 재부팅하는 것을 추천합니다.

## 언인스톨러

윈도우의 입력기는 DLL 형태로 어플리케이션이 임포트하는 형태로 동작합니다. 그래서 언인스톨할 때 입력기를 사용하고 있는 어플리케이션 프로세스를 종료해야만 입력기 DLL을 완전히 삭제할 수 있습니다. 해당 작업을 자동으로 수행하는 언인스톨러를 만들어서 배포합니다.

시스템 레지스터에서 입력기 정보를 제거해야 하기 때문에 언인스톨러는 관리자 권한으로 실행해야 합니다. 

![관리자권한필요](https://github.com/navilera/NavilIME/blob/master/Doc/ReleaseNotes_2.1/Re_run_as_admin.JPG)

만약 나빌 입력기 DLL을 사용하고 있는 프로세스가 동작 중이라면 언인스톨러가 프로세스 목록을 보여줍니다. 언인스톨러가 알려주는 프로세스를 종료해 주세요.

![프로세스 종료](https://github.com/navilera/NavilIME/blob/master/Doc/ReleaseNotes_2.1/need_to_close_app.JPG)

나빌 입력기 DLL을 사용하는 DLL이 없으면 입력기 제거를 시작합니다. 언인스톨러에서 제거 완료를 알려주면 언인스톨러를 종료해 주세요.

![언인스톨 완료](https://github.com/navilera/NavilIME/blob/master/Doc/ReleaseNotes_2.1/uninstall_done.JPG)


## 3-18Na 레이아웃 변경

3-18Na 자판 프로젝트와 나빌 입력기 프로젝트는 별개입니다만, 애초에 제가 나빌 입력기를 만든 가장 큰 이유가 3-18Na 자판을 윈도우에서 쓰기 위함이기에 3-18Na 자판에 대한 변경도 나빌 입력기의 주요 변경 사항이라고 생각하여 여기에 적습니다.

![3-18Na 자판레이아웃](https://raw.githubusercontent.com/navilera/318Na_HangulKeyboard/master/3-18Na_layout.png)

3-18Na 자판으로 십만자 이상 글을 써 보니 의외로 겹받침을 한 번에 입력하는 것이 편했습니다. 비록 시프트키를 누르더라도 한 번에 겹받침을 입력하는 것이 자판을 두 번 눌러서 입력하는 것보다 편했습니다. 그래서 겹받침 ㄶ, ㅀ, ㄼ, ㄻ 네 개를 각각 시프트+H, 시프트+I, 시프트+B, 시프트+M에 배정했습니다.

자세한 내용은 아래 링크를 클릭해서 3-18Na 자판 프로젝트 페이지를 방문해 주세요.

https://github.com/navilera/318Na_HangulKeyboard

## 다운로드

https://github.com/navilera/NavilIME/tree/master/Release/Installer

위 링크를 클릭해서 최신 버전 인스톨러를 내려받아 주세요. 현재 최신 버전은 2.1입니다.