package finalproject;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class QuizGameUI extends JFrame {

    private CardLayout cardLayout = new CardLayout();
    private JPanel mainPanel = new JPanel(cardLayout);

    private JTextField nameField = new JTextField(20);
    private JButton startButton = new JButton("게임 시작");

    private JLabel questionLabel = new JLabel("", SwingConstants.CENTER);
    private JTextField answerField = new JTextField(20);
    private JButton submitButton = new JButton("답 제출");
    private JLabel feedbackLabel = new JLabel("", SwingConstants.CENTER);

    private String playerName;
    private int currentQuestion = 0;
    private int score = 0;

    private String[] questions = {
        "타이타닉의 구명 보트에는 몇 명이 탈수 있을까?",
        "금은 금인데 도둑고양이에게 가장 어울리는 금은?",
        "자동차가 놀라면?",
        "별 중에 가장 슬픈 별은?",
        "진짜 새의 이름은 무엇일까요?",
        "사람의 몸무게가 가장 많이 나갈 때는?",
        "콩이 바쁘면?",
        "쥐가 네 마리 모이면?",
        "매일 욕을 만드는 도시는?",
        "있어도 기분이 안 좋은 금은?",
        "세계에서 가장 큰 양은?",
        "사과를 한 입 베어 먹으면?",
        "싸움을 즐겨 하는 나라는?",
        "오이의 나이는?",
        "세 사람만 탈 수 있는 차는?"
    };

    private String[] answers = {
        "9명", "야금야금", "카놀라유", "이별", "참새", "철들때", "콩비지", "쥐포",
        "뉴욕", "세금", "태양", "파인애플", "칠레", "52세", "인삼차"
    };

    private boolean waitingForNext = false;

    public QuizGameUI() {
        setTitle("넌센스 퀴즈 게임");
        setSize(520, 380);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setLocationRelativeTo(null);

        // 이름 입력 화면
        JPanel namePanel = new JPanel(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();

        JLabel promptLabel = new JLabel("플레이어 이름을 입력하세요:");
        gbc.gridx = 0; gbc.gridy = 0; gbc.insets = new Insets(10,10,10,10);
        namePanel.add(promptLabel, gbc);

        gbc.gridy = 1;
        namePanel.add(nameField, gbc);

        gbc.gridy = 2;
        namePanel.add(startButton, gbc);

        // 퀴즈 화면
        JPanel quizPanel = new JPanel(new GridBagLayout());
        gbc = new GridBagConstraints();

        gbc.gridx = 0; gbc.gridy = 0; gbc.insets = new Insets(10,10,10,10);
        questionLabel.setFont(new Font("Serif", Font.BOLD, 18));
        quizPanel.add(questionLabel, gbc);

        gbc.gridy = 1;
        answerField.setPreferredSize(new Dimension(300, 30));
        quizPanel.add(answerField, gbc);

        gbc.gridy = 2;
        quizPanel.add(submitButton, gbc);

        gbc.gridy = 3;
        feedbackLabel.setFont(new Font("Serif", Font.ITALIC, 16));
        quizPanel.add(feedbackLabel, gbc);

        mainPanel.add(namePanel, "NAME");
        mainPanel.add(quizPanel, "QUIZ");

        add(mainPanel);
        cardLayout.show(mainPanel, "NAME");

        startButton.addActionListener(e -> {
            playerName = nameField.getText().trim();
            if (playerName.isEmpty()) {
                JOptionPane.showMessageDialog(this, "이름을 입력하세요!", "경고", JOptionPane.WARNING_MESSAGE);
                return;
            }
            currentQuestion = 0;
            score = 0;
            answerField.setText("");
            feedbackLabel.setText("");
            waitingForNext = false;
            showQuestion();
            cardLayout.show(mainPanel, "QUIZ");
        });

        submitButton.addActionListener(e -> {
            if (waitingForNext) return;

            String input = answerField.getText().trim();
            if (input.isEmpty()) {
                JOptionPane.showMessageDialog(this, "답을 입력하세요!", "경고", JOptionPane.WARNING_MESSAGE);
                return;
            }
            checkAnswer(input);
        });
    }

    private void showQuestion() {
        if (currentQuestion < questions.length) {
            questionLabel.setText("문제 " + (currentQuestion + 1) + ": " + questions[currentQuestion]);
            answerField.setText("");
            feedbackLabel.setText("");
            waitingForNext = false;
            answerField.requestFocusInWindow();
        } else {
            showResultOptions();
        }
    }

    private void showResultOptions() {
        int result = JOptionPane.showOptionDialog(this,
            playerName + "님, 게임 종료! 점수: " + score + " / " + questions.length + "\n다시 플레이하시겠습니까?",
            "게임 종료",
            JOptionPane.YES_NO_OPTION,
            JOptionPane.INFORMATION_MESSAGE,
            null,
            new Object[] {"다시 하기", "메인 화면으로 돌아가기"},
            "다시 하기");

        if (result == JOptionPane.YES_OPTION) {
            currentQuestion = 0;
            score = 0;
            answerField.setText("");
            feedbackLabel.setText("");
            waitingForNext = false;
            showQuestion();
            cardLayout.show(mainPanel, "QUIZ");
        } else {
            cardLayout.show(mainPanel, "NAME");
        }
    }

    private void checkAnswer(String input) {
        waitingForNext = true;

        if (input.equals(answers[currentQuestion])) {
            feedbackLabel.setText("정답입니다!");
            score++;
            SoundPlayer.play("/finalproject/ding-47489.wav");

            
            Timer timer = new Timer(3000, e -> showQuestion());
            timer.setRepeats(false);
            timer.start();
        } else {
            feedbackLabel.setText("틀렸습니다! 정답은 [" + answers[currentQuestion] + "] 입니다.");
            SoundPlayer.play("/finalproject/bell-98033.wav");

            
            Timer timer = new Timer(2000, e -> showQuestion());
            timer.setRepeats(false);
            timer.start();
        }

        currentQuestion++;
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> new QuizGameUI().setVisible(true));
    }
}
