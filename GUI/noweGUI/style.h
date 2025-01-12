#ifndef STYLE_H
#define STYLE_H

#include <QObject>
#include <QApplication>

class StyleManager : public QObject
{
    Q_OBJECT

public:
    explicit StyleManager(QObject *parent = nullptr);
    static void applyStyle();
};

#endif // STYLE_H
