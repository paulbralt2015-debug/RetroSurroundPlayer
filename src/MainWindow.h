#pragma once

#include <QMainWindow>

class QComboBox;
class QLabel;
class QMediaPlayer;
class QAudioOutput;
class QSlider;
class QVideoWidget;
class QPushButton;

class MainWindow final : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void openFile();
    void togglePlayback();
    void stopPlayback();
    void updatePosition(qint64 position);
    void updateDuration(qint64 duration);
    void seek(int value);
    void updateStatus();

private:
    QMediaPlayer* player_ = nullptr;
    QAudioOutput* audioOutput_ = nullptr;
    QVideoWidget* videoWidget_ = nullptr;
    QSlider* timeline_ = nullptr;
    QSlider* volume_ = nullptr;
    QPushButton* playButton_ = nullptr;
    QLabel* statusLabel_ = nullptr;
    QComboBox* audioMode_ = nullptr;
    bool userSeeking_ = false;
};
