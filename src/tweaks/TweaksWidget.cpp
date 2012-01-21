#include "TweaksWidget.h"
#include <iostream>
#include "../../include/LedManager.h"


using namespace std;

TweaksWidget::TweaksWidget() :
    QPushButton() {}

TweaksWidget::TweaksWidget(const QString& text, QWidget *parent=0) :
    QPushButton(text, parent)
{
   cout << "TweaksWidget()" << endl << flush; 
   QWidget::setAttribute(Qt::WA_AcceptTouchEvents);
   QObject::connect(this, SIGNAL(clicked(bool)), this, SLOT(onPush(bool)));

   // TODO: register with qtscript
   // TODO: hide "advertising" at the bottom
   // TODO: turn leds on/off
   // TODO: disconnect wirelesswatchdog timer signal
}

bool TweaksWidget::event(QEvent* event) {
    cout << "TweaksWidget::event " << event->type() << endl << flush;
    return QPushButton::event(event);
}

void TweaksWidget::onPush(bool)
{
   cout << "TweaksWidget::onPush()" << endl << flush; 
   setText(text() + " push");

   LedManager *lm = LedManager::sharedInstance();
   cout << "Kobo LedManager: " << lm << endl << flush; 
   lm->blueOn();
   cout << "Kobo LedManager turned blue on" << endl << flush; 
}

void TweaksWidget::onResume()
{
   cout << "Kobo onResume()" << endl << flush; 
   setText(text() + " onResume");
   emit setHeaderText("TweaksWidget::onResume()");
   //emit openFooterMenu();
}

void TweaksWidget::goToNextPage()
{
    cout << "Kobo goToNextPage() " << endl << flush;
    emit closeFooterMenu();
}

void TweaksWidget::goToPrevPage()
{
   cout << "Kobo goToPrevPage() " << endl << flush;
   emit setHeaderText("Hello Kobo: PrevPage");
   emit openFooterMenu();
}

void TweaksWidget::onDisplayMenuRequested(QList<PluginInterface::MenuGroup>&)
{
    cout << "Kobo onDisplayMenuRequested() " << endl << flush;
}

void TweaksWidget::displayTitlePage()
{
    cout << "Kobo displayTitlePage() " << endl << flush;
}

void TweaksWidget::homeKeyPressed()
{
    cout << "Kobo homeKeyPressed() " << endl << flush;
}
