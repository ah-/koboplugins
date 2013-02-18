#include<QString>

class MenuEntry
{
public:
    MenuEntry(QString name, QString action, bool enabled);

    QString name();
    QString action();
    bool enabled();

    void setEnabled(bool enabled);

private:
    QString m_name;
    QString m_action;
    bool m_enabled;
};
