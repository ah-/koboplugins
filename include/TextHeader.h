#ifndef __TEXT_HEADER_H__
#define __TEXT_HEADER_H__

class TextHeader : public QWidget {
    Q_OBJECT
public:
    void setText(const QString &);
    QString &getText();
signals:
    void mouseDown();
    void mouseUp();
    void clicked();
};

#endif
