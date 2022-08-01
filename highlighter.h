#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QWidget>
#include <SyntaxHighlighter>
#include <Repository>
#include <Definition>
#include <Theme>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "codeeditor.h"

class Highlighter : public QWidget
{
    Q_OBJECT

public:
    struct BackgroundOption {
        int round = 5;
        int margins = 10;
        QString background = "#000000";
        int titlebarHeight = 30;
    };

    struct ButtonOption {
        int width = 10;
        QColor color;
    };

    struct FrameButtonsOptions {
        QList<ButtonOption> buttons;
        int spacing = 5;
        int margins = 10;
        Qt::LayoutDirection direction = Qt::LeftToRight;
    };

    struct EditorFontOptions {
        QString family;
        int size;
    };

    struct EditorOptions {
        int margins = 20;
    };

    struct Options {
        BackgroundOption background;
        FrameButtonsOptions buttons;
        EditorOptions editor;
    };

    Highlighter(bool lineNumbers, QWidget *parent = nullptr);
    Highlighter(QWidget *parent = nullptr);
    ~Highlighter();

    Options frameOptions() const;

    QString code() const;
    void setCode(const QString &code);
    void setPlainText(const QString &text);

    void save(const QString &path, int width);

    QString file() const;
    void setFile(const QString &file);

    static QStringList themes();
    static QStringList definitions();

    void setDefinitions(const QString &definition);
    void setReadOnly(bool readOnly);

    EditorFontOptions fontOptions() const;
    void setFontOptions(const EditorFontOptions &fontOptions);

    QString theme() const;
    void setTheme(const QString &theme);

    QString frame() const;
    void setFrame(const QString &frame);

    static QMap<QString, QString> frames();

    qint32 minWidth() const;
    void setMinWidth(const qint32 &minWidth);

    qint32 maxWidth() const;
    void setMaxWidth(const qint32 &maxWidth);

    const QString &language() const;
    void setLanguage(const QString &newLanguage);

protected:
    void paintEvent(QPaintEvent *e);
    void paintBackground(QPainter &painter);
    void paintFrameButtons(QPainter &painter);
    void setFrameOptions(const Options &frameOptions);

private:
    Options mFrameOptions;
    EditorFontOptions mFontOptions;
    QString mTheme;
    QString mFrame;
    qint32 mMinWidth = 0;
    qint32 mMaxWidth = 1024;
    QString mLanguage;

    QString mCode;
    QString mFile;

    CodeEditor *mEditor;
    QVBoxLayout *mLayout;

    KSyntaxHighlighting::SyntaxHighlighter *mHighlighter;
    KSyntaxHighlighting::Repository mRepo;

    bool mEditMode;
};

static void operator<<(Highlighter::BackgroundOption &option, const QJsonObject &obj)
{
    option.background = obj["background"].toString();
    option.round = obj["round"].toInt();
    option.margins = obj["margins"].toInt();
    option.titlebarHeight = obj["titlebarHeight"].toInt();
}

static void operator<<(Highlighter::ButtonOption &option, const QJsonObject &obj)
{
    option.color = obj["color"].toString();
    option.width = obj["width"].toInt();
}

static void operator<<(Highlighter::FrameButtonsOptions &option, const QJsonObject &obj)
{
    option.spacing = obj["spacing"].toInt();
    option.margins = obj["margins"].toInt();
    option.direction = (obj["direction"].toString().toLower() == "rtl"? Qt::RightToLeft : Qt::LeftToRight);

    auto buttons = obj["buttons"].toArray();
    for (const auto &b: buttons)
    {
        Highlighter::ButtonOption btn;
        btn << b.toObject();
        option.buttons << btn;
    }
}

static void operator<<(Highlighter::EditorOptions &option, const QJsonObject &obj)
{
    option.margins = obj["margins"].toInt();
}

static Highlighter::Options &operator<<(Highlighter::Options &option, const QJsonDocument &json)
{
    auto obj = json.object();
    option.background << json["background"].toObject();
    option.buttons << json["buttons"].toObject();
    option.editor << json["editor"].toObject();

    return option;
}

#endif // HIGHLIGHTER_H
