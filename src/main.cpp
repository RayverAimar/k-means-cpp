#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

#define PATH "../dataset/iris.data"
#define limiter ','
#define DATASET_LENGTH 150

using namespace std;

struct Point {
    float sepal_length, sepal_width, petal_length, petal_width;
    std::string belonged_class;
    int cluster;
    double minDist;

    Point() : 
        sepal_length(0.0), 
        sepal_width(0.0),
        petal_length(0.0),
        petal_width(0.0),
        cluster(-1),
        minDist(__DBL_MAX__) {}
        
    Point(float a, float b, float c, float d, std::string e = "") : 
        sepal_length(a), 
        sepal_width(b),
        petal_length(c),
        petal_width(d),
        belonged_class(e),
        cluster(-1),
        minDist(__DBL_MAX__) {}

    double distance(const Point &p) {
        return sqrt((p.sepal_length - sepal_length) * (p.sepal_length - sepal_length)
                + (p.sepal_width - sepal_width) * (p.sepal_width - sepal_width)
                + (p.petal_length - petal_length) * (p.petal_length - petal_length)
                + (p.petal_width - petal_width) * (p.petal_width - petal_width));
    }

    bool isEqualWith(const Point& p)
    {
        if(p.petal_length == petal_length && p.petal_width == petal_width && p.sepal_length == sepal_length && p.sepal_width == sepal_width)
            return true;
        return false;
    }

    void print()
    {    
        std::cout << "(" << sepal_length << ", " << sepal_width << ") (" << petal_length << ", " << petal_width << ") " << belonged_class << std::endl;
    }
};


std::vector<Point> dataset;
std::vector<Point> centroids;

std::vector<Point> read_from_csv()
{
    std::vector<Point> ans;
    std::ifstream file(PATH);
    std::string line;
    int idx = 0;
    while (getline(file, line) && idx++ < DATASET_LENGTH)
    {
        stringstream stream(line);
        string sepal_length, sepal_width, petal_length, petal_width, belonged_class;
        getline(stream, sepal_length, limiter);
        getline(stream, sepal_width, limiter);
        getline(stream, petal_length, limiter);
        getline(stream, petal_width, limiter);
        getline(stream, belonged_class, limiter);
        float _sepal_length, _sepal_width, _petal_length, _petal_width;
        _sepal_length = stof(sepal_length);
        _sepal_width = stof(sepal_width);
        _petal_length = stof(petal_length);
        _petal_width = stof(petal_width);
        Point temp(_sepal_length, _sepal_width, _petal_length, _petal_width, belonged_class);
        ans.push_back(temp);
    }
    return ans;
}

void Kmeans(std::vector<Point>& points, int n_centroids = 3, int max_iters = 300)
{
    for(int i = 0; i < n_centroids; i++)
    {
        centroids.push_back(points[i]);
        centroids[i].belonged_class = "";
    }
    bool iterating = true;
    int iters = 0;
    while(iterating && (iters < max_iters))
    {
        for(int i = 0; i < points.size(); i++)
        {
            for(int j = 0; j < centroids.size(); j++)
            {
                if(points[i].distance(centroids[j]) < points[i].minDist)
                {
                    points[i].minDist =  points[i].distance(centroids[j]);
                    points[i].cluster = j;
                }
            }
        }
        iterating = false;

        std::vector<Point> new_centroids;
        
        for(int i = 0; i < centroids.size(); i++)
        {
            float sumSepalLength = 0.0f, sumSepalWidth = 0.0f, sumPetalLength = 0.0f, sumPetalWidth = 0.0f;
            int n_points = 0;
            for(int j = 0; j < points.size(); j++)
            {
                if(points[j].cluster == i)
                {
                    sumSepalLength += points[j].sepal_length;
                    sumSepalWidth += points[j].sepal_width;
                    sumPetalLength += points[j].petal_length;
                    sumPetalWidth += points[j].petal_width;
                    n_points++;
                }
            }
            Point temp(sumSepalLength/n_points, sumSepalWidth/n_points, sumPetalLength/n_points, sumPetalWidth/n_points);
            new_centroids.push_back(temp);
        }

        for(int i = 0; i < centroids.size(); i++)
        {
            if(! centroids[i].isEqualWith(new_centroids[i]))
                iterating = true;
            centroids[i].petal_length = new_centroids[i].petal_length;
            centroids[i].petal_width = new_centroids[i].petal_width;
            centroids[i].sepal_length = new_centroids[i].sepal_length;
            centroids[i].sepal_width = new_centroids[i].sepal_width;
        }
        if(iterating) iters++;
    }
}

int main()
{
    dataset = read_from_csv();
    
    Kmeans(dataset);
    
    for(int j = 0; j < centroids.size(); j++)
    {
        std::cout << "\n\nCLUSTER [" << j << "]\n" << std::endl;
        for(int i = 0; i < dataset.size(); i++)
        {
            if(dataset[i].cluster == j)
            {
                std::cout << dataset[i].cluster << " -> ";
                dataset[i].print();
            }
        }   
    }
    
    std::cout << "\n\n";

    for(int i = 0; i < centroids.size(); i++)
    {
        std::cout << "CENTROID [" << i << "]: ";
        centroids[i].print();
    }

}