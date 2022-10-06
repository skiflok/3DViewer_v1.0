#ifndef GLVIEWER_H
#define GLVIEWER_H

#define GL_SILENCE_DEPRECATION

#include <QtWidgets/QWidget>
#include <QtOpenGL>
#include <QtOpenGLWidgets>
#include <QFileDialog>
#include <QColorDialog>

#include<iostream>

#include <QTimer>

extern "C" {
#include "../backend/parser.h"
}

QT_BEGIN_NAMESPACE
namespace Ui { class GLviewer; }
QT_END_NAMESPACE

class GLviewer : public QOpenGLWidget
{
    Q_OBJECT

public:
    GLviewer(QWidget *parent = nullptr);
    ~GLviewer();


    Vertexes vertex;
    Facets facet;

    double scale;
    double modelScale;
    Move move;
    Rotate rotate;

    QColor colorWidget;
    QColor colorLine;
    QColor colorPoint;

    int pointSize;
    int pointForm;
    int lineWidth;
    int lineForm;
    int frustum;


    enum {
        EMPTY,
        POINT_ROUND = 1,
        POINT_QUADRO,
        LINE_SOLID = 1,
        LINE_DASHED,
        IS_CHECKED = 1

     };
    void pointSettingForm();
    void lineSettingForm();
    void projectionSelect();




    void change_zoom(double value);

private:
//    Ui::GLviewer *ui;

    float xRot, yRot, zRot;
    QPoint mPos;


    void drawShape();
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;


    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;


};
#endif // GLVIEWER_H
