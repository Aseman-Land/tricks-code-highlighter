#include "highlighter.h"

#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QFontMetrics>
#include <QImageWriter>
#include <QDir>
#include <QScrollBar>
#include <QTimer>

#include <QDebug>

Highlighter::Highlighter(bool lineNumbers, QWidget *parent)
    : QWidget(parent),
      mEditMode(false)
{
    mEditor = new CodeEditor(lineNumbers);
    mEditor->setFrameShadow(QFrame::Plain);
    mEditor->setFrameShape(QFrame::NoFrame);
    mEditor->setBackgroundRole(QPalette::NoRole);
    mEditor->setAutoFillBackground(false);
    mEditor->viewport()->setAutoFillBackground(false);
    mEditor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mEditor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mEditor->setReadOnly(true);

    mHighlighter = new KSyntaxHighlighting::SyntaxHighlighter(mEditor->document());

    mLayout = new QVBoxLayout(this);
    mLayout->addWidget(mEditor);

    resize(800, 600);
}

Highlighter::Highlighter(QWidget *parent)
    : Highlighter(true, parent)
{
    mEditMode = true;

    mEditor->setReadOnly(false);

    setFrame("Auto");
    setTheme("Breeze Dark");
    setFontOptions({
        .family = "Ubuntu Mono",
        .size = 16,
    });
}

Highlighter::~Highlighter()
{
    delete mHighlighter;
}

Highlighter::Options Highlighter::frameOptions() const
{
    return mFrameOptions;
}

void Highlighter::setFrameOptions(const Options &frameOptions)
{
    mFrameOptions = frameOptions;

    auto &frame = mFrameOptions.background;
    auto &editor = mFrameOptions.editor;

    mLayout->setContentsMargins(frame.margins + editor.margins,
                                frame.margins + frame.titlebarHeight,
                                frame.margins + editor.margins,
                                frame.margins + editor.margins);
}

const QString &Highlighter::language() const
{
    return mLanguage;
}

void Highlighter::setLanguage(const QString &newLanguage)
{
    mLanguage = newLanguage;

    for (const auto &d: mRepo.definitions())
        if (d.name().toLower() == newLanguage.toLower())
        {
            qDebug() << d.name() << d.extensions();
            mHighlighter->setDefinition(d);
            mHighlighter->rehighlight();
            return;
        }

    auto d = mRepo.definitionForName(newLanguage);
    if (!d.isValid())
        d = mRepo.definitionForMimeType(newLanguage);
    if (!d.isValid())
        d = mRepo.definitionForFileName(newLanguage);


    mHighlighter->setDefinition(d);
    mHighlighter->rehighlight();
}

qint32 Highlighter::maxWidth() const
{
    return mMaxWidth;
}

void Highlighter::setMaxWidth(const qint32 &maxWidth)
{
    mMaxWidth = maxWidth;
}

qint32 Highlighter::minWidth() const
{
    return mMinWidth;
}

void Highlighter::setMinWidth(const qint32 &minWidth)
{
    mMinWidth = minWidth;
}

void Highlighter::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setClipRect(e->rect());
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(e->rect(), QColor(0,0,0,0));

    paintBackground(painter);
    paintFrameButtons(painter);
}

void Highlighter::paintBackground(QPainter &painter)
{
    auto rect = QWidget::rect();
    rect -= QMargins(mFrameOptions.background.margins, mFrameOptions.background.margins, mFrameOptions.background.margins, mFrameOptions.background.margins);

    QPainterPath path;
    path.addRoundedRect(rect, mFrameOptions.background.round, mFrameOptions.background.round);

    const auto bg = mFrameOptions.background.background;
    const auto t = mHighlighter->theme();
    if (bg == QStringLiteral("auto"))
        painter.fillPath(path, QColor::fromRgb(t.editorColor(KSyntaxHighlighting::Theme::BackgroundColor)));
    else
        painter.fillPath(path, QColor(bg));
}

void Highlighter::paintFrameButtons(QPainter &painter)
{
    auto &frame = mFrameOptions.background;
    auto &buttons = mFrameOptions.buttons;

    const int width = QWidget::width();
    const bool ltr = (buttons.direction != Qt::RightToLeft);
    int left = frame.margins + buttons.margins;
    const auto count = buttons.buttons.count();
    for (int i=0; i<count; i++)
    {
        const auto &btn = buttons.buttons[i];
        QRect rect(ltr? left : width - left - btn.width,
                   frame.margins + frame.titlebarHeight/2 - btn.width/2,
                   btn.width, btn.width);

        QPainterPath path;
        path.addRoundedRect(rect, btn.width/2, btn.width/2);

        painter.fillPath(path, btn.color);
        left += btn.width + buttons.spacing;
    }
}

QString Highlighter::frame() const
{
    return mFrame;
}

void Highlighter::setFrame(const QString &frame)
{
    mFrame = frame;

    auto path = frames().value(frame);
    QFile f(path);
    if (!f.open(QFile::ReadOnly))
    {
        qDebug() << path << ": No such file";
        return;
    }

    Highlighter::Options options;
    options << QJsonDocument::fromJson(f.readAll());
    f.close();

    setFrameOptions(options);
}

QMap<QString, QString> Highlighter::frames()
{
    QStringList framesRootDirectories = {
        "/usr/share/" + QCoreApplication::applicationName() + "/frames",
        QCoreApplication::applicationDirPath() + "/frames",
        QCoreApplication::applicationDirPath() + "/share/frames",
        QCoreApplication::applicationDirPath() + "/../share/frames",
        QDir::homePath() + "/.local/share/" + QCoreApplication::applicationName() + "/frames",
    };

    QMap<QString, QString> res;
    for (const auto &f: framesRootDirectories)
    {
        QStringList list = QDir(f).entryList({"*.json"}, QDir::Files);
        for (const auto &l: list)
            res[l.mid(0, l.length()-5)] = f + "/" + l;
    }

    return res;
}

QString Highlighter::theme() const
{
    return mTheme;
}

void Highlighter::setTheme(const QString &theme)
{
    mTheme = theme;

    mHighlighter->setTheme(mRepo.theme(mTheme));
    mHighlighter->rehighlight();
}

Highlighter::EditorFontOptions Highlighter::fontOptions() const
{
    return mFontOptions;
}

void Highlighter::setFontOptions(const EditorFontOptions &fontOptions)
{
    mFontOptions = fontOptions;

    auto font = this->font();
    font.setFamily(mFontOptions.family);
    font.setFamilies({mFontOptions.family});
    font.setPixelSize(mFontOptions.size);

    setFont(font);
}

QString Highlighter::file() const
{
    return mFile;
}

void Highlighter::setFile(const QString &file)
{
    mFile = file;

    QString code;
    QFile f(mFile);
    if (f.open(QFile::ReadOnly))
    {
        code = QString::fromUtf8(f.readAll());
        f.close();
    }

    if (mLanguage.isEmpty())
        mHighlighter->setDefinition(mRepo.definitionForFileName(mFile));

    setCode(code);
}

QStringList Highlighter::themes()
{
    KSyntaxHighlighting::Repository repo;
    QStringList res;
    auto themes = repo.themes();
    for (const auto &t: themes)
        res << t.name();
    return res;
}

QStringList Highlighter::definitions()
{
    KSyntaxHighlighting::Repository repo;
    QStringList res;
    auto definitions = repo.definitions();
    for (const auto &d: definitions)
        res << d.name();

    res.sort();
    return res;
}

void Highlighter::setDefinitions(const QString &definition)
{
    auto definitions = mRepo.definitions();
    for (const auto &d: definitions)
        if (d.name().toLower() == definition.toLower())
        {
            mHighlighter->setDefinition(d);
            break;
        }
}

void Highlighter::setReadOnly(bool readOnly)
{
    mEditor->setReadOnly(readOnly);
}

QString Highlighter::code() const
{
    return mCode;
}

void Highlighter::setCode(const QString &code)
{
    mCode = code;
    mEditor->setPlainText(mCode);

    QFontMetrics metric(font());

    int maxWidth = 100;
    auto lines = code.split("\n");
    for (const auto &l: lines)
    {
        auto rect = metric.boundingRect(l);
        maxWidth = qMax(maxWidth, rect.width());
    }

    int width = mFrameOptions.background.margins * 2 + mFrameOptions.editor.margins * 2 + mEditor->getViewportMargins().left() +
                mEditor->getViewportMargins().right() + 20 + maxWidth;

    resize(qMax(mMinWidth, qMin(width, mMaxWidth)), 200);

    auto doc = mEditor->document();
    auto height = mEditor->getBlockHeight(doc->lastBlock());
    height += mFrameOptions.background.margins * 2 + mFrameOptions.editor.margins + mFrameOptions.background.titlebarHeight +
              mEditor->getViewportMargins().top() + mEditor->getViewportMargins().bottom();

    resize(Highlighter::width(), height);

    // Render to fix wrap bugz
    QImage img(10, 10, QImage::Format_ARGB32);
    img.fill(QColor(0,0,0,0));

    render(&img, QPoint(), QRegion(rect()), QWidget::DrawChildren);

    height = mEditor->getBlockHeight(doc->lastBlock());
    height += mFrameOptions.background.margins * 2 + mFrameOptions.editor.margins + mFrameOptions.background.titlebarHeight +
              mEditor->getViewportMargins().top() + mEditor->getViewportMargins().bottom();

    resize(Highlighter::width(), height);
    // End
}

void Highlighter::setPlainText(const QString &text)
{
    mEditor->setPlainText(text);
}

void Highlighter::save(const QString &path, int w)
{
    qreal ratio = (qreal)w / width();

    QImage img(w, height()*ratio, QImage::Format_ARGB32);
    img.fill(QColor(0,0,0,0));
    img.setDevicePixelRatio(ratio);

    render(&img, QPoint(), QRegion(rect()), QWidget::DrawChildren);

    QImageWriter writer(path);
    writer.write(img);
}

