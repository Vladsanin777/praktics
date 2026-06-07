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
#include <QtEasy/TitlesBars/QTitleBar.hpp>
#include <QtEasy/Widgets/QPaintArea.hpp>


using QtEasy::TitlesBars::QTitleBar;
using QtEasy::Widgets::QPaintArea;

class PaintEditor : public QWidget {
    Q_OBJECT

private:
    QVBoxLayout * m_layout{nullptr};
    QTitleBar * m_title{nullptr};
    QPaintArea * m_paint{nullptr};

public:
    PaintEditor(QWidget *parent = nullptr) : QWidget(parent) {
        m_layout = new QVBoxLayout(this);
        m_title = new QTitleBar{tr("Paint Editor"), this};
        m_paint = new QPaintArea{};

        m_layout->addWidget(m_title);
        m_layout->addWidget(m_paint);
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

    if (translator.load(systemLocale, "PaintEditor", "_", ":/i18n")) {
        app.installTranslator(&translator);
    }

    PaintEditor window{};
    window.resize(500, 500);
    window.show();

    return app.exec();
}

#include <main.moc>
