#include <stdio.h>
#include <math.h>
#include <sstream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include<time.h>
#include<sstream>
#include<fstream>

using namespace std;
using namespace cv;

int main() {
	int num;
	cout << "請輸入圖片代號(1.Bunny 2.Eagle 3.Teapot 4.Venus):";
	cin >> num;
	cout << "please wait..." << endl;

	Mat Pic2D[8];
	stringstream Picname;
	//根據代號讀入8張圖片
	if (num == 1) {
		for (int i = 0; i < 8; i++) {
			Picname.str("");
			Picname.clear();
			Picname << "Bunny\\\\" << "00" << i + 1 << ".bmp";
			Pic2D[i] = imread(Picname.str(), 0);
		}
	}

	else if (num == 2) {
		for (int i = 0; i < 8; i++) {
			Picname.str("");
			Picname.clear();
			Picname << "Eagle\\\\" << "00" << i + 1 << ".bmp";
			Pic2D[i] = imread(Picname.str(), 0);
		}
	}

	else if (num == 3) {
		for (int i = 0; i < 8; i++) {
			Picname.str("");
			Picname.clear();
			Picname << "Teapot\\\\" << "00" << i + 1 << ".bmp";
			Pic2D[i] = imread(Picname.str(), 0);
		}
	}

	else if (num == 4) {
		for (int i = 0; i < 8; i++) {
			Picname.str("");
			Picname.clear();
			Picname << "Venus\\\\" << "00" << i + 1 << ".bmp";
			Pic2D[i] = imread(Picname.str(), 0);
		}
	}

	//instrinsic parameter
	Mat inPara = (Mat_<float>(3, 3) <<
		714.074036, 0.000000, 400.000000,
		0.000000, 714.073975, 300.000000,
		0.000000, 0.000000, 1.000000
		);
	//extrinsic parameter
	Mat exPara[8];
	exPara[0] = (Mat_<float>(3, 4) <<
		0.866849, -0.493982, -0.067491, 5.680202,
		-0.033026, 0.078177, -0.996382, 40.192253,
		0.497476, 0.865950, 0.051456, 131.575211
		);
	exPara[1] = (Mat_<float>(3, 4) <<
		0.691898, 0.718699, -0.068914, 3.033622,
		-0.018523, -0.077748, -0.996791, 40.538036,
		-0.721758, 0.690961, -0.040479, 132.814957
		);
	exPara[2] = (Mat_<float>(3, 4) <<
		-0.708825, 0.705031, -0.022315, 2.710659,
		-0.023775, -0.055497, -0.998163, 40.025272,
		-0.704984, -0.707001, 0.056103, 131.650055
		);
	exPara[3] = (Mat_<float>(3, 4) <<
		-0.990984, -0.131983, -0.023039, -1.359515,
		0.033893, -0.080598, -0.996156, 40.564919,
		0.129620, -0.987970, 0.084348, 126.794678
		);
	exPara[4] = (Mat_<float>(3, 4) <<
		-0.071742, -0.997325, 0.014031, 10.313296,
		-0.040528, -0.011141, -0.999102, 40.912224,
		0.996600, -0.072247, -0.039619, 127.704018
		);
	exPara[5] = (Mat_<float>(3, 4) <<
		-0.946351, -0.190357, 0.261120, -10.116337,
		-0.241168, 0.953881, -0.178661, 7.099833,
		-0.215071, -0.232055, -0.948628, 156.788635
		);
	exPara[6] = (Mat_<float>(3, 4) <<
		-0.280008, 0.952858, 0.116867, -2.020520,
		-0.725914, -0.130496, -0.675269, 28.864723,
		-0.628193, -0.273920, 0.728245, 106.115578
		);
	exPara[7] = (Mat_<float>(3, 4) <<
		0.897493, -0.360072, -0.254667, 8.695093,
		0.088773, 0.713100, -0.695396, 31.042011,
		0.432002, 0.601513, 0.671979, 125.422333
		);

	Mat PM, point3D, point2D;
	string outputName = "output.xyz";
	fstream outf;
	outf.open(outputName.c_str(), ios::out);

	for (int x = -50; x <= 50; x++)
	for (int y = -50; y <= 50; y++)
	for (int z = -10; z <= 90; z++)
	for (int i = 0; i <= 7; i++) {
		point3D = (Mat_<float>(4, 1) <<
			x, y, z, 1);
		PM = inPara*exPara[i];
		point2D = PM*point3D;
		//正規化
		int X = point2D.at<float>(0, 0) = point2D.at<float>(0, 0) / point2D.at<float>(2, 0); //X
		int Y = point2D.at<float>(1, 0) = point2D.at<float>(1, 0) / point2D.at<float>(2, 0); //Y
		point2D.at<float>(2, 0) = 1;
		//檢查有無超出邊界或是對應的8張圖pixel是否為0，若成立則跳出迴圈
		if (X > Pic2D[i].cols || Y > Pic2D[i].rows || X < 0 || Y < 0 || Pic2D[i].at<unsigned char>(Y, X) == 0)
			break;
		//在邊界內且8張圖pixel均為255，存入檔案中
		if (i == 7) {
			outf << x << " " << y << " " << z << endl;
		}
	}

	cout << "complete." << endl;
	system("pause");
	return 0;
}
