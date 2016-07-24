#include "pcaknn.h"


PCAKNN::PCAKNN(unsigned int data_type_) {
    this->data_type = data_type_;
    n = width = height = 0;
}

PCAKNN::~PCAKNN(void) {}

void PCAKNN::train(const list<Interval> &intervals, const bool console_output) {
    if (intervals.size() == 0) return;
    projections.clear(); n = 0;

    width = intervals.front().start->face.cols;
    height = intervals.front().start->face.rows;

    list<Interval>::const_iterator itr;
    vector<Face>::const_iterator start, end;
    //add the number of images from all intervals
    for (itr = intervals.cbegin(); itr != intervals.cend(); itr++) {
        n += itr->Length();
    }

    Mat pca_matrix(static_cast<int>(n), width*height, data_type);
    int c = 0;
    for (itr = intervals.cbegin(); itr != intervals.cend(); itr++) {
        //for each image in the current interval
        for (start = itr->start, end = itr->end; start != end; start++, ++c) {
            if (console_output) printf("Preparing samples %d/%d\n", c + 1, n);
            //insert current image into pca_matrix
            Mat image_row = start->face.clone().reshape(1, 1);
            Mat row_i = pca_matrix.row(c);
            image_row.convertTo(row_i, data_type);//CV_64FC1 ?
            Face f; f.name = start->name;
            projections.push_back(f);//save the names for later
        }
    }

    if (console_output) printf("TRAINING...\n");
    //Perfrom principal component analysis on pca_matrix
    PCA pca(pca_matrix, Mat(), CV_PCA_DATA_AS_ROW, pca_matrix.rows);

    //extract mean/eigenvalues
    mean = pca.mean.reshape(1, 1);
    ev = pca.eigenvalues.clone();
    transpose(pca.eigenvectors, w);

    //project each face into subspace and save them with the name above for recognition
    for (unsigned int i = 0; i<n; ++i) {
        if (console_output) printf("Projecting %d/%d\n", i + 1, n);//project so subspace
        projections[i].face = subspaceProject(w, mean, pca_matrix.row(i));
    }
}

//k should be uneven to break ties
string PCAKNN::recognize(const Mat &face, const unsigned int k, const bool use_distance_weighting, const bool console_output) const {
    string name = "N/A";
    if (n <= k) return name;

    //project target face to subspace
    Mat target = subspaceProject(w, mean, face.reshape(1, 1));

    vector<unsigned int> classes(k, 0);
    vector<double> distances(k, DBL_MAX);

    double dist = DBL_MAX;
    //find k nearest neighbours
    for (unsigned int i = 0; i < projections.size(); ++i) {
        dist = norm(projections[i].face, target, NORM_L2);//norml2
        for (unsigned int j = 0; j < k; ++j) {
            if (dist < distances[j]) {
                //discard the worst match and shift remaining down
                for (int l = k - 1; l > j; --l) {
                    distances[l] = distances[l - 1];
                    classes[l] = classes[l - 1];
                }
                classes[j] = i; //set new best match
                distances[j] = dist;
                break;
            }
        }
    }

    map<string, Weighting> neighbours;
    //count occurence of classes
    for (unsigned int i = 0; i<k; ++i) {
        Weighting &weight = neighbours[projections[classes[i]].name];
        weight.count++;
        weight.total_dist += distances[i];
    }

    //evaluate voting
    if (use_distance_weighting) {
        double min_weight = DBL_MAX;
        for (map<string, Weighting>::iterator itr = neighbours.begin(); itr != neighbours.end(); ++itr) {
            double weight = itr->second.total_dist / (double)itr->second.count;
            //concider average weight instead of number of votes
            if (weight < min_weight) {
                min_weight = weight;
                name = itr->first;
            }
            if (console_output)printf("%s %f\n", itr->first.c_str(), weight);
        }
    }
    else {
        unsigned int max_count = 0;
        //choose the class with the most votes
        for (map<string, Weighting>::iterator itr = neighbours.begin(); itr != neighbours.end(); ++itr) {
            if (itr->second.count > max_count) {
                max_count = itr->second.count;
                name = itr->first;
            }
            if (console_output)printf("%s %d\n", itr->first.c_str(), itr->second.count);
        }
    }

    return name;
}
