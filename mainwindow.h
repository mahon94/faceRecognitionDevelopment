#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "pcaknn.h"
#include <QMainWindow>
#include <QTimer>

#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <random>
#include <time.h>
#include <math.h>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <list>
#include <algorithm>

using namespace cv;
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void processFrameAndUpdateGUI();                // function prototype

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_addImgBtn_clicked();

    void on_trainBtn_clicked();

    void on_recBtn_clicked();


private:
    Ui::MainWindow *ui;
    VideoCapture capWebcam;
    //CvCapture* capture;
    Mat frame;
    Mat faceROI;
    CascadeClassifier facecascade;
    CascadeClassifier eyecascade;
    String facename="haarcascade_frontalface_alt.xml";
    String eyename= "haarcascade_eye.xml";
    //String dataset= "E:/university/faceRecognition/codes/faceRecognitionQt0/attrain.txt";
    String testdata = "train.txt";
    //wofstream fout;//("train.txt");

    int im_width = 92;
    int im_height = 112;

    vector<Mat> images;
    vector<int> labels;
    vector<Face> facesvector;
    map<std::string,int> mymap;
    Ptr<FaceRecognizer> model;
    PCAKNN myknn;
    QTimer* qtimer;                 // timer for processFrameAndUpdateGUI()
    String recogtype;

    // function prototypes -----------------------
    void MainWindow::exitProgram();                    // function prototype
    QImage MainWindow::matToQImage(cv::Mat mat);       // function prototype
    wstring MainWindow::s2ws(const std::string& s);
    double MainWindow::distance(Point p,Point q);
    Mat MainWindow::cropface(Mat image, Point eyeleft, Point eyeright,pair<float,float> offset_pct, pair<int,int> dest_size );
    void MainWindow::read_csv(string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';');
    void MainWindow::read_knn_csv(string& filename, vector<Face>& facesvector, char separator = ';' );
    void MainWindow::train(String recType);
    bool MainWindow::ListFiles(wstring path, wstring mask, vector<wstring>& files);
    void MainWindow::fillcsv();
    string MainWindow::ExePath();

    //---------------------------


};

#endif // MAINWINDOW_H
