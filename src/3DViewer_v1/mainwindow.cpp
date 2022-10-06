#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //____GIF
    timer_for_gif = new QTimer(0); // parent must be null
    connect(timer_for_gif, SIGNAL(timeout()), this, SLOT(create_screen()));
    //_____

    //set value in file
    this->m_sSettingsFile = QApplication::applicationDirPath() + "/settings.ini";
    qDebug() << m_sSettingsFile;
//    load_settings();

}

MainWindow::~MainWindow()
{
//    save_settings();
    delete ui;
    delete timer_for_gif;
}

void MainWindow::free_vertex_and_facet()
{
    if (ui->widget->vertex.arg || ui->widget->facet.arg) {
    free (ui->widget->vertex.arg);
    ui->widget->vertex.count = 0;
    free (ui->widget->facet.arg);
    ui->widget->facet.count = 0;
    }
}

void MainWindow::on_openFile_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, "Выберите файл", ".", tr( " (*.obj)"));

    if (file != ""){
    free_vertex_and_facet();
    ui->statusBar->showMessage(file);
    QByteArray ba = file.toLocal8Bit();
    char *str = ba.data();
    int error = 0;
    error = parser(str , &ui->widget->vertex, &ui->widget->facet);

    if (error) {
        ui->statusBar->showMessage("file not found");
    }

    ui->coun_vertexes->setText(QString::number((ui->widget->vertex.count - 3) / 3));
    ui->count_facets->setText(QString::number(ui->widget->facet.count / 2));
    ui->filename->setText(file.right(file.size()-file.lastIndexOf("/")-1));
    }
}

void MainWindow::wheelEvent(QWheelEvent *event){
   if (event->angleDelta().y() > 0) {

       ui->widget->change_zoom(0.9);
       ui->doubleSpinBox_Scale->setValue(ui->widget->scale);
   } else {

       ui->widget->change_zoom(1.1);
       ui->doubleSpinBox_Scale->setValue(ui->widget->scale);
   }

}


void MainWindow::on_doubleSpinBox_Scale_valueChanged(double valueScale){
    ui->widget->scale = valueScale;

}


void MainWindow::on_update_clicked()
{
     // move
    ui->widget->move.dx = ui->widget->move.dx - (double) ui->dx->value();
    ui->widget->move.dy = ui->widget->move.dy - (double) ui->dy->value();
    ui->widget->move.dz = ui->widget->move.dz - (double) ui->dz->value();

    if (ui->widget->move.dx == 0 ||
        ui->widget->move.dy == 0 ||
        ui->widget->move.dz == 0) {
        moveObj(&ui->widget->vertex, ui->widget->move);
    }

    ui->widget->move.dx = (double) ui->dx->value();
    ui->widget->move.dy = (double) ui->dy->value();
    ui->widget->move.dz = (double) ui->dz->value();

    // rotate
    ui->widget->rotate.dx = ui->widget->rotate.dx - (double)ui->rdx->value();
    ui->widget->rotate.dy = ui->widget->rotate.dy - (double)ui->rdy->value();
    ui->widget->rotate.dz = ui->widget->rotate.dz - (double)ui->rdz->value();

    if(ui->widget->rotate.dx != 0 ||
       ui->widget->rotate.dy != 0 ||
       ui->widget->rotate.dz != 0) {
       rotationObj(&ui->widget->vertex, ui->widget->rotate);
    }

    ui->widget->rotate.dx = (double)ui->rdx->value();
    ui->widget->rotate.dy = (double)ui->rdy->value();
    ui->widget->rotate.dz = (double)ui->rdz->value();

    // scale
    if (ui->modelScale->value() <= 0) {
        ui->modelScale->setValue(1);
    }

    ui->widget->modelScale = ui->modelScale->value() / ui->widget->modelScale;

    if(ui->widget->modelScale != 0 &&
       ui->modelScale->value() > 0) {
       scaleObj(&ui->widget->vertex, ui->widget->modelScale);
    }
    ui->widget->modelScale = ui->modelScale->value();

}


void MainWindow::on_spinBox_line_width_valueChanged(int value)
{
    ui->widget->lineWidth = value;
}


void MainWindow::on_spinBox_point_size_valueChanged(int value)
{
     ui->widget->pointSize = value;
//     qDebug() << ui->widget->pointSize;
}


void MainWindow::on_color_clicked()
{
    if (ui->colorPoint->isChecked()) {
        ui->widget->colorPoint = QColorDialog::getColor(Qt::gray);
    } else if (ui->colorLine->isChecked()) {
        ui->widget->colorLine = QColorDialog::getColor(Qt::gray);
    } else if (ui->colorBackground->isChecked()) {
        ui->widget->colorWidget = QColorDialog::getColor(Qt::gray);
    }
}

// ________PHOTO ___GIF
void MainWindow::on_Pthoto_clicked()
{
      QFileDialog file_dialog_photo(this);
      QString f_name_photo =
          file_dialog_photo.getSaveFileName(this,"Save as...", QDir::currentPath(), "BMP (*.bmp);; JPEG (*.jpeg)");
      QFile file(f_name_photo);
      file.open(QIODevice::WriteOnly);
      QRect rect(0, 0, ui->widget->width(), ui->widget->height());
      QPixmap pixmap = ui->widget->grab(rect);
      pixmap.copy(rect);
      pixmap.toImage().save(&file, "jpg");
      QString q_command_line = "open " + f_name_photo;
      QByteArray temp = q_command_line.toLocal8Bit();
      char *command_line = temp.data();
      system(command_line);
}

void MainWindow::on_stop_and_save_GIF_clicked(){

    timer_for_gif->start(100);
    create_screen();

}

void MainWindow::save_gif() {

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save screenshot"), "", tr("GIF screenshot (*.gif);;GIF screenshot (*.gif)"));
    QGifImage gif(QSize(640, 480));

    gif.setDefaultTransparentColor(Qt::black);
    gif.setDefaultDelay(100);

    for (QVector<QImage>::Iterator img = mas_image.begin(); img != mas_image.end(); ++img) {
        gif.addFrame(*img);

    }

    gif.save(fileName);
    mas_image.clear();
    ui->label_Timer_GIF->clear();
    time = 0.0;

}



void MainWindow::create_screen()
{
    if ( time <= 5.0) {
        mas_image.push_back(ui->widget->grab().toImage());
        time += 0.1;
        ui->label_Timer_GIF->setText(QString::number(time));
    } else if (time >= 5.0) {
        save_gif();
        timer_for_gif->stop();
    }
}

//_________GIF end

void MainWindow::on_optimization_clicked(){
    double xMax = 0;
    double xMin = 0;
    double yMax = 0;
    double yMin = 0;
    double zMax = 0;
    double zMin = 0;
    double maxSize = 0;
    double scale = 100;

    for (unsigned int var = 3; var < ui->widget->vertex.count; var += 3) {

        if(ui->widget->vertex.arg[var] > xMax) xMax = ui->widget->vertex.arg[var];
        if(ui->widget->vertex.arg[var] < xMin) xMin = ui->widget->vertex.arg[var];
        if(ui->widget->vertex.arg[var+1] > yMax) yMax = ui->widget->vertex.arg[var];
        if(ui->widget->vertex.arg[var+1] < yMin) yMin = ui->widget->vertex.arg[var];
        if(ui->widget->vertex.arg[var+2] > zMax) zMax = ui->widget->vertex.arg[var];
        if(ui->widget->vertex.arg[var+2] < zMin) zMin = ui->widget->vertex.arg[var];
    }
    maxSize = xMax - xMin;
    if (maxSize < yMax - yMin) maxSize = yMax - yMin;
    if (maxSize < zMax - zMin) maxSize = zMax - zMin;


    while(maxSize * scale > 1 && scale > 0.0001) {
        scale *= 0.9;
    }

      ui->doubleSpinBox_Scale->setValue(scale);
}


void MainWindow::on_comboBox_point_form_currentIndexChanged(int index)
{
    ui->widget->pointForm = index;
    ui->widget->update();

}


void MainWindow::on_comboBox_line_form_currentIndexChanged(int index)
{
    ui->widget->lineForm = index;
    ui->widget->update();
}


void MainWindow::on_radioButton_frustum_clicked(bool checked)
{
    ui->widget->frustum = checked;

}

void MainWindow::on_radioButton_ortho_clicked()
{
    ui->widget->frustum = ui->widget->EMPTY;
}

void MainWindow::load_settings()
{

    QSettings settings(m_sSettingsFile, QSettings::NativeFormat);
    ui->comboBox_point_form->setCurrentIndex(settings.value("comboBox_point_form").toInt());
    ui->comboBox_line_form->setCurrentIndex(settings.value("comboBox_line_form").toInt());

    ui->spinBox_point_size->setValue(settings.value("spinBox_point_size").toInt());
    ui->spinBox_line_width->setValue(settings.value("spinBox_line_width").toInt());

    ui->colorBackground->setChecked(settings.value("colorBackground").toBool());
    ui->colorLine->setChecked(settings.value("colorLine").toBool());
    ui->colorPoint->setChecked(settings.value("colorPoint").toBool());


    ui->widget->colorLine = settings.value("colorLine_c").value<QColor>();
    ui->widget->colorPoint = settings.value("colorPoint_c").value<QColor>();
    ui->widget->colorWidget = settings.value("colorWidget_c").value<QColor>();

}

void MainWindow::save_settings()
{
    QSettings settings(m_sSettingsFile, QSettings::NativeFormat);
    settings.setValue("comboBox_point_form", ui->comboBox_point_form->currentIndex());
    settings.setValue("spinBox_point_size", ui->spinBox_point_size->value());
    settings.setValue("comboBox_line_form", ui->comboBox_line_form->currentIndex());
    settings.setValue("spinBox_line_width", ui->spinBox_line_width->value());

    settings.setValue("colorBackground", ui->colorBackground->isChecked());
    settings.setValue("colorLine", ui->colorLine->isChecked());
    settings.setValue("colorPoint", ui->colorPoint->isChecked());

    settings.setValue("colorLine_c", ui->widget->colorLine);
    settings.setValue("colorPoint_c", ui->widget->colorPoint);
    settings.setValue("colorWidget_c", ui->widget->colorWidget);

}

void MainWindow::on_save_settings_clicked()
{
    save_settings();
    QMessageBox::information(this, "Сохранение настроек", "Сохранение настроек выполнено успешно");

}

void MainWindow::on_load_setting_clicked()
{
    load_settings();
}

