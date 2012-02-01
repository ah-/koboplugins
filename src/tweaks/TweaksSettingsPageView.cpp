#include "TweaksSettingsPageView.h"

#include <iostream>
#include "MainWindowController.h"
#include "TweaksSettingsView.h"

using namespace std;

TweaksSettingsPageView::TweaksSettingsPageView(QWidget *parent, QWidget *contentWidget)
    : QWidget(parent)
{
    cout << "TweaksSettingsPageView()" << endl << flush; 
    setupUi(this);

    contentWidget->setParent(this);
    setContentWidget(contentWidget);
    connect(header, SIGNAL(back()), this, SLOT(dismissDialog()));
}

void TweaksSettingsPageView::changeEvent(QEvent *e)
{
    cout << "TweaksSettingsPageView::changeEvent(QEvent *e)" << endl << flush;
}

void TweaksSettingsPageView::dismissDialog()
{
    cout << "TweaksSettingsPageView::dismissDialog()" << endl << flush;
    MainWindowController::sharedInstance()->popView(this);
}

QWidget *TweaksSettingsPageView::getContentWidget()
{
    return contentWidget;
}

void TweaksSettingsPageView::setContentWidget(QWidget *w)
{
    if (contentWidget)
        contentLayout->removeWidget(contentWidget);

    contentWidget = w;
    contentLayout->addWidget(w);
}
