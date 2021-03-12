#ifndef EDITORDIALOG_H
#define EDITORDIALOG_H

#include <QDialog>

#include "ui_editordialog.h"
#include "highlighter.h"

class EditorDialog : public QDialog
{
    Q_OBJECT
public:
    class Background: public QWidget {
    public:
        void setColor(const QColor &color);

    private:
        void paintEvent(QPaintEvent *e);
        QColor mColor;
    };

    EditorDialog(QWidget *parent = Q_NULLPTR);
    virtual ~EditorDialog();

public Q_SLOTS:
    void accept();
    void save();

private Q_SLOTS:
    void switchTheme();
    void switchFrame();
    void switchFont();
    void switchDefinitions();

private:
    Ui::EditorDialog *ui;
    Highlighter *mHighlighter;
};

#endif // EDITORDIALOG_H
