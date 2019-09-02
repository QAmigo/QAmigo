#include "texttranslator.h"

#include <sstream>

TextTranslator::TextTranslator(QString string) :
    string(string)
{

}

QByteArray TextTranslator::toHex()
{
    QByteArray array;

    while (string.count() >= 2) {
        array.append(stringToHexByte(string.left(2)));
        string.remove(0, 2);
    }
    if (string.count() == 1)
        array.append(stringToHexByte(string));
    return array;
}

QString TextTranslator::getText()
{
    return string;
}

char TextTranslator::stringToHexByte(QString string)
{
    std::stringstream ss[2];
    int data[2];
    const char *byteData = string.toUtf8().constData();

    ss[0] << std::hex << byteData[0];
    ss[0] >> data[0];

    ss[1] << std::hex << byteData[1];
    ss[1] >> data[1];

    return static_cast<char>((data[0] << 4) | data[1]);
}
