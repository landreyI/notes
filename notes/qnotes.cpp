#include "qnotes.h"
#include "mainwindow.h"
QNotes::QNotes(QTextEdit *notesTextEdit,QWidget *parent) : QWidget(parent),textEditNotes(notesTextEdit)
{
    setStyleSheet("QWidget { border-radius: 10px; background-color: #FFCC00; }");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    textEditNotes->setPlaceholderText("Напишіть вашу нотатку...");
    textEditNotes->setReadOnly(true);
    textEditNotes->setMouseTracking(false);
    textEditNotes->installEventFilter(this);
    textEditNotes->setMaximumSize(450, 130);
    textEditNotes->setMinimumSize(300, 55);
    textEditNotes->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    textEditNotes->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    textEditNotes->setSizeAdjustPolicy(QTextEdit::AdjustToContents);
    textEditNotes->setTextInteractionFlags(Qt::NoTextInteraction);
    textEditNotes->setStyleSheet("border-top-left-radius: 0px; border-top-right-radius: 0px; background-color: #202020;");

    createTooolBar();

    QVBoxLayout  *layout = new QVBoxLayout (this);
    layout->setSpacing(0);
    layout->addWidget(toolBarNotes);
    layout->addWidget(textEditNotes);
    layout->addStretch();
    layout->setContentsMargins(0, 0, 0, 0);
}

void QNotes::createTooolBar()
{
    toolBarNotes=new QToolBar();
    toolBarNotes->setMovable(false);
    toolBarNotes->setStyleSheet("QToolBar { border-bottom-left-radius: 0px; border-bottom-right-radius: 0px;spacing: -10px; padding: 0px; }"
                                "QToolBar QToolButton:hover { background-color: #FFFFFF;}");
    toolBarNotes->setMaximumSize(450, 20);

    QToolButton *toolButton = new QToolButton(this);
    QMenu *menu = new QMenu(this);
    menu->setStyleSheet("QMenu { background-color: #303030; }QMenu::item:selected { color: #FFCC00; }");
    // Создание элементов меню
    QAction *item1 = new QAction("Відкрити замітку", this);

    QAction *item2 = new QAction("Видалити замітку", this);
    // Добавление элементов меню в выпадающий список
    menu->addAction(item1);
    menu->addAction(item2);
    // Установка выпадающего списка для QToolButton
    toolButton->setIcon(QIcon("img/Dot3.png"));
    toolButton->setMenu(menu);
    toolButton->setPopupMode(QToolButton::InstantPopup);
    toolBarNotes->addWidget(toolButton);

    connect(item1, &QAction::triggered, this, [=]() {
        QString text = getTextEdit();
        emit dataReady(text,this);
    });
    connect(item2, &QAction::triggered, this, [=]() {
        emit deletData(this);
    });
}
