#include "ZuoGuangCFD.h"
//
//#include <sstream>
//
// using namespace std;
// using namespace Eigen;
// using namespace zgCFD;
//
// void test1() {
//    /*zgCFD::OfMesh mymesh("F:\\021.OPenFOAM\\of-8\\run\\test2\\constant\\polyMesh");*/
//    // OfMesh mymesh("F:\\027.CFDsoftware\\ZuoGuangCFD\\test\\of_mesh_test\\elbow\\constant\\polyMesh");
//
//    std::fstream tmp_field_file("F:\\021.OPenFOAM\\of-8\\run\\test2\\0\\aa", std::ios::in);
//
//    if (!tmp_field_file.is_open()) {
//        std::cerr << "不存在文件：" << std::endl;
//        exit(-1);
//    }
//
//    char tmp_char;
//    std::string tmp_string;
//    std::stringstream tmp_string1;
//    int tmp_num;
//    int tmp_number_of_values;
//
//    tmp_field_file >> tmp_char;
//    tmp_string1 << tmp_char;
//    tmp_field_file >> tmp_string;
//    tmp_string1 << tmp_string;
//    tmp_string1 >> tmp_num;
//    cout << tmp_num;
//
//    // while (true)
//    //{
//    //	tmp_field_file >> tmp_string;
//    //	if (tmp_string == "internalField")
//    //	{
//    //		break;
//    //	}
//    //}
//    ////tmp_field_file >> tmp_string;//internalField
//    // tmp_field_file >> tmp_string;
//
//    // if (tmp_string == "nonuniform") {
//
//    //	tmp_field_file >> tmp_string;
//
//    //	tmp_string = tmp_string.substr(tmp_string.find("<") + 1, tmp_string.find(">") - tmp_string.find("<") - 1);
//
//    //	if (tmp_string == "vector")
//    //	{
//    //		tmp_field_file >> tmp_number_of_values;
//    //		tmp_field_file >> tmp_char;//"("
//
//    //		for (int i = 0; i < tmp_number_of_values; i++)
//    //		{
//    //			tmp_field_file >> tmp_char;
//
//    //			tmp_field_file >> tmp_num;
//    //			cout << tmp_num << endl;
//    //			tmp_field_file >> tmp_num;
//    //			cout << tmp_num << endl;
//    //			tmp_field_file >> tmp_num;
//    //			cout << tmp_num << endl;
//
//    //			tmp_field_file >> tmp_char;
//    //		}
//
//    //		tmp_field_file >> tmp_string;//"};"
//    //	}
//    //	else if (tmp_string == "scalar")
//    //	{
//    //		tmp_field_file >> tmp_number_of_values;
//    //		tmp_field_file >> tmp_char;//"("
//
//    //		for (int i = 0; i < tmp_number_of_values; i++)
//    //		{
//    //			tmp_field_file >> tmp_num;
//    //			cout << tmp_num << endl;
//    //		}
//
//    //		tmp_field_file >> tmp_string;//"};"
//    //	}
//
//    //}
//}
//
// void PrintPoint(vector<zgCFD::point>& v) {
//    for (vector<zgCFD::point>::iterator it = v.begin(); it != v.end(); it++) {
//        cout << it->x_ << " " << it->y_ << " " << it->z_;
//        cout << endl;
//    }
//}
//
// void PrintBoundary(vector<zgCFD::boundary>& v) {
//    for (vector<zgCFD::boundary>::iterator it = v.begin(); it != v.end(); it++) {
//        cout << it->boundary_name_ << " " << it->index_ << " " << it->number_of_faces_ << " " << it->start_face_ << "
//        "
//             << it->type_of_boundary_;
//        cout << endl;
//    }
//}
//
// void PrintFace(vector<zgCFD::face>& v) {
//    for (vector<zgCFD::face>::iterator it = v.begin(); it != v.end(); it++) {
//        cout << it->owner_ << " " << it->neighbour_;
//        cout << endl;
//    }
//}
//
// class f {
//  public:
//    int a;
//};
//
// class s : public f {
//  public:
//    int b;
//};
// void print_ff(const f& ff) { cout << ff.a << endl; }
// void test2() {
//    zgCFD::CsrMatrix a(10, 8);
//    a.InsertValue(0, 0, 3);
//    a.InsertValue(4, 2, 5);
//    zgCFD::CsrMatrix b(8, 10);
//    b.InsertValue(0, 0, 6);
//    b.InsertValue(5, 6, 7);
//    b.InsertValue(6, 6, 8);
//    b.InsertValue(2, 6, 9);
//    zgCFD::CsrMatrix c(3, 3);
//    c.InsertValue(0, 0, 1);
//    c.InsertValue(0, 2, 2);
//    c.InsertValue(1, 2, 3);
//    c.InsertValue(2, 0, 4);
//    c.InsertValue(2, 1, 5);
//    c.InsertValue(2, 2, 6);
//    zgCFD::CsrMatrix d(3, 3);
//    cout << c << endl;
//    cout << d << endl;
//    d = c.Transpose();
//    cout << d << endl;
//
//    /*uint32_t a = 0;
//    cout << a << endl;*/
//}
//
// void test3() {
//    zgCFD::Vector<int> a(1, 1, 1);
//    zgCFD::Vector<int> b(2, 2, 2);
//    zgCFD::Vector<Scalar> c;
//    zgCFD::Tensor<Scalar> d(1, 2, 3, 4, 5, 6, 7, 8, 9);
//    zgCFD::Tensor<Scalar> e(1, 2, 3, 4, 5, 6, 7, 8, 9);
//    zgCFD::Tensor<Scalar> f;
//
//    f = d + e;
//    c = a + b;
//}
// void test4() {
//    Mesh* mymesh = new OfMesh("F:\\021.OPenFOAM\\of-8\\run\\test2\\constant\\polyMesh");
//    ScalarVolumeField p(*mymesh, "p", "F:\\021.OPenFOAM\\of-8\\run\\test2\\0", "no", "yes");
//
//    std::string tmp_file_path;
//    tmp_file_path = "F:\\021.OPenFOAM\\of-8\\run\\test2\\constant\\polyMesh\\pointss";
//    struct stat buffer;
//    if (stat(tmp_file_path.c_str(), &buffer) == 0) {
//        cout << "yes" << endl;
//    }
//
//    ScalarVolumeField a(1, *mymesh, "a", "b");
//    ScalarVolumeField b(2, *mymesh, "a", "b");
//
//    if (a == b) {
//        cout << "ok" << endl;
//    }
//
//    cout << "end" << endl;
//}
//
int main(int argc, char* argv[])
{
    QTextCodec* codec = QTextCodec::codecForName("UTF-8");  //或者"GBK",不分大小写
    QTextCodec::setCodecForLocale(codec);
    //    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());

    QApplication a(argc, argv);

    QString qss;
    QFile qssFile(":/qss/qss/flatwhite.css");
    qssFile.open(QFile::ReadOnly);

    if (qssFile.isOpen())
    {
        qss = QLatin1String(qssFile.readAll());
        qApp->setStyleSheet(qss);
        qssFile.close();
    }

    MainWindow w;
    w.show();
    return a.exec();
}
