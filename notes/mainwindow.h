#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qnotes.h"

#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QStyleFactory>
#include <QCoreApplication>
#include <QLabel>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QIcon>
#include <QFileDialog>
#include <QMouseEvent>
#include <QDialog>
#include <QScrollArea>
#include <QPushButton>

#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
using namespace std;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void upperMenu(); //верхнее меню
    QString fileSelection();
    void writeFile(string nameFile);
    void readFile(string nameFile);

    QWidget *centralWidget;
    QVBoxLayout *mainGridLayout;
    vector<QNotes*> qnotes_vec;
    vector<QTextEdit*> textEdit_vec;
    QVBoxLayout  *notesLayout;
    QScrollArea *scrollArea;
protected:
    void mousePressEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton) {
            // Запоминаем начальные координаты окна и позицию нажатия мыши
            dragPosition = event->globalPos() - frameGeometry().topLeft();
            event->accept();
        }
    }
    void mouseMoveEvent(QMouseEvent *event) override
    {
        if (event->buttons() & Qt::LeftButton) {
            // Перемещаем окно на новые координаты, относительно начальных координат и перемещения мыши
            move(event->globalPos() - dragPosition);
            event->accept();
        }
    }
private:
    QPoint dragPosition;
private slots:
    void addAction(QString textNotes,QNotes *obj); //редактор записки
    void handleTextEditChanged(); //добавление записки
    void deletNotes(QNotes *obj); //удаление записки из окна и вектора
    void filterTextEdit(const QString& text); //поиск записок
};
#endif // MAINWINDOW_H
