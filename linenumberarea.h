#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include <QWidget>
#include "codeeditor.h"

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor), mCodeEditor(editor)
    {}

    QSize sizeHint() const override
    {
        return QSize(mCodeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        mCodeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *mCodeEditor;
};

#endif // LINENUMBERAREA_H
