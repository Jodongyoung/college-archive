import cv2
import numpy as np
import mediapipe as mp
import os


class CapstonePremium:
    def __init__(self):
        self.mp_face_mesh = mp.solutions.face_mesh
        self.mp_hands = mp.solutions.hands

        # refine_landmarks=True 가 있어야 눈동자(홍채) 추적이 가능함!
        self.face_mesh = self.mp_face_mesh.FaceMesh(max_num_faces=1, refine_landmarks=True)
        self.hands = self.mp_hands.Hands(max_num_hands=2)

        # 🌟 필터 6개 (eyes는 선글라스, sharingan은 사륜안)
        self.filters = {'face': False, 'eyes': False, 'sharingan': False, 'nose': False, 'mouth': False, 'hand': False}

        self.img_face = cv2.imread('face.png', cv2.IMREAD_UNCHANGED) if os.path.exists('face.png') else None
        self.img_glasses = cv2.imread('sunglasses.png', cv2.IMREAD_UNCHANGED) if os.path.exists('sunglasses.png') else None
        self.img_sharingan = cv2.imread('eye.png', cv2.IMREAD_UNCHANGED) if os.path.exists('eye.png') else None
        self.img_nose = cv2.imread('pig.png', cv2.IMREAD_UNCHANGED) if os.path.exists('pig.png') else None
        self.img_mouth = cv2.imread('joker.png', cv2.IMREAD_UNCHANGED) if os.path.exists('joker.png') else None
        self.img_rasengan = cv2.imread('rasengan.png', cv2.IMREAD_UNCHANGED) if os.path.exists('rasengan.png') else None

        self.rasengan_angle = 0

    def overlay_png(self, bg_img, img_to_overlay_t, x, y):
        if img_to_overlay_t is None or len(img_to_overlay_t.shape) < 3 or img_to_overlay_t.shape[2] < 4:
            return bg_img

        bg_img = bg_img.copy()
        h, w, _ = img_to_overlay_t.shape
        bg_h, bg_w, _ = bg_img.shape

        y1, y2 = max(0, y), min(bg_h, y + h)
        x1, x2 = max(0, x), min(bg_w, x + w)
        y1_o, y2_o = max(0, -y), min(h, bg_h - y)
        x1_o, x2_o = max(0, -x), min(w, bg_w - x)

        if y1 >= y2 or x1 >= x2 or y1_o >= y2_o or x1_o >= x2_o:
            return bg_img

        alpha = img_to_overlay_t[y1_o:y2_o, x1_o:x2_o, 3] / 255.0
        alpha_inv = 1.0 - alpha

        for c in range(3):
            bg_img[y1:y2, x1:x2, c] = (alpha * img_to_overlay_t[y1_o:y2_o, x1_o:x2_o, c] +
                                       alpha_inv * bg_img[y1:y2, x1:x2, c])
        return bg_img

    def mouse_callback(self, event, x, y, flags, param):
        if event == cv2.EVENT_LBUTTONDOWN:
            if x >= 640:
                # 480픽셀을 6등분 (간격: 80픽셀)
                if 0 <= y < 80:
                    self.filters['face'] = not self.filters['face']
                elif 80 <= y < 160:
                    self.filters['eyes'] = not self.filters['eyes']
                elif 160 <= y < 240:
                    self.filters['sharingan'] = not self.filters['sharingan']
                elif 240 <= y < 320:
                    self.filters['nose'] = not self.filters['nose']
                elif 320 <= y < 400:
                    self.filters['mouth'] = not self.filters['mouth']
                elif 400 <= y < 480:
                    self.filters['hand'] = not self.filters['hand']

    def draw_ui(self, display_frame):
        bg_face = (70, 70, 70) if self.filters['face'] else (20, 20, 20)
        bg_eye = (70, 70, 70) if self.filters['eyes'] else (20, 20, 20)
        bg_sharingan = (70, 70, 70) if self.filters['sharingan'] else (20, 20, 20)
        bg_nose = (70, 70, 70) if self.filters['nose'] else (20, 20, 20)
        bg_mouth = (70, 70, 70) if self.filters['mouth'] else (20, 20, 20)
        bg_hand = (70, 70, 70) if self.filters['hand'] else (20, 20, 20)

        # 6개 버튼 배경 그리기
        cv2.rectangle(display_frame, (640, 0), (800, 80), bg_face, -1)
        cv2.rectangle(display_frame, (640, 80), (800, 160), bg_eye, -1)
        cv2.rectangle(display_frame, (640, 160), (800, 240), bg_sharingan, -1)
        cv2.rectangle(display_frame, (640, 240), (800, 320), bg_nose, -1)
        cv2.rectangle(display_frame, (640, 320), (800, 400), bg_mouth, -1)
        cv2.rectangle(display_frame, (640, 400), (800, 480), bg_hand, -1)

        c_face = (0, 255, 255) if self.filters['face'] else (255, 255, 255)
        c_eye = (0, 255, 255) if self.filters['eyes'] else (255, 255, 255)
        c_sharingan = (0, 255, 255) if self.filters['sharingan'] else (255, 255, 255)
        c_nose = (0, 255, 255) if self.filters['nose'] else (255, 255, 255)
        c_mouth = (0, 255, 255) if self.filters['mouth'] else (255, 255, 255)
        c_hand = (0, 255, 255) if self.filters['hand'] else (255, 255, 255)

        # 구분선 그리기
        cv2.line(display_frame, (640, 80), (800, 80), (100, 100, 100), 2)
        cv2.line(display_frame, (640, 160), (800, 160), (100, 100, 100), 2)
        cv2.line(display_frame, (640, 240), (800, 240), (100, 100, 100), 2)
        cv2.line(display_frame, (640, 320), (800, 320), (100, 100, 100), 2)
        cv2.line(display_frame, (640, 400), (800, 400), (100, 100, 100), 2)
        cv2.line(display_frame, (640, 0), (640, 480), (100, 100, 100), 2)

        # 텍스트
        cv2.putText(display_frame, "Face", (675, 50), cv2.FONT_HERSHEY_DUPLEX, 0.7, c_face, 2)
        cv2.putText(display_frame, "Glasses", (655, 130), cv2.FONT_HERSHEY_DUPLEX, 0.7, c_eye, 2)
        cv2.putText(display_frame, "Eye", (685, 210), cv2.FONT_HERSHEY_DUPLEX, 0.7, c_sharingan, 2)
        cv2.putText(display_frame, "Nose", (675, 290), cv2.FONT_HERSHEY_DUPLEX, 0.7, c_nose, 2)
        cv2.putText(display_frame, "Mouth", (665, 370), cv2.FONT_HERSHEY_DUPLEX, 0.7, c_mouth, 2)
        cv2.putText(display_frame, "Hand", (670, 450), cv2.FONT_HERSHEY_DUPLEX, 0.7, c_hand, 2)

    def run(self):
        cap = cv2.VideoCapture(0)
        cv2.namedWindow('Capstone Premium')
        cv2.setMouseCallback('Capstone Premium', self.mouse_callback)

        while cap.isOpened():
            ret, frame = cap.read()
            if not ret:
                break

            frame = cv2.flip(frame, 1)
            frame = cv2.resize(frame, (640, 480))
            h, w, _ = frame.shape
            rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

            try:
                if self.filters['face'] or self.filters['eyes'] or self.filters['sharingan'] or self.filters['nose'] or self.filters['mouth']:
                    face_results = self.face_mesh.process(rgb_frame)
                    if face_results.multi_face_landmarks:
                        for face_landmarks in face_results.multi_face_landmarks:

                            # [얼굴 가면]
                            if self.filters['face']:
                                left_x = int(face_landmarks.landmark[234].x * w)
                                right_x = int(face_landmarks.landmark[454].x * w)
                                face_width = right_x - left_x
                                center_x = int(face_landmarks.landmark[168].x * w)
                                center_y = int(face_landmarks.landmark[168].y * h)

                                if self.img_face is not None and len(self.img_face.shape) == 3 and self.img_face.shape[2] == 4:
                                    img_h, img_w = self.img_face.shape[:2]
                                    img_ratio = img_h / img_w
                                    scale_factor = 4.5
                                    mask_w = int(face_width * scale_factor)
                                    mask_h = int(mask_w * img_ratio)
                                    resized_mask = cv2.resize(self.img_face, (mask_w, mask_h))
                                    offset_y = int(mask_h * 0.45)
                                    frame = self.overlay_png(frame, resized_mask, center_x - mask_w // 2, center_y - offset_y)

                            # [선글라스]
                            if self.filters['eyes']:
                                lx, ly = int(face_landmarks.landmark[33].x * w), int(face_landmarks.landmark[33].y * h)
                                rx, ry = int(face_landmarks.landmark[263].x * w), int(face_landmarks.landmark[263].y * h)
                                eye_width = int(np.sqrt((rx - lx) ** 2 + (ry - ly) ** 2))

                                if self.img_glasses is not None and len(self.img_glasses.shape) == 3 and self.img_glasses.shape[2] == 4:
                                    new_glass_width = int(eye_width * 2.2)
                                    resized_img = cv2.resize(self.img_glasses, (new_glass_width, int(new_glass_width * self.img_glasses.shape[0] / self.img_glasses.shape[1])))
                                    offset_x = lx - int((new_glass_width - eye_width) / 2)
                                    frame = self.overlay_png(frame, resized_img, offset_x, ly - resized_img.shape[0] // 2)

                            # [사륜안]
                            if self.filters['sharingan']:
                                lx_in, ly_in = int(face_landmarks.landmark[133].x * w), int(face_landmarks.landmark[133].y * h)
                                lx_out, ly_out = int(face_landmarks.landmark[33].x * w), int(face_landmarks.landmark[33].y * h)
                                l_iris_x, l_iris_y = int(face_landmarks.landmark[468].x * w), int(face_landmarks.landmark[468].y * h)
                                l_eye_width = int(np.sqrt((lx_in - lx_out) ** 2 + (ly_in - ly_out) ** 2))

                                rx_in, ry_in = int(face_landmarks.landmark[362].x * w), int(face_landmarks.landmark[362].y * h)
                                rx_out, ry_out = int(face_landmarks.landmark[263].x * w), int(face_landmarks.landmark[263].y * h)
                                r_iris_x, r_iris_y = int(face_landmarks.landmark[473].x * w), int(face_landmarks.landmark[473].y * h)
                                r_eye_width = int(np.sqrt((rx_out - rx_in) ** 2 + (ry_out - ry_in) ** 2))

                                if self.img_sharingan is not None and len(self.img_sharingan.shape) == 3 and self.img_sharingan.shape[2] == 4:
                                    scale = 0.4
                                    l_size = max(1, int(l_eye_width * scale))
                                    r_size = max(1, int(r_eye_width * scale))

                                    resized_l_eye = cv2.resize(self.img_sharingan, (l_size, l_size))
                                    resized_r_eye = cv2.resize(self.img_sharingan, (r_size, r_size))

                                    frame = self.overlay_png(frame, resized_l_eye, l_iris_x - l_size // 2, l_iris_y - l_size // 2)
                                    frame = self.overlay_png(frame, resized_r_eye, r_iris_x - r_size // 2, r_iris_y - r_size // 2)

                            # [돼지코]
                            if self.filters['nose']:
                                nx, ny = int(face_landmarks.landmark[1].x * w), int(face_landmarks.landmark[1].y * h)
                                if self.img_nose is not None and len(self.img_nose.shape) == 3 and self.img_nose.shape[2] == 4:
                                    resized_img = cv2.resize(self.img_nose, (80, 80))
                                    frame = self.overlay_png(frame, resized_img, nx - 40, ny - 40)

                            # [조커 입]
                            if self.filters['mouth']:
                                lx, ly = int(face_landmarks.landmark[61].x * w), int(face_landmarks.landmark[61].y * h)
                                rx, ry = int(face_landmarks.landmark[291].x * w), int(face_landmarks.landmark[291].y * h)
                                mouth_width = int(np.sqrt((rx - lx) ** 2 + (ry - ly) ** 2))
                                cx, cy = (lx + rx) // 2, (ly + ry) // 2

                                if self.img_mouth is not None and len(self.img_mouth.shape) == 3 and self.img_mouth.shape[2] == 4:
                                    new_mouth_width = int(mouth_width * 3.0)
                                    new_mouth_height = int(new_mouth_width * (self.img_mouth.shape[0] / self.img_mouth.shape[1]))
                                    resized_joker = cv2.resize(self.img_mouth, (new_mouth_width, new_mouth_height))
                                    frame = self.overlay_png(frame, resized_joker, cx - new_mouth_width // 2, cy - new_mouth_height // 2)

                # [나선환]
                if self.filters['hand']:
                    hand_results = self.hands.process(rgb_frame)
                    if hand_results.multi_hand_landmarks:
                        for hand_landmarks in hand_results.multi_hand_landmarks:
                            palm = hand_landmarks.landmark[9]
                            px, py = int(palm.x * w), int(palm.y * h)

                            if self.img_rasengan is not None and len(self.img_rasengan.shape) == 3 and self.img_rasengan.shape[2] == 4:
                                resized_img = cv2.resize(self.img_rasengan, (220, 220))
                                self.rasengan_angle = (self.rasengan_angle + 15) % 360
                                center = (110, 110)
                                matrix = cv2.getRotationMatrix2D(center, self.rasengan_angle, 1.0)
                                rotated_img = cv2.warpAffine(
                                    resized_img, matrix, (220, 220),
                                    flags=cv2.INTER_LINEAR,
                                    borderMode=cv2.BORDER_CONSTANT,
                                    borderValue=(0, 0, 0, 0)
                                )
                                frame = self.overlay_png(frame, rotated_img, px - 110, py - 110)
            except Exception:
                pass

            display_frame = np.zeros((480, 800, 3), dtype=np.uint8)
            display_frame[:, :640] = frame

            self.draw_ui(display_frame)
            cv2.imshow('Capstone Premium', display_frame)

            # 🌟 [수정된 핵심 로직!]
            # 1) 먼저 waitKey로 OS 이벤트(키 입력 및 창 닫기 신호)를 처리한다.
            key = cv2.waitKey(1) & 0xFF
            # 'q', 'p', 'P', 또는 ESC(27) 누르면 루프 종료
            if key in [ord('q'), ord('p'), ord('P'), 27]:
                break

            # 2) waitKey 실행 직후 창 상태를 확인! 창 닫기(X)를 눌렀으면 1 미만이 되므로 바로 루프 탈출
            try:
                if cv2.getWindowProperty('Capstone Premium', cv2.WND_PROP_VISIBLE) < 1:
                    break
            except Exception:
                break

        cap.release()
        cv2.destroyAllWindows()
        cv2.waitKey(1)  # 윈도우 OS에서 창 잔상이 남지 않게 깔끔히 닫아줌


if __name__ == "__main__":
    app = CapstonePremium()
    app.run()