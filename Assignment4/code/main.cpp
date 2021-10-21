#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

vector<Point2f> inputPoints;

void mouse_handler(int event, int x, int y, int flags, void *userdata)
{
    if (event == EVENT_LBUTTONDOWN && inputPoints.size() < 4)
    {
        cout << "Left button of the mouse is clicked - position (" << x << ", "
             << y << ")" << '\n';
        inputPoints.emplace_back(x, y);
    }
}

void naive_bezier(const vector<Point2f> &points, Mat &window)
{
    auto &p_0 = points[0];
    auto &p_1 = points[1];
    auto &p_2 = points[2];
    auto &p_3 = points[3];

    for (double t = 0.0; t <= 1.0; t += 0.001)
    {
        auto point = pow(1 - t, 3) * p_0 + 3 * t * pow(1 - t, 2) * p_1 +
                     3 * pow(t, 2) * (1 - t) * p_2 + pow(t, 3) * p_3;

        window.at<Vec3b>(point.y, point.x)[2] = 255;
    }
}

Point2f recursive_bezier(const vector<Point2f> &inputPoints, float t)
{
    if (inputPoints.size() == 2)
    {
        return (1 - t) * inputPoints[0] + t * inputPoints[1];
    }

    vector<Point2f> vec;
    for (int i = 0; i < inputPoints.size() - 1; i++)
    {
        vec.push_back((1 - t) * inputPoints[i] + t * inputPoints[i + 1]);
    }
    return recursive_bezier(vec, t);
}

void bezier(const vector<Point2f> &inputPoints, Mat &window)
{
    // TODO: Iterate through all t = 0 to t = 1 with small steps, and call de Casteljau's
    // recursive Bezier algorithm.
    for (double t = 0.0; t <= 1.0; t += 0.001)
    {
        auto point = recursive_bezier(inputPoints, t);
        window.at<Vec3b>(point.y, point.x)[1] = 255;
    }
}

int main()
{
    Mat window = Mat(700, 700, CV_8UC3, Scalar(0));
    cvtColor(window, window, COLOR_BGR2RGB);
    namedWindow("Bezier Curve", WINDOW_AUTOSIZE);

    setMouseCallback("Bezier Curve", mouse_handler, nullptr);

    int key = -1;
    while (key != 27)
    {
        for (auto &point : inputPoints)
        {
            circle(window, point, 3, {255, 255, 255}, 3);
        }

        if (inputPoints.size() == 4)
        {
            naive_bezier(inputPoints, window);
            //   bezier(inputPoints, window);

            imshow("Bezier Curve", window);
            imwrite("my_bezier_curve.png", window);
            key = waitKey(0);

            return 0;
        }

        imshow("Bezier Curve", window);
        key = waitKey(20);
    }

    return 0;
}
