# 🎓 College Coursework Archive (전공 프로젝트 모음)

컴퓨터공학 전공 수업에서 진행한 프로그래밍 과제 및 기말 프로젝트 아카이브 저장소입니다.  
C/C++, Java, Python(OpenCV)을 활용한 핵심 프로젝트 소스 코드 및 결과 보고서를 포함하고 있습니다.

---

## 📁 Projects Overview

### 1. 🚗 [Automotive Camera Vision](./automotive-camera-vision)
* **주제:** 실시간 얼굴 및 손 인식 기술을 활용한 가상 마스크 필터
* **기술 스택:** `Python`, `OpenCV`, `MediaPipe`, `NumPy`
* **주요 기능:**
  * MediaPipe FaceMesh를 활용한 얼굴 랜드마크 추출 및 가상 마스크(선글라스, 가면, 사륜안 등) 오버레이
  * 손 인식(Hands) 기반 제스처 인터랙션 (나선환 회전 이펙트)
  * OpenCV GUI 기반 필터 토글 인터페이스 구현
* **문서:** [결과 보고서 PDF](./automotive-camera-vision)

---

### 2. 🎮 [Hangman Quiz Game](./c-quiz-game)
* **주제:** Windows API 사운드 및 아스키 아트를 적용한 콘솔 행맨 퀴즈 게임
* **기술 스택:** `C++`, `Visual Studio`, `Windows API`
* **주요 기능:**
  * 단계별 행맨 아스키 아트 렌더링 및 입력 판정 로직
  * `winmm.lib` 및 `PlaySound` 함수를 활용한 실시간 배경음악(BGM) 재생
  * 콘솔 버퍼 제어를 통한 게임 화면 갱신
* **문서:** [기말 프로젝트 보고서 PDF](./c-quiz-game)

---

### 3. 🧠 [Java Nonsense Quiz](./java-nonsense-quiz)
* **주제:** GUI 기반 인터랙티브 넌센스 퀴즈 프로그램
* **기술 스택:** `Java`, `Swing/AWT`, `Clip/AudioSystem`
* **주요 기능:**
  * 자바 GUI 컴포넌트를 활용한 퀴즈 인터페이스
  * 정답/오답 판정에 따른 사운드 이펙트(Ding/Bell) 재생 기능
* **문서:** [실습 기말 프로젝트 보고서 PDF](./java-nonsense-quiz)
