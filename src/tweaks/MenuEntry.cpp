#include "MenuEntry.h"

MenuEntry::MenuEntry(QString name, QString action, bool enabled)
    : m_name(name), m_action(action), m_enabled(enabled)
{}

QString MenuEntry::name() {
    return m_name;
}

QString MenuEntry::action() {
    return m_action;
}

bool MenuEntry::enabled() {
    return m_enabled;
}

void MenuEntry::setEnabled(bool enabled) {
    m_enabled = enabled;
}
