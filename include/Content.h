#ifndef __CONTENT_H__
#define __CONTENT_H__

#include <QtCore>

class ScObject {
public:
    virtual ~ScObject();
    virtual void getDBRetrieveFields(void);
};

class Content {

public:
    QString s1;
    QString s2;
    QString s3;
    QString s4;
    QString s5;
    QString s6;
    QString s7;
    QString s8;
    QDateTime dt1;
    QString s9;
    QString s10;
    QString s11;
    QString s12;
    QString s13;
    QDateTime dt2;
    QString s14;
    QString s15;
    QString s16;
    QString s17;
    QDateTime dt3;
    QString s[20];
    int foo[200];

public:
    bool isValid() const;
    QString *getId() const;
    void getAttribution();
    void setAttribute(QString const&, QVariant const&);
    void setAttribution(QString const&);
    void setBookId(QString const&);
    void setBookmarkWordOffset(int);
    void setContentType(QString const&);
    void setDate(QString const&);
    void setDescription(QString const&);
    void setEncrypted(bool);
    void setFirstTimeReading(bool);
    void setId(QString const&);
    void setImageId(QString const&);
    virtual void setMimeType(QString const&);
    virtual void setNumPages(int);
    void setParagraphBookmarked(int);
    void setPublisher(QString const&);
    virtual void setTitle(QString const&);
    void setUserID(QString const&);
    void setVolumeIndex(int);
};

#endif
