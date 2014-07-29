#ifndef MARLIGHTCONTROLLER_H
#define MARLIGHTCONTROLLER_H

#include "marlightcontroller_global.h"
#include <QUdpSocket>
#include <string>

class MARLIGHTCONTROLLERSHARED_EXPORT MarlightController
{

public:
    enum mode_t { MODE_RGB, MODE_WHITE };
    enum preset_t { PRESET_NIGHT = 0x14, PRESET_MEETING = 0x15, PRESET_READING = 0x16, PRESET_MODE = 0x17, PRESET_SLEEP = 0x1A };

    MarlightController(QString *server, int port);
    MarlightController(QString *server); // for default port 50000

    void selectChannels(int ch);
    void setMode(enum mode_t mode);

    void increaseBrightness();
    void decreaseBrightness();

    void increaseTemp();
    void decreaseTemp();

    void powerOn();
    void powerOff();

    void writeRGBValue(int r, int g, int b);

    void resetValues();

    void setPresetMode(enum preset_t preset);
    void timerPreset(int h1, int m1, int h2, int m2);
    void alarmPreset(int h1, int m1);
    void recreationPreset(int r, int g, int b);

private:
    QUdpSocket *socket;
    QHostAddress *server;
    int port;

    int channels;
    enum mode_t mode;

    void sendData(QByteArray *data);
    void sendData(unsigned char cmd);

};

#endif // MARLIGHTCONTROLLER_H
