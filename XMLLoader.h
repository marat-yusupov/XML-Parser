#include <QMainWindow>
#include <QPushButton>
#include <QPainter>
#include <QMap>
#include <QPair>
#include <QList>

class XMLLoader : public QMainWindow {

public:
    XMLLoader(QWidget *parent = 0);

  Q_OBJECT

private slots:
    void loadFilePath();

private:
    QList<QPair<double, double>> pointsList;
    QPushButton *loadXMLButton;
    void parseXMLFile(QString pathFile);
    bool mouseZoom = false;
    bool mouseZoomingActive = false;
    double defaultWindowWidth;
    double defaultWindowHeight;

protected:
    void paintEvent(QPaintEvent*) override;
    void wheelEvent(QWheelEvent *event) override;
};
