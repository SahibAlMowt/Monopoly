#include "audio.h"

AudioPlayer::AudioPlayer()
{
    player = new QMediaPlayer;
}

void AudioPlayer::playSound(const QString &filePath)
{
    player->setMedia(QUrl::fromLocalFile(filePath));
    player->setVolume(100);
    player->play();
}
