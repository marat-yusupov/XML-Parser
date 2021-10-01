#include <QLabel>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QDebug>
#include <QXmlStreamReader>
#include <QPainter>
#include <QStyle>
#include <QIcon>
#include <QWheelEvent>
#include "XMLLoader.h"

XMLLoader::XMLLoader(QWidget *parent) :QMainWindow(parent) {
    QFrame *frame = new QFrame(this);
      setCentralWidget(frame);

      defaultWindowWidth = window()->width();
      defaultWindowHeight = window()->height();

      QHBoxLayout *hbox = new QHBoxLayout(frame);

      loadXMLButton = new QPushButton("Load XML file", frame);
      hbox->addWidget(loadXMLButton, 0, Qt::AlignLeft | Qt::AlignTop);

      connect(loadXMLButton, &QPushButton::clicked, this, &XMLLoader::loadFilePath);
}

void XMLLoader::loadFilePath() {
    QString filePath = QFileDialog::getOpenFileName();
    parseXMLFile(filePath);
}

void XMLLoader::parseXMLFile(QString filePath)
{
    QFile* file = new QFile(filePath);
    bool res = file->open(QIODevice::ReadOnly | QIODevice::Text);

    if (!res) {
        return;
    }

    QXmlStreamReader xml(file);

    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartDocument) {
            continue;
        }
        if (token == QXmlStreamReader::StartElement) {
            if (xml.name() == "Plot") {
               xml.readNext();
               while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "Plot")) {
                   if (xml.tokenType() == QXmlStreamReader::StartElement && xml.name() == "Point") {
                       QXmlStreamAttributes attributes = xml.attributes();
                       if (attributes.hasAttribute("X") && attributes.hasAttribute("Y")) {
                           QPair<double, double> coordPair;
                           coordPair.first = attributes.value("X").toDouble();
                           coordPair.second = attributes.value("Y").toDouble();
                           pointsList.append(coordPair);
                       }
                   }
                   xml.readNext();
               }
            }
        }
    }
    update();
}

void XMLLoader::paintEvent(QPaintEvent *)
{
    QPainter painter;
    painter.begin(this);
    painter.setPen(QPen(QBrush(QColor(Qt::white)), 1 ));
    painter.translate(window()->width()/2, window()->height()/2);

    if (mouseZoomingActive) {
        if (mouseZoom) {
            defaultWindowWidth++;
            defaultWindowHeight++;
        } else {
            defaultWindowWidth--;
            defaultWindowHeight--;
        }
        mouseZoomingActive = false;
        painter.scale(defaultWindowWidth*0.016, defaultWindowHeight*0.016);
    } else painter.scale(window()->width()*0.016, window()->height()*0.016);

    for (auto item : pointsList) {
        painter.drawPoint(item.first, item.second);
    }

    painter.end();
}

void XMLLoader::wheelEvent(QWheelEvent *event)
{
    mouseZoomingActive = true;
    if (event->delta() > 0) {
        mouseZoom = true;
        update();
    } else if (event->delta() < 0){
        mouseZoom = false;
        update();
    }
}


