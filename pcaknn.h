#ifndef PCAKNN_H
#define PCAKNN_H

#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <list>
#include <vector>

using namespace cv;
using namespace std;

struct Face{
    Mat face;
    string name;
};

struct Weighting{
    unsigned int count;
    double total_dist;
};

struct Interval{
    vector<Face>::iterator start, end;//end is NOT in the INTERVAL!
    Interval(vector<Face>::iterator s, vector<Face>::iterator e){//make ref?
        start = s; end = e;
    }
    int Length() const{
        return end-start;
    }
};

class PCAKNN
{
private:
    Mat w, ev, mean;
    vector<Face> projections;
    unsigned int data_type;

    unsigned int width;
    unsigned int height;
    unsigned int n;
public:

    PCAKNN(unsigned int data_type_ = CV_32F);
    ~PCAKNN(void);

    unsigned int getWidth() const { return width; }
    unsigned int getHeight() const { return height; }

     void train(const list<Interval> &intervals, const bool console_output = true);
    string recognize(const Mat &face, const unsigned int k = 5, const bool use_distance_weighting = true, const bool console_output = true) const;
};

#endif // PCAKNN_H



