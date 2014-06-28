#include <opencv2/core/core.hpp> // fungsi draw
#include "opencv2/highgui/highgui.hpp" // fungsi gui
#include "opencv2/imgproc/imgproc.hpp" // fungsi image processing
#include <iostream>

using namespace cv;
using namespace std;

    string textSpeed, speedo;
    double sec, length;
    int speed;
    time_t startTime, endTime;
    bool enCount;
    Mat framePrev, frameNow, framePrev2, f1, f2, result;

int detectWhite(Mat result, int x1, int x2, int y1, int y2, bool countTime, int toleranceRate, string msg){
    int movement = 0;
    for(int j=x1;j<x2;j++)
    {
        for (int i=y1;i<y2;i++)
        {
            Scalar intensity = result.at<uchar>(i, j);
            if(intensity[0] == 255){
                movement++;
            }
        }
    }
    if(movement > toleranceRate){
        enCount = countTime;
        cout << msg << endl;
    }
}

int calcSpeed(int length){
        sec = difftime(endTime, startTime);

        speed = length / sec * 3600 / 1000;
        if(speed < 0){
            speed = 0;
        }
        //Konversi dari double ke string
        ostringstream stspeed;
        stspeed << speed;
        speedo = stspeed.str();
}


int main(int argc, char *argv[])
{
    VideoCapture cap("SAMPLEX.avi"); // Jika dex 0 - Menangkap video dari kamera default
    namedWindow("SpeedTrap", CV_WINDOW_AUTOSIZE); // Menamakan jendela dan mengaktifkan parameter autosize, penyesuaian dengan resolusi gambar yang ditangkap.
    namedWindow("SpeedTrap-Motion Process", CV_WINDOW_AUTOSIZE);

    // Inisiasi pembacaan
    cap.read(frameNow);
    frameNow.copyTo(framePrev);
    //framePrev.copyTo(framePrev2);

    while (1) // Loop untuk menampilkan video tertangkap per frame untuk ditampilkan ke jendela SpeedTrap
    {
        // Baca image dari video yang terbaca
        cap.read(frameNow);

        // Tampilkan informasi
        textSpeed = "KECEPATAN:" + speedo + "KM/JAM";
        putText(frameNow, textSpeed, Point(0,30), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0,255,0), 2, 8);

        // Gambarkan trigger
        rectangle(frameNow,Point(150,120),Point(250,140),Scalar( 0, 255, 0 ),1, 8); // Start Line the Upper Line
        rectangle(frameNow,Point(60,250),Point(340,270),Scalar( 0, 255, 0 ),1, 8); // End Line the Lower Line

        // Process Motion
        absdiff(framePrev,frameNow, result); // Hitung perbedaan frame saat ini dengan frame sebelumnya
        //absdiff(framePrev2,framePrev, f2); // Hitung perbedaan Frame 1 dengan frame 2
        //bitwise_or(f1,f2,result); // OR bit dari kedua hasil untuk memvalidasi bahwa adanya perbedaan di f1 juga terjadi di f2

        // Jadikan Grayscale
        cvtColor(result, result, CV_BGR2GRAY);

        // Lakukan tresholding
        threshold(result,result,70,255,CV_THRESH_BINARY);

        //framePrev.copyTo(framePrev2);
        frameNow.copyTo(framePrev);

        // Setiap frame yang ada dan diproses, ditampilkan
        imshow("SpeedTrap", frameNow);
        imshow("SpeedTrap-Motion Process", result);

        // Kotak Bawah
        if(enCount != true){
        detectWhite(result,65,335,255, 265, true, 20, "START COUNTING");
        // Trigger hitung waktu
        time(&startTime);
        }

        // Kotak Atas
        if(enCount != false){
        detectWhite(result,150,250,120,140, false, 20, "STOP COUNTING");
        // Trigger hentikan hitung waktu
        time(&endTime);
        //Hitung kecepatan dengan jarak yang diketahui
        calcSpeed(10);
        }

        // Tutup Window
        if (waitKey(30) == 27)
        {
            break;
        }
    }
    return 0;
}
