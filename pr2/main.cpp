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


using QtEasy::TitlesBars::QTitleBar;

#include <QWidget>
#include <QImage>
#include <QPoint>

class PaintArea : public QWidget {
    Q_OBJECT

public:
    PaintArea(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawLineTo(const QPoint &endPoint);

    bool drawing = false;
    int penWidth = 3;
    QColor penColor = Qt::blue;
    QImage image;
    QPoint lastPoint;
};

#include <QPainter>
#include <QMouseEvent>

PaintArea::PaintArea(QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_StaticContents);
}

void PaintArea::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawImage(0, 0, image);
}

void PaintArea::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        drawing = true;
    }
}

void PaintArea::mouseMoveEvent(QMouseEvent *event) {
    if ((event->buttons() & Qt::LeftButton) && drawing) {
        drawLineTo(event->pos());
    }
}

void PaintArea::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && drawing) {
        drawLineTo(event->pos());
        drawing = false;
    }
}

void PaintArea::drawLineTo(const QPoint &endPoint) {
    QPainter painter(&image);
    painter.setPen(QPen(penColor, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    
    lastPoint = endPoint;
    update();
}

void PaintArea::resizeEvent(QResizeEvent *event) {
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        
        QImage newImage(newWidth, newHeight, QImage::Format_RGB32);
        newImage.fill(Qt::white);
        
        QPainter painter(&newImage);
        painter.drawImage(0, 0, image);
        image = newImage;
    }
    QWidget::resizeEvent(event);
}

class PaintEditor : public QWidget {
    Q_OBJECT

private:
    QVBoxLayout * m_layout{nullptr};
    QTitleBar * m_title{nullptr};
    PaintArea * m_paint{nullptr};

public:
    PaintEditor(QWidget *parent = nullptr) : QWidget(parent) {
        m_layout = new QVBoxLayout(this);
        m_title = new QTitleBar{tr("Paint Editor"), this};
        m_paint = new PaintArea{this};

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
