package finalproject;

import javax.sound.sampled.*;
import java.net.URL;

public class SoundPlayer {
    public static void play(String soundFile) {
        try {
            URL soundURL = SoundPlayer.class.getResource(soundFile);
            if (soundURL == null) {
                System.err.println("리소스를 찾을 수 없습니다: " + soundFile);
                return;
            }

            AudioInputStream audioIn = AudioSystem.getAudioInputStream(soundURL);
            Clip clip = AudioSystem.getClip();
            clip.open(audioIn);
            clip.start();
        } catch (Exception e) {
            System.err.println("사운드 재생 실패: " + e.getMessage());
            e.printStackTrace();
        }
    }
}
