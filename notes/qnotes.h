#ifndef QNOTES_H
#define QNOTES_H

#include <QWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QToolBar>
#include <QMouseEvent>
#include <QToolButton>
#include <QMenu>


class QNotes : public QWidget
{
    Q_OBJECT
public:
    explicit QNotes(QTextEdit *notesTextEdit,QWidget *parent=nullptr);

    QString getTextEdit(){return textEditNotes->toHtml();}
    void setTextEdit(QString text){textEditNotes->setText(text);}
    QTextEdit* getTextEditptr(){return textEditNotes;}
private:
    QTextEdit *textEditNotes;
    QToolBar *toolBarNotes;
    void createTooolBar();
    bool eventFilter(QObject *object, QEvent *event)
    {
        if (object == textEditNotes && event->type() == QEvent::MouseMove) {
            return true; // Предотвращаем обработку события перемещения мыши
        }
        return QWidget::eventFilter(object, event);
    }
private slots:

signals:
    void dataReady(QString value,QNotes *obj); //соед. с редактором
    void deletData(QNotes *value);
};

#endif // QNOTES_H
