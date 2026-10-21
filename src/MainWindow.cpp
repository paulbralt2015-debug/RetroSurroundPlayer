#include "MainWindow.h"
#include "SurroundProcessor.h"

#include <QAudioOutput>
#include <QComboBox>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QMediaPlayer>
#include <QPushButton>
#include <QSlider>
#include <QStyle>
#include <QVideoWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QUrl>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("RetroSurround Player 0.1");
    resize(980, 620);

    auto* central = new QWidget(this);
    auto* root = new QVBoxLayout(central);
    root->setContentsMargins(8, 8, 8, 8);
    root->setSpacing(6);

    videoWidget_ = new QVideoWidget(central);
    videoWidget_->setMinimumSize(640, 360);
    videoWidget_->setStyleSheet("background: #101010; border: 1px solid #555;");
    root->addWidget(videoWidget_, 1);

    timeline_ = new QSlider(Qt::Horizontal, central);
    timeline_->setRange(0, 0);
    root->addWidget(timeline_);

    auto* controls = new QHBoxLayout();
    auto* openButton = new QPushButton("Ouvrir…", central);
    playButton_ = new QPushButton("Lire", central);
    auto* stopButton = new QPushButton("Arrêter", central);
    controls->addWidget(openButton);
    controls->addWidget(playButton_);
    controls->addWidget(stopButton);

    controls->addSpacing(12);
    controls->addWidget(new QLabel("Audio :", central));
    audioMode_ = new QComboBox(central);
    audioMode_->addItem("Direct (sans traitement)", static_cast<int>(SurroundMode::Direct));
    audioMode_->addItem("Surround virtuel (expérimental)", static_cast<int>(SurroundMode::StereoWidening));
    audioMode_->addItem("Matrice Dolby Surround (expérimental)", static_cast<int>(SurroundMode::MatrixSurround));
    audioMode_->setToolTip("Prototype d'interface : le DSP n'est pas encore raccordé au flux audio.");
    controls->addWidget(audioMode_, 1);

    controls->addWidget(new QLabel("Volume :", central));
    volume_ = new QSlider(Qt::Horizontal, central);
    volume_->setRange(0, 100);
    volume_->setValue(70);
    volume_->setMaximumWidth(130);
    controls->addWidget(volume_);
    root->addLayout(controls);

    statusLabel_ = new QLabel("Prêt — ouvrez un fichier vidéo.", central);
    statusLabel_->setStyleSheet("color: #333; padding: 3px;");
    root->addWidget(statusLabel_);

    setCentralWidget(central);

    player_ = new QMediaPlayer(this);
    audioOutput_ = new QAudioOutput(this);
    player_->setAudioOutput(audioOutput_);
    player_->setVideoOutput(videoWidget_);
    audioOutput_->setVolume(0.70f);

    connect(openButton, &QPushButton::clicked, this, &MainWindow::openFile);
    connect(playButton_, &QPushButton::clicked, this, &MainWindow::togglePlayback);
    connect(stopButton, &QPushButton::clicked, this, &MainWindow::stopPlayback);
    connect(player_, &QMediaPlayer::positionChanged, this, &MainWindow::updatePosition);
    connect(player_, &QMediaPlayer::durationChanged, this, &MainWindow::updateDuration);
    connect(player_, &QMediaPlayer::playbackStateChanged, this, &MainWindow::updateStatus);
    connect(timeline_, &QSlider::sliderPressed, this, [this] { userSeeking_ = true; });
    connect(timeline_, &QSlider::sliderReleased, this, [this] {
        userSeeking_ = false;
        seek(timeline_->value());
    });
    connect(volume_, &QSlider::valueChanged, this, [this](int value) {
        audioOutput_->setVolume(static_cast<float>(value) / 100.0f);
    });
    connect(player_, &QMediaPlayer::errorOccurred, this,
        [this](QMediaPlayer::Error, const QString& error) {
            statusLabel_->setText("Erreur de lecture : " + error);
        });

    // Classic desktop-player styling.
    setStyleSheet(
        "QMainWindow { background: #d6d6d6; }"
        "QPushButton, QComboBox { padding: 4px 8px; }"
        "QSlider::groove:horizontal { height: 5px; background: #999; }"
        "QSlider::handle:horizontal { background: #f2f2f2; border: 1px solid #555; width: 12px; margin: -5px 0; }"
    );
}

void MainWindow::openFile() {
    const QString path = QFileDialog::getOpenFileName(
        this, "Ouvrir une vidéo", QString(),
        "Fichiers vidéo (*.mp4 *.mkv *.avi *.mov *.wmv *.mpeg *.mpg *.m4v);;Tous les fichiers (*.*)");
    if (path.isEmpty()) return;

    player_->setSource(QUrl::fromLocalFile(path));
    player_->play();
    playButton_->setText("Pause");
    statusLabel_->setText("Lecture : " + path);
}

void MainWindow::togglePlayback() {
    if (player_->playbackState() == QMediaPlayer::PlayingState) {
        player_->pause();
        playButton_->setText("Lire");
    } else {
        player_->play();
        playButton_->setText("Pause");
    }
}

void MainWindow::stopPlayback() {
    player_->stop();
    playButton_->setText("Lire");
}

void MainWindow::updatePosition(qint64 position) {
    if (!userSeeking_) timeline_->setValue(static_cast<int>(position));
}

void MainWindow::updateDuration(qint64 duration) {
    timeline_->setRange(0, static_cast<int>(duration));
}

void MainWindow::seek(int value) {
    player_->setPosition(static_cast<qint64>(value));
}

void MainWindow::updateStatus() {
    if (player_->playbackState() == QMediaPlayer::PlayingState) {
        playButton_->setText("Pause");
    } else {
        playButton_->setText("Lire");
    }
}
