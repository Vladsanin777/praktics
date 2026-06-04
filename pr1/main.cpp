#include <QWidget>
#include <QTranslator>
#include <QString>
#include <QTextStream>
#include <QApplication>
#include <QTextEdit>
#include <QToolBar>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QColorDialog>
#include <QComboBox>
#include <QFontComboBox>
#include <QFile>
#include <QtEasy/TitlesBars/QTitleBarTempInfo.hpp>


using QtEasy::TitlesBars::QTitleBarTempInfo;

class TextEditor : public QWidget {
    Q_OBJECT

private:
    QVBoxLayout * m_layout{nullptr};
    QTitleBarTempInfo * m_titleBar{nullptr};
    QPushButton * m_open{nullptr};
    QPushButton * m_save{nullptr};
    QToolBar * m_toolBar{nullptr};
    QComboBox * m_sizeBox{nullptr};
    QFontComboBox * m_fontBox{nullptr};
    QTextEdit * m_text{nullptr};

public:
    TextEditor(QWidget * parent = nullptr) : QWidget{parent} {
        m_open = new QPushButton{"🗁", this};
        m_open->setObjectName("open");
        m_open->setFixedSize(30, 30);
        m_save = new QPushButton{"🖫", this};
        m_save->setFixedSize(30, 30);

        connect(m_open, &QPushButton::clicked, this, &TextEditor::openFile);
        connect(m_save, &QPushButton::clicked, this, &TextEditor::saveFile);

        m_titleBar = new QTitleBarTempInfo{tr("Text editor"), this};
        m_titleBar->addWidget(m_save);
        m_titleBar->addWidget(m_open);

        m_toolBar = new QToolBar(tr("Format panel"), this);

        m_toolBar->setMovable(false); 
        m_toolBar->setFixedHeight(40);
        m_toolBar->setMovable(false);
        m_toolBar->setContextMenuPolicy(Qt::PreventContextMenu);
        m_toolBar->setAllowedAreas(Qt::TopToolBarArea);
        m_toolBar->setContentsMargins(5, 2, 5, 2);
        m_toolBar->setMinimumWidth(500);

        if (m_toolBar->layout()) {
            m_toolBar->layout()->setSpacing(5);
        }

        m_text = new QTextEdit{this};
        m_text->setTabChangesFocus(true);

        m_layout = new QVBoxLayout{this};
        m_layout->addWidget(m_titleBar);
        m_layout->addWidget(m_toolBar);
        m_layout->addWidget(m_text);

        QAction *boldAction = m_toolBar->addAction("B");
        boldAction->setFont(QFont("Arial", 10, QFont::Bold));
        connect(boldAction, &QAction::triggered, this, &TextEditor::toggleBold);

        QWidget *boldButton = m_toolBar->widgetForAction(boldAction);
        if (boldButton) {
            boldButton->setFixedSize(30, 30);
        }

        QFont italicFont("Arial", 10);
        italicFont.setItalic(true);
        QAction *italicAction = m_toolBar->addAction("I");
        italicAction->setFont(italicFont);
        connect(italicAction, &QAction::triggered, this, &TextEditor::toggleItalic);

        QWidget *italicButton = m_toolBar->widgetForAction(italicAction);
        if (italicButton) {
            italicButton->setFixedSize(30, 30);
        }

        QAction *colorAction = m_toolBar->addAction(tr("Color"));
        connect(colorAction, &QAction::triggered, this, &TextEditor::changeColor);

        QWidget *colorButton = m_toolBar->widgetForAction(colorAction);
        if (colorButton) {
            colorButton->setFixedSize(60, 30);
        }

        m_fontBox = new QFontComboBox(this);
        m_fontBox->setCurrentFont(QFont("Arial"));
        connect(m_fontBox, &QFontComboBox::currentFontChanged, this, &TextEditor::changeFontFamily);
        m_toolBar->addWidget(m_fontBox);
        m_fontBox->setFixedHeight(30);

        m_sizeBox = new QComboBox(this);
        m_sizeBox->addItems({"8", "10", "12", "14", "16", "18", "24", "36", "48", "72"});
        m_sizeBox->setCurrentText("12");
        connect(m_sizeBox, &QComboBox::currentTextChanged, this, &TextEditor::changeFontSize);
        m_toolBar->addWidget(m_sizeBox);
        m_sizeBox->setFixedHeight(30);
    }

private slots:
    void changeFontFamily(const QFont &font) {
        m_text->setFontFamily(font.family());
    }

    void toggleBold() {
        int weight = (m_text->fontWeight() != QFont::Bold) ? QFont::Bold : QFont::Normal;
        m_text->setFontWeight(weight);
    }

    void toggleItalic() {
        m_text->setFontItalic(!m_text->fontItalic());
    }

    void changeFontSize(const QString &sizeStr) {
        m_text->setFontPointSize(sizeStr.toDouble());
    }

    void changeColor() {
        QColor color = QColorDialog::getColor(Qt::black, this, tr("Select text color"));
        if (color.isValid()) {
            m_text->setTextColor(color);
        }
    }

    void openFile() {
        QString filePath = QFileDialog::getOpenFileName(this, tr("Open file"), "", "HTML Files (*.html);;Text Files (*.txt)");
        if (!filePath.isEmpty()) {
            QFile file(filePath);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&file);
                m_text->setHtml(in.readAll());
            }
        }
    }

    void saveFile() {
        QString filePath = QFileDialog::getSaveFileName(this, tr("Save file"), "", "HTML Files (*.html)");
        if (!filePath.isEmpty()) {
            QFile file(filePath);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                out << m_text->toHtml();
            }
        }
    }

protected:
    void paintEvent(QPaintEvent *event) {
        QStyleOption opt = QStyleOption{};
        opt.initFrom(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
};

int main(int argc, char ** argv) {
    QApplication app{argc, argv};

    QFile styleFile("main.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        app.setStyleSheet(styleSheet);
    }

    QTranslator translator;

    QLocale systemLocale = QLocale::system();

    if (translator.load(systemLocale, "TextEditor", "_", ":/i18n")) {
        app.installTranslator(&translator);
    }

    TextEditor window{};
    window.resize(500, 500);
    window.show();

    return app.exec();
}

#include <main.moc>
