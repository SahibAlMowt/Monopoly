#ifndef AUDIO_H
#define AUDIO_H

#include <QMediaPlayer>
#include <QUrl>

class AudioPlayer : public QObject
{

public:

    AudioPlayer();

    void playSound(const QString &filePath);

private:

    QMediaPlayer *player;

};


#endif // AUDIO_H
