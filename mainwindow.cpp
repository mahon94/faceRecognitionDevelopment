#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QtCore>

//line 189 problem
//#include <pair>


MainWindow::MainWindow(QWidget *parent) :  QMainWindow(parent),    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    srand (time(NULL));
    ui->comboBox->addItem("Eigenfaces");
    ui->comboBox->addItem("Fisherfaces");
    ui->comboBox->addItem("LBPH");
    ui->comboBox->addItem("KNN");
    //----------------------
    if(!facecascade.load(facename)){
        QMessageBox::critical(this,tr("haarCascade"),tr("Error loading face cascade"));
        exitProgram();
        return;
        }
    if(!eyecascade.load(eyename)){
        QMessageBox::critical(this, tr("haarCascade"), tr("Error loading eye cascade"));
        exitProgram();
        return;
    }

    capWebcam.open(0);
    if(!capWebcam.isOpened()){
        QMessageBox::critical(this, tr("Error"), tr("Error capturing video"));
        exitProgram();
        return;
    }

   //capture=cvCaptureFromCAM(0);



    qtimer = new QTimer(this);                          // instantiate timer
    connect(qtimer, SIGNAL(timeout()), this, SLOT(processFrameAndUpdateGUI()));     // associate timer to processFrameAndUpdateGUI
    qtimer->start(20);

}
///////////////////////////////////////////////////////////////////////////////////////////////////
MainWindow::~MainWindow()
{
    delete ui;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
QImage MainWindow::matToQImage(cv::Mat mat) {
    if(mat.channels() == 1) {                                   // if 1 channel (grayscale or black and white) image
        return QImage((uchar*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);     // return QImage
    } else if(mat.channels() == 3) {                            // if 3 channel color image
        cv::cvtColor(mat, mat, CV_BGR2RGB);                     // flip colors
        return QImage((uchar*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);       // return QImage
    } else {
        qDebug() << "in openCVMatToQImage, image was not 1 channel or 3 channel, should never get here";
    }
    return QImage();        // return a blank QImage if the above did not work
}
///////////////////////////////////////////////////////////////////////////////////////////////////
Mat MainWindow::cropface(Mat image, Point eyeleft, Point eyeright,pair<float,float> offset_pct, pair<int,int> dest_size ){
    double offset_h = floor(float(offset_pct.first)*dest_size.first);
    double offser_v = floor(float(offset_pct.second)* dest_size.second);

    Point eye_direction = (eyeright.x-eyeleft.x, eyeright.y-eyeleft.y);
    double rotation = -atan2(float(eye_direction.x),float(eye_direction.y));
    double dist = distance(eyeleft,eyeright);
    double reference = dest_size.first - 2.0*offset_h;
    double scale = float(dist)/float(reference);

    Mat dst;
    Mat r = getRotationMatrix2D(eyeleft,rotation,1.0);
    warpAffine(image,dst,r,Size(dest_size.first,dest_size.second));
    return dst;
//    crop_xy = (eye_left[0] - scale*offset_h, eye_left[1] - scale*offset_v)

//      crop_size = (dest_sz[0]*scale, dest_sz[1]*scale)
//      image = image.crop((int(crop_xy[0]), int(crop_xy[1]), int(crop_xy[0]+crop_size[0]), int(crop_xy[1]+crop_size[1])))
//      # resize it
//      image = image.resize(dest_sz, Image.ANTIALIAS)
//      return image


//    pair<float,float> crop_xy = make_pair(eyeleft.x - scale*offset_h, eyeleft.y - scale*offser_v);
//    pair<float,float> crop_size = make_pair(dest_size.first * scale, dest_size.second * scale);
//    cv::Rect roi(int(crop_xy.first), int(crop_xy.second), int(crop_xy.first + crop_size.first), int(crop_xy.second + crop_size.second));
//     //cv::Rect roi(0,0,100,100);
//    return dst(roi);

}
///////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::on_addImgBtn_clicked()
{

   //QMessageBox::information(this,tr("haarcascade"),ui->subNameLbl->text());
   //char code[6];
   //code[5]='\0';
   int code=0;
   for (int i=0; i<5; ++i){
       code*=10;
        code+= rand() % 10; //range 0 to 9
   }
   //QString s = QString::number(code);
   //QString Qaddress="E:/university/faceRecognition/codes/faceRecognitionQt0/orl_faces/"+ui->subNameLbl->text();//
   QString Qaddress="faceDataset/"+ui->subNameLbl->text();//
   //QMessageBox::information(this,tr("haarcascade"),Qaddress);
   String address = Qaddress.toLocal8Bit().constData();
   //ui->subNameLbl->setText(Qaddress);


   std::wstring stemp =s2ws(address);

   if (CreateDirectory(stemp.c_str(), NULL) ||
       ERROR_ALREADY_EXISTS == GetLastError())
   {
       String fulladr=address+"/"+to_string(code)+".pgm";
       Mat face_resized;
       cv::resize(faceROI, face_resized, Size(im_width, im_height), 1.0, 1.0, INTER_CUBIC);

       imwrite(fulladr,face_resized);
       QMessageBox::information(this,tr("haarcascade"),"SAVED!");


   }
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::exitProgram() {
    if(qtimer->isActive()) qtimer->stop();          // if timer is running, stop timer
    QApplication::quit();                           // and exit program
}
wstring MainWindow::s2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::processFrameAndUpdateGUI() {
    vector< Rect_<int> > faces;
    std::vector< Rect_<int> > eyes;
    bool blnFrameReadSuccessfully = capWebcam.read(frame);                    // get next frame from the webcam
    if (!blnFrameReadSuccessfully || frame.empty()) {                            // if we did not get a frame
        QMessageBox::information(this, "", "unable to read from webcam \n\n exiting program\n");        // show error via message box
        exitProgram();                                                              // and exit program
        return;                                                                     //
    }
        if(frame.empty()){
            QMessageBox::critical(this, tr("Error"), tr("Error: empty frame!"));
            exitProgram();
            return;
        }
        else{
            Mat original = frame.clone();
            Mat gray;
            cvtColor(original,gray,CV_BGR2GRAY);
            equalizeHist(gray,gray);

            facecascade.detectMultiScale(gray, faces,1.1,2,0| CV_HAAR_SCALE_IMAGE, Size(30, 30));

            for(size_t i=0; i<faces.size(); i++){
               //Point center()
                Point topleft(faces[i].x, faces[i].y);
                Point lowright(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
                rectangle(frame, topleft, lowright, Scalar(255,0,255),1,8,0);
                faceROI = gray(faces[i]);

                eyecascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
                for (size_t j = 0; j < eyes.size(); j++)
                {
                    Point center(faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5);
                    int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
                    circle(frame, center, radius, Scalar(255, 0, 0), 1, 8, 0);
                }
                //****?
                Mat croped;
                if(eyes.size()>=2){
                    Point lefteye(faces[i].x + eyes[0].x + eyes[0].width*0.5, faces[i].y + eyes[0].y + eyes[0].height*0.5);
                    Point righteye(faces[i].x + eyes[1].x + eyes[1].width*0.5, faces[i].y + eyes[1].y + eyes[1].height*0.5);
                    croped = cropface(faceROI,lefteye,righteye,make_pair(0.2,0.2), make_pair(200,200));
                    QImage Qcropped = matToQImage(croped);
 //********check why not work                  // ui->label->setPixmap(QPixmap::fromImage(Qcropped));
                }
                 //****?
            }

            QImage Qframe = matToQImage(frame);
            QImage QfaceROI = matToQImage(faceROI);

            ui->capturelbl->setPixmap((QPixmap::fromImage(Qframe)));
            ui->faceROI->setPixmap((QPixmap::fromImage(QfaceROI)));

        }



}
///////////////////////////////////////////////////////////////////////////////////////////////////
double MainWindow::distance(Point p,Point q){
    double dx= abs(p.x-q.x);
    double dy= abs(p.y-q.y);
    return sqrt(dx*dx+dy*dy);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::on_trainBtn_clicked()
{
    //QString rt=ui->comboBox->currentText();
    //recognizer* rec=new recognizer();
    QString Qrectype = ui->comboBox->currentText();
   QMessageBox::information(this,"title",Qrectype);
   String rectype = Qrectype.toLocal8Bit().constData();

   train(rectype);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::read_csv(string& filename, vector<Mat>& images, vector<int>& labels, char separator ) {
    mymap.clear();
    map<std::string, int>::iterator it;
    int i=0;
    ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    string line, path, classlabel;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        if (!path.empty() && !classlabel.empty()) {
            images.push_back(imread(path, 0));
            std::string mylbl = classlabel.c_str();
            it = mymap.find(mylbl);
            if(it == mymap.end()){
                mymap[mylbl]=i;
                i++;
            }

            labels.push_back(mymap[mylbl]);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::read_knn_csv(string& filename, vector<Face>& facesvector, char separator ) {
    mymap.clear();
    map<std::string, int>::iterator it;
    ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    string line, path, classlabel;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        if (!path.empty() && !classlabel.empty()) {
            Face f; f.face=imread(path, 0);

            std::string mylbl = classlabel.c_str();

            f.name=mylbl;
            facesvector.push_back(f);
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::train(String recType){
    fillcsv();

    try {
            if(recType =="KNN"){
                read_knn_csv(testdata,facesvector);
            }
            else{
                read_csv(testdata, images, labels);
            }
            QMessageBox::information(this,"train","read csv file successfully");
        }
        catch (cv::Exception& e) {
            QMessageBox::critical(this,"dataset Error", "Error opening csv file!");
            exitProgram();
            return;
        }

//    for (std::map<std::string,int>::iterator it=mymap.begin(); it!=mymap.end(); ++it){
//        ui->consuletxt->appendPlainText(QString::fromStdString(it->first));
//    }
    if (recType!="KNN" && images.size() <= 1) {
        QMessageBox::critical(this,"dataset Error", "This demo needs at least 2 images to work.\n Please add more images to your data set!");
        exitProgram();
        return;
        }


    //create model
    if(recType=="Eigenfaces"){
        model=createEigenFaceRecognizer();
        recogtype="Eigenfaces";
    }else if(recType == "Fisherfaces"){
        model=createFisherFaceRecognizer();
        recogtype="Fisherfaces";
    }else if(recType == "LBPH"){
        model= createLBPHFaceRecognizer();
        recogtype="LBPH";
    }else if (recType=="KNN"){

            list<Interval> intervals;
            intervals.push_back((Interval(facesvector.begin(),facesvector.end())));
            myknn.train(intervals);
            recogtype="KNN";
    }
    else{
        model=createEigenFaceRecognizer();
        recogtype="Eigenfaces";
    }

//    for (std::vector<int>::iterator it = labels.begin() ; it != labels.end(); ++it){
//       ui->consuletxt->appendPlainText(QString::number(*it));

//    }
    if(recType!="KNN")model->train(images, labels);
   QMessageBox::information(this,"train","successfully trained!");
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::on_recBtn_clicked()
{

    //ui->consuletxt->appendPlainText(QString("w: ")+QString::number(im_width));
    //ui->consuletxt->appendPlainText(QString("h: ")+QString::number(im_height));

    if ( recogtype==""){
        QMessageBox::critical(this,"dataset Error", "no model trained yet");
    }
    Mat face_resized;
    cv::resize(faceROI, face_resized, Size(im_width, im_height), 1.0, 1.0, INTER_CUBIC);
    int predictedLable;
    if(recogtype=="KNN"){

      String predictedName= myknn.recognize(face_resized,5,false) ;
       ui->consuletxt->appendPlainText(QString("you are ") + QString::fromStdString(predictedName));

    }
    else{

        im_width = images[0].cols;
        im_height = images[0].rows;
        predictedLable= model->predict(face_resized);
    for (std::map<std::string,int>::iterator it=mymap.begin(); it!=mymap.end(); ++it){
            //ui->consuletxt->appendPlainText(QString::fromStdString(it->first));
            if(it->second == predictedLable){
                    ui->consuletxt->appendPlainText(QString("you are ") + QString::fromStdString(it->first));
                    return;
            }
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////
bool MainWindow::ListFiles(wstring path, wstring mask, vector<wstring>& files)
{
    HANDLE hFind = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA fdata;
    wstring fullpath;
    stack<wstring> folders;
    folders.push(path);
    files.clear();
    while (!folders.empty()){
        path = folders.top();
        fullpath = path + L"\\" + mask;
        folders.pop();

        hFind = FindFirstFile(fullpath.c_str(), &fdata);

        if (hFind != INVALID_HANDLE_VALUE){
            do{
                if (wcscmp(fdata.cFileName, L".") != 0 &&
                    wcscmp(fdata.cFileName, L"..") != 0){
                    if (fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
                        folders.push(path + L"\\" + fdata.cFileName);
                    }
                    else{
                        files.push_back(path + L"\\" + fdata.cFileName);
                    }
                }
            } while (FindNextFile(hFind, &fdata) != 0);
        }
        if (GetLastError() != ERROR_NO_MORE_FILES){
            FindClose(hFind);
            return false;
        }

        FindClose(hFind);
        hFind = INVALID_HANDLE_VALUE;
    }

    return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::fillcsv()
{
    wofstream fout("train.txt");
    vector<wstring> files;
    ////////
//    string myadd = ExePath();
//    std::replace(myadd.begin(), myadd.end(), '\\', '/');
//    std::string::size_type found = myadd.rfind('/');
//    string myadd2 = myadd.substr(0,found);
//    QMessageBox::information(this,"title",QString::fromStdString(myadd2+"/orl_faces"));
    ///////
        //if (ListFiles(L"E:\\university\\faceRecognition\\codes\\faceRecognitionQt0\\orl_faces", L"*", files))
    if (ListFiles(L"faceDataset", L"*", files))
        {
            for (vector<wstring>::iterator iter = files.begin(); iter != files.end(); ++iter)
            {
                wstring hole = iter->c_str();
                std::string::size_type found = hole.rfind('\\');	//rfind('\\');
                wstring untillast = hole.substr(0, found);

                std::string::size_type prefound = untillast.rfind('\\');
                //wcout << untillast << " "<< hole.substr(prefound+1,found-prefound-1);
                //wcout << iter->c_str() << ";" << hole.substr(prefound + 1, found - prefound-1)<<endl;
                fout << iter->c_str() << ";" << hole.substr(prefound + 1, found - prefound - 1) << endl;
            }
        }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
string MainWindow::ExePath() {

    wchar_t buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    wstring ws(buffer);
    string str(ws.begin(), ws.end());
    string::size_type pos = str.find_last_of("\\/");
    return str.substr(0, pos);
}
