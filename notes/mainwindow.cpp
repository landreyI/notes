#include "mainwindow.h"
string fileName;
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    ifstream read("user.txt",ios::in);
    if(read.is_open())
    {
        string line;
        while (getline(read, line)) {
            fileName=line;
        }
    }

    resize(450, 600);
    setWindowFlags(Qt::CustomizeWindowHint);
    upperMenu();
    mainGridLayout = new QVBoxLayout();
    scrollArea =new QScrollArea();
    scrollArea->setMinimumWidth(300);
    scrollArea->setMaximumWidth(450);
    scrollArea->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    {scrollArea->setStyleSheet(
            "QScrollArea {"
            "   background-color: transparent;"
            "   border: none;"
            "}"
            "QScrollBar:vertical {"
            "   background-color: transparent;"
            "   width: 5px;" // Ширина полоски прокрутки
            "   margin: 0px;"
            "}"
            "QScrollBar::handle:vertical {"
            "   background-color: #808080;" // Серый цвет ползунка
            "   min-height: 20px;"
            "   border-radius: 2px;" // Увеличьте или уменьшите значение, чтобы изменить скругление углов ползунка
            "}"
            "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
            "   background-color: transparent;"
            "}"
            );}

    notesLayout = new QVBoxLayout(scrollArea);
    notesLayout->setContentsMargins(0, 0, 0, 0);

    centralWidget = new QWidget;
    centralWidget->setLayout(mainGridLayout);

    QLabel *label = new QLabel("Нотатки");
    label->setStyleSheet("font-size: 20px;");

    QLineEdit *search = new QLineEdit();
    QPalette palette = search->palette();
    palette.setColor(QPalette::PlaceholderText, Qt::white);
    search->setPalette(palette);
    search->setPlaceholderText("Пошук...");

    search->setMaximumSize(450, 30);
    search->setMinimumSize(300, 30);

    QVBoxLayout *layout = new QVBoxLayout();  // Создаем вертикальный макет для ячейки
    layout->addWidget(search);  // Добавляем поле в макет

    mainGridLayout->addWidget(label);
    mainGridLayout->addLayout(layout);
    mainGridLayout->addWidget(scrollArea);

    setCentralWidget(centralWidget);
    readFile(fileName);

    connect(search, &QLineEdit::textChanged, this, &MainWindow::filterTextEdit);
}

QString MainWindow::fileSelection()
{
    QDialog *selection = new QDialog();
    selection->setMaximumSize(250, 400);
    selection->setWindowTitle("Оберіть користувача");
    selection->setStyleSheet("QDialog { background-color: #303030; color: #FFFFFF; }"
                             "QDialog QPushButton { background-color: #303030; color: #FFFFFF; }");
    QDir directory("doc");
    QStringList fileNames = directory.entryList(QStringList() << "*.txt", QDir::Files);

    QVector<QString> users; // Множество для хранения существующих номеров

    for (const QString& fileName : fileNames) {
        // Извлекаем номер пользователя из имени файла
        QString numberStr = QFileInfo(fileName).baseName(); // Предполагается, что формат имени файла "userX.txt"
        users.push_back(numberStr);
    }
    QVBoxLayout *layoutMain = new QVBoxLayout(selection);
    layoutMain->setContentsMargins(0, 0, 0, 0);
    QScrollArea *scrollArea = new QScrollArea();
    {
        scrollArea->setStyleSheet(
            "QScrollArea {"
            "   background-color: transparent;"
            "   border: none;"
            "}"
            "QScrollBar:vertical {"
            "   background-color: transparent;"
            "   width: 5px;" // Ширина полоски прокрутки
            "   margin: 0px;"
            "}"
            "QScrollBar::handle:vertical {"
            "   background-color: #808080;" // Серый цвет ползунка
            "   min-height: 20px;"
            "   border-radius: 2px;" // Увеличьте или уменьшите значение, чтобы изменить скругление углов ползунка
            "}"
            "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
            "   background-color: transparent;"
            "}"
            );
    }
    scrollArea->setWidgetResizable(true);
    QVBoxLayout *layout = new QVBoxLayout(scrollArea);

    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QList<QPushButton*> listButton;
    QString selectedFile;
    for (auto &it : users) {
        QPushButton *button = new QPushButton(it);
        button->setMaximumHeight(30);
        listButton.append(button);
        layout->addWidget(button);

        connect(button, &QPushButton::clicked, [selection, &selectedFile, button]() {
            // Выполняем действие при нажатии на кнопку
            selectedFile = "doc/" + button->text() + ".txt";
            selection->accept(); // Закрыть диалоговое окно после выбора файла
        });
    }
    layoutMain->addWidget(scrollArea);
    if (selection->exec() == QDialog::Accepted) {
        return selectedFile;
    }
    return ""; // Возвращаем пустую строку, если ничего не выбрано
}

void MainWindow::upperMenu()
{
    QToolBar *toolbar = addToolBar("Toolbar");  // Создание верхней панели с названием "Toolbar"
    toolbar->setMovable(false);

    QAction *action1 = new QAction(this);
    action1->setIcon(QIcon("img/plus.png"));

    QToolButton *menuButton = new QToolButton(this);
    menuButton->setIcon(QIcon("img/settings.png"));
    menuButton->setPopupMode(QToolButton::InstantPopup); // Установка режима открытия выпадающего меню при нажатии на кнопку

    QMenu *file_menu = new QMenu(this);
    QList<QAction*> style_action ={
        new QAction("Імпорт нотаток", this),
        new QAction("Створити нового користувача", this),
        new QAction("Видалити користувача", this),
        new QAction("Змінити користувача", this),
    };
    file_menu->addActions(style_action);

    QAction *action3 = new QAction(this);
    action3->setIcon(QIcon("img/Off.png"));

    toolbar->addAction(action1);  // Добавление действия "Action 1" на панель

    QWidget *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    toolbar->addWidget(spacer);

    toolbar->addWidget(menuButton);  // Добавление кнопки меню на панель
    toolbar->addAction(action3);  // Добавление действия "Action 3" на панель

    // Установка выпадающего меню для кнопки
    menuButton->setMenu(file_menu);

    connect(action1, &QAction::triggered, this, &MainWindow::handleTextEditChanged);

    connect(style_action[0], &QAction::triggered, [this](){
        QString filePath = fileSelection();
        if (!filePath.isEmpty()) {
            // Пользователь выбрал файл
            readFile(filePath.toStdString());
        } else {
            // Пользователь не выбрал файл
            qDebug() << "Файл не выбран";
        }
    });
    connect(style_action[1], &QAction::triggered, [this](){
        writeFile(fileName);
        QDir directory("doc");
        QStringList fileNames = directory.entryList(QStringList() << "*.txt", QDir::Files);

        QSet<int> existingNumbers; // Множество для хранения существующих номеров

        for (const QString& fileName : fileNames) {
            // Извлекаем номер пользователя из имени файла
            QString numberStr = fileName.mid(4, fileName.length() - 8); // Предполагается, что формат имени файла "userX.txt"
            int number = numberStr.toInt();
            existingNumbers.insert(number);
        }

        int newNumber = 1;
        while (existingNumbers.contains(newNumber)) {
            newNumber++;
        }

        QString newUserName = "doc/user" + QString::number(newNumber)+".txt";
        QFile file(newUserName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            file.close();
            qDebug() << "Файл успешно создан.";
        } else {
            qDebug() << "Ошибка при создании файла.";
        }
        QString filePath = newUserName;
        if (!filePath.isEmpty()) {
            // Пользователь выбрал файл
            while (!qnotes_vec.empty()) {
                QNotes* note = qnotes_vec.back();
                deletNotes(note);
            }
            fileName=filePath.toStdString();
            qDebug() << "Выбранный файл: " << filePath;
        } else {
            // Пользователь не выбрал файл
            qDebug() << "Файл не выбран";
        }

    });
    connect(style_action[2], &QAction::triggered, [this](){
        QString filePath = fileSelection();
        if (!filePath.isEmpty()) {
            // Пользователь выбрал файл
            QFile file(filePath);
            if (file.remove()) {
                qDebug() << "Файл успешно удален";
            } else {
                qDebug() << "Ошибка удаления файла";
            }
            if(filePath.toStdString()==fileName)
            {
                while (!qnotes_vec.empty()) {
                    QNotes* note = qnotes_vec.back();
                    deletNotes(note);
                }
                fileName="";
            }
        } else {
            // Пользователь не выбрал файл
            qDebug() << "Файл не выбран";
        }
    });
    connect(style_action[3], &QAction::triggered, [this](){
        writeFile(fileName);
        QString filePath = fileSelection();
        if(filePath!="")
        {
            while (!qnotes_vec.empty()) {
                QNotes* note = qnotes_vec.back();
                deletNotes(note);
            }
            readFile(filePath.toStdString());
            fileName=filePath.toStdString();
        }
        else {
            // Пользователь не выбрал файл
            qDebug() << "Файл не выбран";
            fileName="";
        }
    });

    connect(action3, &QAction::triggered, [this](){
        writeFile(fileName);
        ofstream write("user.txt",ios::out);
        if (write.is_open()) {
            write << fileName;
        }
        else {write.close();qDebug() << "Ошибка записи";}
        write.close();
        QCoreApplication::quit();
    });
}

/////////////SLOTS
void MainWindow::filterTextEdit(const QString& text)
{
    for (QNotes* notes : qnotes_vec) {
        if (text.isEmpty() || notes->getTextEditptr()->toPlainText().startsWith(text, Qt::CaseInsensitive)) {
            notes->show();
        } else {
            notes->hide();
        }
    }
}

void MainWindow::handleTextEditChanged()
{
    QTextEdit *result = new QTextEdit();
    textEdit_vec.push_back(result);

    QNotes *nots=new QNotes(textEdit_vec.back());
    notesLayout->insertWidget(0,nots);
    notesLayout->addStretch(1);
    qnotes_vec.push_back(nots);

    connect(nots, &QNotes::dataReady, this, &MainWindow::addAction);
    connect(nots, &QNotes::deletData, this, &MainWindow::deletNotes);

    QWidget *widget = new QWidget();
    widget->setLayout(notesLayout);

    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);

    addAction(result->toPlainText(),qnotes_vec.back());
}

void MainWindow::addAction(QString textNotes,QNotes *obj)
{
    QDialog dialog;
    dialog.setWindowTitle(" ");
    dialog.setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    dialog.setStyleSheet("QDialog { background-color: #202020; color: #FFFFFF; }"
                         "QDialog QPushButton { background-color: #303030; color: #FFFFFF; }");


    dialog.resize(300, 250);
    dialog.show();

    QToolBar *toolbar = addToolBar("Toolbar");  // Создание верхней панели с названием "Toolbar"
    toolbar->setMovable(false);
    toolbar->setStyleSheet("QToolBar { spacing: -10px; padding: 0px;background-color: #FFCC00; }"
                           "QToolBar QToolButton:hover { background-color: #FFDB4D;}");

    QAction *action1 = new QAction(QIcon("img/plus.png"), "", this);

    QAction *action3 = new QAction(QIcon("img/Off.png"), "", this);

    toolbar->addAction(action1);  // Добавление действия "Action 1" на панель

    QWidget *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    toolbar->addWidget(spacer);
    toolbar->addAction(action3);  // Добавление действия "Action 3" на панель

    QTextEdit *textEdit = new QTextEdit(&dialog);
    textEdit->setText(textNotes);
    textEdit->setPlaceholderText("Ваш текст...");
    textEdit->setStyleSheet("background-color: #303030;");

    connect(action1, &QAction::triggered, this, MainWindow::handleTextEditChanged);

    connect(action3, &QAction::triggered, this,[=, &dialog]() {
        if(textEdit->toPlainText()=="")
        {
            deletNotes(qnotes_vec.back());
        }
        dialog.close();
    });

    QToolBar *lower_toolbar = addToolBar("Toolbar");  // Создание верхней панели с названием "Toolbar"
    lower_toolbar->setMovable(false);
    lower_toolbar->setStyleSheet("QToolBar { spacing: -10px; padding: 0px; background-color: #303030; }"
                                 "QToolBar QPushButton { background-color: transparent;border: 3px solid rgba(0, 0, 0, 0); }"
                                 "QToolBar QPushButton:checked { background-color: rgba(220, 220, 220, 0.2); color: #FFFFFF; }"
                                 "QToolBar QPushButton:hover { background-color: rgba(220, 220, 220, 0.1); }");

    QPushButton* toggleButton_bold = new QPushButton(this);
    toggleButton_bold->setIcon(QIcon("img/bold.png"));
    toggleButton_bold->setToolTip("жирний");
    QPushButton* toggleButton_italic = new QPushButton(this);
    toggleButton_italic->setIcon(QIcon("img/italic.png"));
    toggleButton_italic->setToolTip("курсивний");
    QPushButton* toggleButton_underline = new QPushButton(this);
    toggleButton_underline->setIcon(QIcon("img/underline.png"));
    toggleButton_underline->setToolTip("підкресилити");

    lower_toolbar->addWidget(toggleButton_bold);
    lower_toolbar->addWidget(toggleButton_italic);
    lower_toolbar->addWidget(toggleButton_underline);

    toggleButton_bold->setCheckable(true);
    toggleButton_italic->setCheckable(true);
    toggleButton_underline->setCheckable(true);

    connect(toggleButton_bold, &QPushButton::toggled, [this,textEdit](bool checked) {
        if (checked) {
            textEdit->setFontWeight(QFont::Bold);
        } else {
            textEdit->setFontWeight(QFont::Normal);
        }
    });
    connect(toggleButton_italic, &QPushButton::toggled, [this,textEdit](bool checked) {
        if (checked) {
            textEdit->setFontItalic(true);
        } else {
            textEdit->setFontItalic(false);
        }
    });
    connect(toggleButton_underline, &QPushButton::toggled, [this,textEdit](bool checked) {
        if (checked) {
            textEdit->setFontUnderline(true);
        } else {
            textEdit->setFontUnderline(false);
        }
    });

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    layout->setSpacing(0);
    layout->addWidget(toolbar);
    layout->addWidget(textEdit);
    layout->addWidget(lower_toolbar);
    layout->setContentsMargins(0, 0, 0, 0);

    connect(textEdit, &QTextEdit::textChanged, this, [=]() {
        notesLayout->removeWidget(obj);
        notesLayout->insertWidget(0, obj);
        obj->setTextEdit(textEdit->toHtml());
    });
    dialog.exec();
}

void MainWindow::deletNotes(QNotes *obj)
{
    if (!obj)
    {
        qDebug() << "Ошибка: Неверный указатель на QNotes.";
        return;
    }

    auto it = std::find(qnotes_vec.begin(), qnotes_vec.end(), obj);
    if (it == qnotes_vec.end())
    {
        qDebug() << "Ошибка: Указатель на QNotes не найден в векторе.";
        return;
    }

    // Получите индекс элемента в QVBoxLayout
    int index = notesLayout->indexOf(obj);
    if (index == -1)
    {
        qDebug() << "Ошибка: QNotes не найден в QVBoxLayout.";
        return;
    }

    // Удалите элемент из QVBoxLayout
    QLayoutItem* item = notesLayout->takeAt(index);
    if (!item)
    {
        qDebug() << "Ошибка: Неверный элемент в QVBoxLayout.";
        return;
    }
    delete item->widget();
    delete item;

    // Удалите указатель на QNotes из вектора
    size_t vecIndex = std::distance(qnotes_vec.begin(), it);
    if (vecIndex >= qnotes_vec.size())
    {
        qDebug() << "Ошибка: Неверный индекс вектора qnotes_vec.";
        return;
    }
    qnotes_vec.erase(qnotes_vec.begin() + vecIndex);
    // Обновите макет
    mainGridLayout->update();
}


MainWindow::~MainWindow()
{
    delete mainGridLayout;
}

void MainWindow::writeFile(string nameFile)
{
    ofstream write(nameFile,ios::out);
    if (write.is_open()) {
        for (auto& it : qnotes_vec) {
            const QString& text = it->getTextEdit();
            std::string textStd = text.toStdString();
            write << textStd<<endl<<"----нова----"<<endl;
        }
        write.close();
    }
    else {write.close();qDebug() << "Ошибка записи";}
}
void MainWindow::readFile(string nameFile)
{
    ifstream read(nameFile,ios::in);
    if(read.is_open())
    {
        string textNotes;
        string line;
        while (getline(read, line)) {
            if(line=="----нова----")
            {
                if(textNotes!="")
                {QTextEdit *result = new QTextEdit();
                    QString qstr1 = QString::fromStdString(textNotes);
                    result->setText(qstr1);
                    textEdit_vec.push_back(result);

                    QNotes *nots=new QNotes(textEdit_vec.back());
                    qnotes_vec.push_back(nots);
                    notesLayout->insertWidget(0,nots);
                    notesLayout->addStretch(1);


                    connect(nots, &QNotes::dataReady, this, &MainWindow::addAction);
                    connect(nots, &QNotes::deletData, this, &MainWindow::deletNotes);

                    QWidget *widget = new QWidget();
                    widget->setLayout(notesLayout);

                    scrollArea->setWidgetResizable(true);
                    scrollArea->setWidget(widget);
                    textNotes="";}
            }
            else
            {
                textNotes += line + "\n";
            }
        }
        read.close();
    }
    else {read.close();qDebug() << "Ошибка считывания";}
}
