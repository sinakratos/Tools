#include "mainwindow.h"
#include "ui_mainwindow.h"

//essential parts of the code
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::makeTemp(int n)
{
    if(n == 1)
    {
        Trect1.x = rect1.x + ratio;
        Trect1.y = rect1.y + ratio;
        Trect2.x = rect2.x - ratio;
        Trect2.y = rect2.y - ratio;
    }
    else if(n == 2)
    {
        Tsqr1.x = sqr1.x + ratio;
        Tsqr1.y = sqr1.y + ratio;
        Tsqr2.x = sqr2.x - ratio;
        Tsqr2.y = sqr2.y - ratio;
    }
    else if (n == 3)
    {
        Tradius = radius;
    }


}


//---| camera button click |---//
void MainWindow::on_pushButton_clicked()
{
    cap.open(0);    //---| open camera's first frame
    w = ui->label->width();
    h = ui->label->height();
    ui->label_2->resize(w,h);
    if(checked == 1)    //---| if it is already clicked close the camera
    {
        closeWebcam();
        checked *= -1;
    }
    if(cap.isOpened())  //---| if cap is opened open camera
    {
        openWebcam();
        checked *= -1;
    }
}

//---| this function gets repeated every frame |---//
void MainWindow::updateWindow()
{
    cap >> frame;   //---| put cap into frame
    cvtColor(frame, frame, COLOR_BGR2RGB);  //---| switch the clore mode from BRG to RGB
    on_rectangle_stateChanged();    //---| draw rectangle
    on_square_stateChanged();   //---| draw square
    on_circle_stateChanged();   //---| draw circle

    //---| set the frame into the selected label in ui |---//
    qt_image = QImage((const unsigned char*) (frame.data), frame.cols, frame.rows, QImage::Format_RGB888);
    //---| this line set the lable size based on the frames width and hight
    ui->label->setPixmap(QPixmap::fromImage(qt_image).scaled(w,h, KeepAspectRatio));
    ui->label->resize(ui->label->pixmap()->size());
}



//---| closes camera |---//
void MainWindow::closeWebcam()
{
    disconnect(timer, SIGNAL(timeout()), this, SLOT(updateWindow()));
    cap.release();
    ui->label->hide();
    ui->label_2->show();

}

//---| opens camera |---//
void MainWindow::openWebcam()
{
    ui->label_2->hide();
    ui->label->show();
    connect(timer, SIGNAL(timeout()), this, SLOT(updateWindow()));
    timer->start(1);
}

//---| a function to set the default ratios for slider |---//
void MainWindow::reset(int n)
{
    if(n == 1)
    {
        rect1.x = frame.cols / 2 - 30 - ratio;
        rect1.y = frame.rows / 3 - ratio;
        rect2.x = frame.cols / 2 + 30 + ratio;
        rect2.y = frame.rows * 2 / 3 + ratio;
    }

    if(n == 2)
    {
        sqr1.x = frame.cols / 2 - 40 - ratio;
        sqr1.y = frame.rows / 2 - 40 - ratio;
        sqr2.x = frame.cols / 2 + 40 + ratio;
        sqr2.y = frame.rows / 2 + 40 + ratio;
    }

    if(n == 3)
    {
        center.x = frame.cols / 2;
        center.y = frame.rows / 2;
        radius = 50 + ratio;
    }

}

//---| rectangle box checked or not |---//
void MainWindow::on_rectangle_stateChanged()
{

    if(ui->rectangle->isChecked())  //---| if checked
    {
        if(startRect)   //---| if the shape is on the default spot
        {
            reset(1);   //---| save the default ratios for slider
        }
        rectangle(frame, rect1, rect2, Scalar(200,0,0), 2); //---| draw the shape
    }
}

//---| square box checked or not |---//
void MainWindow::on_square_stateChanged()
{

    if(ui->square->isChecked()) //---| if checked
    {
        if(startSqr == true)   //---| if the shape is on the default spot
        {
            reset(2);   //---| save the default ratios for slider
        }
        rectangle(frame, sqr1, sqr2, Scalar(0,0,200), 2);   //---| draw the shape
    }
}

//---| circle box checked or not |---//
void MainWindow::on_circle_stateChanged()
{

    if(ui->circle->isChecked()) //---| if checked
    {
        if(startCircle == true) //---| if the shape is on the default spot
        {
            reset(3);   //---| save the default ratios for slider
        }
        circle(frame, center, radius, Scalar(50,200,70), 2);    //---| draw the shape
    }
}

//---| up button |---//
void MainWindow::on_up_clicked()
{
    if(ui->rectangle->isChecked())
    {
        rect1.y -= 50;
        rect2.y -= 50;

        //---| saves the changed values |---//
        makeTemp(1);

        startRect = false;
    }

    if(ui->square->isChecked())
    {
        sqr1.y -= 50;
        sqr2.y -= 50;

        //---| saves the changed values |---//
        makeTemp(2);

        startSqr = false;
    }

    if(ui->circle->isChecked())
    {
        center.y -= 50;

        //---| saves the last value |---//
        makeTemp(3);

        startCircle = false;
    }

}

//---| left button |---//
void MainWindow::on_left_clicked()
{
    if(ui->rectangle->isChecked())
    {
        rect1.x -= 50;
        rect2.x -= 50;

        //---| saves the changed values |---//
        makeTemp(1);

        startRect = false;
    }

    if(ui->square->isChecked())
    {
        sqr1.x -= 50;
        sqr2.x -= 50;

        //---| saves the changed values |---//
        makeTemp(2);

        startSqr = false;
    }

    if(ui->circle->isChecked())
    {
        center.x -= 50;

        //---| saves the last value |---//
        makeTemp(3);

        startCircle = false;
    }
}

//---| right button |---//
void MainWindow::on_right_clicked()
{
    if(ui->rectangle->isChecked())
    {
        rect1.x += 50;
        rect2.x += 50;

        //---| saves the changed values |---//
        makeTemp(1);
        startRect = false;
    }

    if(ui->square->isChecked())
    {
        sqr1.x += 50;
        sqr2.x += 50;

        //---| saves the changed values |---//
        makeTemp(2);

        startSqr = false;
    }

    if(ui->circle->isChecked())
    {
        center.x += 50;

        //---| saves the last value |---//
        makeTemp(3);

        startCircle = false;
    }
}

//---| down button |---//
void MainWindow::on_down_clicked()
{
    if(ui->rectangle->isChecked())
    {
        rect1.y += 50;
        rect2.y += 50;

        //---| saves the changed values |---//
        makeTemp(1);

        startRect = false;
    }

    if(ui->square->isChecked())
    {
        sqr1.y += 50;
        sqr2.y += 50;

        //---| saves the changed values |---//
        makeTemp(2);
        startSqr = false;
    }

    if(ui->circle->isChecked())
    {
        center.y += 50;

        //---| saves the last value |---//
        makeTemp(3);

        startCircle = false;
    }
}

//---| reset button |---//
void MainWindow::on_reset_clicked()
{
    if(ui->rectangle->isChecked())
    {
        reset(1);
        makeTemp(1);
    }

    if(ui->square->isChecked())
    {
        reset(2);
        makeTemp(2);
    }

    if(ui->circle->isChecked())
    {
        reset(3);
        makeTemp(1);
    }
}

//---| slider value changed |---//
void MainWindow::on_size_valueChanged(int value)
{

    ratio = ui->size->value();  //---| save the slider value into ratio

    //---| if rectangle box is checked and it's not in the dafault position |---//
    if(ui->rectangle->isChecked() && startRect == false)
    {
        rect1.x = Trect1.x - ratio;
        rect1.y = Trect1.y - ratio;
        rect2.x = Trect2.x + ratio;
        rect2.y = Trect2.y + ratio;

    }

    //---| if square box is checked and it's not in the dafault position |---//
    if(ui->square->isChecked() && startSqr == false)
    {
        sqr1.x = Tsqr1.x - ratio;
        sqr1.y = Tsqr1.y - ratio;
        sqr2.x = Tsqr2.x + ratio;
        sqr2.y = Tsqr2.y + ratio;
    }

    //---| if circle box is checked and it's not in the dafault position |---//
    if(ui->circle->isChecked() && startCircle == false)
    {
        radius = Tradius + ratio;
    }
}
