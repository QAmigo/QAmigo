#ifndef TEXTTRANSLATOR_H
#define TEXTTRANSLATOR_H

#include <QString>

class TextTranslator
{
public:
    TextTranslator(QString string);

    QByteArray toHex();
    QString getText();

private:
    char stringToHexByte(QString string);

private:
    QString string;
};

#endif // TEXTTRANSLATOR_H
