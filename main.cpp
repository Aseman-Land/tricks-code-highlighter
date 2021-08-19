#include "highlighter.h"
#include "editordialog.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QDateTime>
#include <QDebug>

#include <iostream>

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

        QApplication app(argc, argv);
        app.setApplicationName("tricks-code-highlighter");
        app.setApplicationVersion("1.0");

        EditorDialog w;
        w.show();

        return app.exec();
    }
    else
    {
//        qputenv("QT_QPA_PLATFORM", "offscreen");

        QApplication app(argc, argv);
        app.setApplicationName("tricks-code-highlighter");
        app.setApplicationVersion("1.0");

        QCommandLineParser parser;
        parser.setApplicationDescription("Code highlighter to create tricks images.");
        parser.addHelpOption();
        parser.addVersionOption();

        QCommandLineOption showProgressOption("p", QStringLiteral("Show progress during copy"));
        parser.addOption(showProgressOption);

        QCommandLineOption inputOption(QStringList() << "i" << "input", QStringLiteral("Input source code file. (Required)"), "file");
        parser.addOption(inputOption);

        QCommandLineOption destOption(QStringList() << "o" << "output", QStringLiteral("Output image file path. (Required)"), "file");
        parser.addOption(destOption);

        QCommandLineOption widthOption(QStringList() << "w" << "width", QStringLiteral("Width of output image"), "pixels");
        parser.addOption(widthOption);

        QCommandLineOption minWidthOption(QStringList() << "minimum-width", QStringLiteral("Minimum width of frame"), "pixels");
        parser.addOption(minWidthOption);

        QCommandLineOption maxWidthOption(QStringList() << "maximum-width", QStringLiteral("Maximum width of frame"), "pixels");
        parser.addOption(maxWidthOption);

        QCommandLineOption fontFanmilyOption(QStringList() << "font-family", QStringLiteral("Editor font family name."), "name", "Ubuntu Mono");
        parser.addOption(fontFanmilyOption);

        QCommandLineOption fontSizeOption(QStringList() << "font-size", QStringLiteral("Editor font size in pixels."), "pixels", "20");
        parser.addOption(fontSizeOption);

        QCommandLineOption themeOption(QStringList() << "theme", QStringLiteral("Select highlighter theme"), "theme", "ayu Dark");
        parser.addOption(themeOption);

        QCommandLineOption frameOption(QStringList() << "frame", QStringLiteral("Select frame theme"), "theme", "Dark");
        parser.addOption(frameOption);

        QCommandLineOption lineNumbersOption(QStringList() << "line-numbers", QStringLiteral("Visible line numbers"));
        parser.addOption(lineNumbersOption);

        QCommandLineOption themesOption(QStringList() << "print-themes", QStringLiteral("Print all available themes."));
        parser.addOption(themesOption);

        QCommandLineOption definitionsOption(QStringList() << "print-definitions", QStringLiteral("Print all available definitions."));
        parser.addOption(definitionsOption);

        QCommandLineOption framesOption(QStringList() << "print-frames", QStringLiteral("Print all available frames."));
        parser.addOption(framesOption);

        parser.process(app);

        if (parser.isSet(themesOption))
        {
            std::cout << QString("    - " + Highlighter::themes().join("\n    - ")).toStdString().c_str() << std::endl;
            return 0;
        }

        if (parser.isSet(framesOption))
        {
            std::cout << QString("    - " + Highlighter::frames().keys().join("\n    - ")).toStdString().c_str() << std::endl;
            return 0;
        }

        if (parser.isSet(definitionsOption))
        {
            std::cout << QString("    - " + Highlighter::definitions().join("\n    - ")).toStdString().c_str() << std::endl;
            return 0;
        }

        if (!parser.isSet(inputOption) || !parser.isSet(destOption))
        {
            parser.showHelp();
            return 0;
        }

        auto input = parser.value(inputOption);
        auto dest = parser.value(destOption);
        auto fontFamiliy = parser.value(fontFanmilyOption);
        auto fontSize = parser.value(fontSizeOption).toInt();
        int width = parser.isSet(widthOption)? parser.value(widthOption).toInt() : 1024;
        int min_width = parser.isSet(minWidthOption)? parser.value(minWidthOption).toInt() : 0;
        int max_width = parser.isSet(maxWidthOption)? parser.value(maxWidthOption).toInt() : 1024;
        auto frame = parser.value(frameOption);
        auto theme = parser.value(themeOption);
        auto lineNumbers = parser.isSet(lineNumbersOption);

        if (!QFile::exists(input))
        {
            qDebug() << "Can't open input file: No such file or directory.";
            return 0;
        }

        Highlighter w(lineNumbers);
        w.setFrame(frame);
        w.setTheme(theme);
        w.setMinWidth(min_width);
        w.setMaxWidth(max_width);
        w.setFontOptions({
            .family = fontFamiliy,
            .size = fontSize,
        });
        w.setFile(input);
        w.save(dest, width);

        return 0;
    }
}
