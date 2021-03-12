#include "editordialog.h"

#include <QTimer>
#include <QMdiSubWindow>
#include <QPainter>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QDesktopServices>

EditorDialog::EditorDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::EditorDialog)
{
    ui->setupUi(this);

    mHighlighter = new Highlighter(this);
    mHighlighter->setPlainText("print(\"Hello World :)\");");

    auto background = new Background;
    background->setColor("#888888");

    auto backgroundLayout = new QHBoxLayout(background);
    backgroundLayout->addWidget(mHighlighter);

    auto win = ui->mdiArea->addSubWindow(background);
    win->setGeometry(30, 30, 600, 300);
    win->setBackgroundRole(QPalette::Dark);
    win->setAutoFillBackground(true);

    ui->themeCombo->addItems(Highlighter::themes());
    ui->frameCombo->addItems(Highlighter::frames().keys());
    ui->languageCombo->addItems(Highlighter::definitions());
    ui->languageCombo->setCurrentText("C++");
    ui->fontCombo->setCurrentText("monospace");
    ui->fontSizeSpin->setValue(16);
    ui->scaleSpin->setValue(2);

    connect(ui->themeCombo, &QComboBox::currentTextChanged, this, &EditorDialog::switchTheme);
    connect(ui->frameCombo, &QComboBox::currentTextChanged, this, &EditorDialog::switchFrame);
    connect(ui->languageCombo, &QComboBox::currentTextChanged, this, &EditorDialog::switchDefinitions);
    connect(ui->fontCombo, &QComboBox::currentTextChanged, this, &EditorDialog::switchFont);
    connect(ui->fontSizeSpin, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &EditorDialog::switchFont);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &EditorDialog::save);

    switchFrame();
    switchTheme();
    switchDefinitions();
    switchFont();
}

EditorDialog::~EditorDialog()
{
    delete ui;
}

void EditorDialog::accept()
{
}

void EditorDialog::save()
{
    auto path = QFileDialog::getSaveFileName(this, tr("Save"), QDir::homePath(), "PNG file (*.png");
    if (path.isEmpty())
        return;

    mHighlighter->setReadOnly(false);
    mHighlighter->save(path, mHighlighter->width() * ui->scaleSpin->value());
    mHighlighter->setReadOnly(true);

    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void EditorDialog::switchTheme()
{
    auto theme = ui->themeCombo->currentText();
    if (theme.isEmpty())
        return;

    mHighlighter->setTheme(theme);
    mHighlighter->repaint();
}

void EditorDialog::switchFrame()
{
    auto frame = ui->frameCombo->currentText();
    if (frame.isEmpty())
        return;

    mHighlighter->setFrame(frame);
    mHighlighter->repaint();
}

void EditorDialog::switchFont()
{
    mHighlighter->setFontOptions({
        .family = ui->fontCombo->currentText(),
        .size = ui->fontSizeSpin->value(),
    });
    mHighlighter->repaint();
}

void EditorDialog::switchDefinitions()
{
    auto defs = Highlighter::definitions();
    auto def = ui->languageCombo->currentText();
    if (!defs.contains(def))
        return;

    mHighlighter->setDefinitions(def);
    mHighlighter->repaint();
}


void EditorDialog::Background::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.fillRect(e->rect(), mColor);
}

void EditorDialog::Background::setColor(const QColor &color)
{
    mColor = color;
    repaint();
}
