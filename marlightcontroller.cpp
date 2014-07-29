#include "marlightcontroller.h"

MarlightController::MarlightController(QString *s, int p)
{
    // create socket
    socket = new QUdpSocket();

    // save server address and port
    server = new QHostAddress(*s);
    port = p;

    // setup default channels value to 0
    channels = 0;
}

MarlightController::MarlightController(QString *s)
{
    MarlightController(s, 50000);
}

// Send datagram for each selected channel
void MarlightController::sendData(QByteArray *data)
{
    unsigned char cmd_buf[2] = { 0, 0x55 };

    for (int i=0; i<4; i++) {
        if (channels & (1<<i)) {
            cmd_buf[0] = 0x08 + i * 2;
            socket->writeDatagram(QByteArray::fromRawData((const char *) cmd_buf, 2), *server, port); // activate required channel
            socket->writeDatagram(*data, *server, port); // send command
        }
    }
}

void MarlightController::sendData(unsigned char cmd)
{
    unsigned char cmd_buf[] = { cmd, 0x55 };

    sendData(*QByteArray::fromRawData((const char *) cmd_buf, sizeof(cmd_buf)));
}

void MarlightController::selectChannels(int ch)
{
    channels = ch & 0x0F;
}

void MarlightController::setMode(mode_t m)
{
    mode = m; /* save mode */

    switch (mode) {
    case MODE_RGB:
        sendData(0x12); // command: RGB mode ON
        break;
    case MODE_WHITE:
        // try just to increase and decrease white-mode brightness,
        // since there's no command to get to white mode
        increaseBrightness();
        decreaseBrightness();
        break;
    }
}

void MarlightController::increaseBrightness()
{
    switch (mode) {
    case MODE_RGB:
        sendData(0x11); // command: RGB brightness up
        break;
    case MODE_WHITE:
        sendData(0x03); // command: white brightness up
        break;
    }
}

void MarlightController::decreaseBrightness()
{
    switch (mode) {
    case MODE_RGB:
        sendData(0x10); // command: RGB brightness down
        break;
    case MODE_WHITE:
        sendData(0x04); // command: white brightness down
        break;
    }
}

void MarlightController::increaseTemp()
{
    if (mode == MODE_WHITE) {
        sendData(0x06); // command: white temp warmer
    }
}

void MarlightController::decreaseTemp()
{
    if (mode == MODE_WHITE) {
        sendData(0x05); // command: white temp warmer
    }
}

void MarlightController::resetValues()
{
    sendData(0x07); // command: reset temperature and brightness
}

void MarlightController::powerOn()
{
    unsigned char cmd_buf[2] = { 0, 0x55 };
    if (channels == 0x0F) { // there is a special command to turn on all channels
        cmd_buf[0] = 0x01; // command: turn all channels on
        socket->writeDatagram(QByteArray::fromRawData((const char *) cmd_buf, 2), *server, port);
    } else {
        for (int i=0; i<4; i++) {
            if (channels & (1<<i)) {
                cmd_buf[0] = 0x08 + i * 2;
                socket->writeDatagram(QByteArray::fromRawData((const char *) cmd_buf, 2), *server, port); // command to turn on
            }
        }
    }
}

void MarlightController::powerOff()
{
    unsigned char cmd_buf[2] = { 0, 0x55 };
    if (channels == 0x0F) { // there is a special command to turn off all channels
        cmd_buf[0] = 0x02; // command: turn all channels off
        socket->writeDatagram(QByteArray::fromRawData((const char *) cmd_buf, 2), *server, port);
    } else {
        for (int i=0; i<4; i++) {
            if (channels & (1<<i)) {
                cmd_buf[0] = 0x09 + i * 2;
                socket->writeDatagram(QByteArray::fromRawData((const char *) cmd_buf, 2), *server, port); // command to turn off
            }
        }
    }
}

void MarlightController::writeRGBValue(int r, int g, int b)
{
    unsigned char cmd_buf[] = { 0x13, r, g, b, 0x55 }; // command: write rgb value

    sendData(*QByteArray::fromRawData((const char *) cmd_buf, sizeof(cmd_buf)));
}

void MarlightController::setPresetMode(enum preset_t preset)
{
    sendData(preset); // just so simple :)
}

void MarlightController::timerPreset(int h1, int m1, int h2, int m2)
{
    unsigned char cmd_buf[] = { 0x18, h1, m1, h2, m2, 0x09, 0x14, 0x55 }; // command: mode_timer

    sendData(*QByteArray::fromRawData((const char *) cmd_buf, sizeof(cmd_buf)));
}

void MarlightController::alarmPreset(int h1, int m1)
{
    unsigned char cmd_buf[] = { 0x19, h1, m1, 0x09, 0x14, 0x55 }; // command: mode_alarm

    sendData(*QByteArray::fromRawData((const char *) cmd_buf, sizeof(cmd_buf)));
}

void MarlightController::recreationPreset(int r, int g, int b)
{
    unsigned char cmd_buf[] = { 0x1B, r, g, b, 0x55 }; // command: mode_recreation

    sendData(*QByteArray::fromRawData((const char *) cmd_buf, sizeof(cmd_buf)));
}
