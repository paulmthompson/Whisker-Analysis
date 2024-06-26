#ifndef DETECTOR_BANK_HPP
#define DETECTOR_BANK_HPP

#include <vector>
#include <unordered_map>
#include <array>

namespace janelia {

#define bigReal 1.E38f

enum SEED_METHOD_ENUM {
    SEED_ON_MHAT_CONTOURS, SEED_ON_GRID, SEED_EVERYWHERE
};

struct JaneliaConfig {
    SEED_METHOD_ENUM _seed_method {SEED_ON_GRID};
    int _lattice_spacing {30}; // SEED_ON_GRID_LATTICE_SPACING (pixels)
    int _maxr {4}; //SEED_SIZE_PX Width of the seed detector in pixels.
    int _maxiter {1}; // SEED_ITERATIONS Maxium number of iterations to re-estimate a seed.
    float _iteration_thres {0.0}; // SEED_ITERATION_THRESH (0 to 1) Threshold score determining when a seed should be reestimated.
    float _accum_thres {0.99f}; // SEED_ACCUM_THRESH (0 to 1) Threshold score determining when to accumulate statistics
    float _seed_thres {0.99f}; // SEED_THRESH (0 to 1) Threshold score determining when to generate a seed

    // These are all related to the detector bank (should be in that class)?
    // The detector bank will need to be updated if any of them change.
    int _tlen {8};  // (px) half the size of the detector support.
    float _angle_step {18.0}; // divisions of pi/4
    float _offset_step {0.1}; // pixels
    float _width_min {0.4}; // (pixels) must be a multiple of WIDTH_STEP
    float _width_max {6.5}; // (pixels) must be a multiple of WIDTH_STEP
    float _width_step {0.2}; // (pixels)
    float _min_signal {5.0}; // minimum detector response per detector column.  Typically: (2*TLEN+1)*MIN_SIGNAL is the threshold determining when tracing stops.
    float _half_space_assymetry {0.25}; // (between 0 and 1)  1 is completely insensitive to asymmetry
    float _max_delta_angle {10.1f}; // (degrees)  The detector is constrained to turns less than this value at each step.
    int _half_space_tunneling_max_moves {50}; // (pixels)  This should be the largest size of an occluding area to cross
    float _max_delta_width {6.0f}; // (pixels)   The detector width is constrained to change less than this value at each step.
    float _max_delta_offset {6.0f}; // (pixels)   The detector offset is constrained to change less than this value at each step.


    float _min_length {20.0f};
    float _redundancy_thres {5.0f};
};

struct Range {
    double min;
    double max;
    double step;
};

struct point {
    float x;
    float y;
};

struct ipoint {
    long x;
    long y;
};

struct rng {
    long mn;
    long mx;
};

struct box {
    point min;
    point max;
};

struct vertex {
    ipoint ip;
    rng rx;
    rng ry;
    short in;
};

struct Array {
    int ndim;
    std::array<int, 6> strides_bytes;
    std::array<int, 6> strides_px;
    std::array<int, 5> shape;
    std::vector<float> data;
    //std::unordered_map<int,float> data;

    Array() {

    }

    // ndim_in is always 5
    Array(std::array<int, 5> &shape_in, int bytesperpixel) {
        int i = 5;
        ndim = 5;
        shape = {};
        strides_bytes = {};
        strides_px = {};

        strides_bytes[ndim] = bytesperpixel;
        strides_px[ndim] = 1;

        while (i--)                                            // For shape = (w,h,d):
        {
            strides_bytes[i] = strides_bytes[i + 1] * shape_in[ndim - 1 - i];//   strides = (whd, wh, w, 1)
            strides_px[i] = strides_bytes[i] / bytesperpixel;
            shape[i] = shape_in[i];
        }
        data = std::vector<float>(strides_bytes[0]);
        //data.reserve(strides_bytes[0]);
    }
};


class DetectorBank {
public:
    Array bank;
    Range off;
    Range wid;
    Range ang;

    int compute_number_steps(Range r);

    int get_nearest(float offset, float width, float angle);

    int Get_Detector(const int ioffset, const int iwidth, const int iangle);

protected:

};

class LineDetector : public DetectorBank {
public:
    LineDetector();

    LineDetector(JaneliaConfig config);

private:
    void Build_Line_Detectors(float length, int supportsize);

    void Render_Line_Detector(float offset, float length,
                              float angle,
                              float width,
                              point anchor,
                              float *image, int *strides);
};

class HalfSpaceDetector : public DetectorBank {
public:
    HalfSpaceDetector();

    HalfSpaceDetector(JaneliaConfig config);

    float norm;

private:
    void Build_Half_Space_Detectors(float length, int supportsize);

    void Render_Half_Space_Detector(float offset,
                                    float length,
                                    float angle,
                                    float width,
                                    point anchor,
                                    float *image, int *strides);

};

template<std::size_t N>
void Simple_Line_Primitive(std::array<point, N> &verts, const point offset, const float length, const float thick);

/**
 * @brief Creates a point by adding a geometric vector to an existing point.
 *
 * This function creates a new point by adding the x and y components of a geometric vector to the x and y coordinates of an existing point, respectively.
 * The new point is located at the end of the vector if the vector is placed such that its start coincides with the existing point.
 *
 * @param vec A GeomVector object representing the vector to be added to the point.
 * @param p1 A Point2D<float> object representing the existing point.
 * @return A Point2D<float> object representing the new point.
 */
template<std::size_t N>
void rotate(std::array<point, N> &pbuf, const float angle);

/**
 * @brief Translates a set of points by a given offset.
 *
 * This function translates a set of points by a given offset. The translation is performed in-place, modifying the original points.
 * Each point in the array is moved by adding the x and y components of the offset to the x and y coordinates of the point, respectively.
 *
 * @tparam N The number of points in the array.
 * @param pbuf An array of points to be translated.
 * @param ori A point object representing the offset by which to translate the points.
 */
template<std::size_t N>
void translate(std::array<point, N> &pbuf, const point ori);

template<std::size_t N>
void Sum_Pixel_Overlap(std::array<point, N> &xy, const float gain, float *grid, int *strides);

void pixel_to_vertex_array(const int p, const int stride, std::array<point, 4> &v);

template<std::size_t N>
float inter(std::array<point, N> &a, std::array<point, 4> &b);

template<std::size_t N>
void range(box &B, std::array<point, N> &x);

/**
 * @brief Calculates the integrand for the line integral.
 *
 * This function calculates the integrand for the line integral, which is used in the computation of polygon area.
 * The formula used in this function is derived from Green's theorem. For more information, Google `Green's theorem polygon area`.
 *
 * @param f An ipoint object representing the first point.
 * @param t An ipoint object representing the second point.
 * @param w A short representing the weight of the line integral.
 * @return A long long representing the computed integrand for the line integral.
 */
long long cntrib(const ipoint f, const ipoint t, const short w);

/**
 * @brief Computes the area of a triangle defined by three points.
 *
 * This function calculates the area of a triangle defined by three points (a, p, q).
 * The area is calculated using the formula:
 * Area = p.x * q.y - p.y * q.x + a.x * (p.y - q.y) + a.y * (q.x - p.x)
 *
 * @param a An ipoint object representing the first point of the triangle.
 * @param p An ipoint object representing the second point of the triangle.
 * @param q An ipoint object representing the third point of the triangle.
 * @return A long long representing the computed area of the triangle.
 */
long long area(const ipoint a, const ipoint p, const ipoint q);

long long cross(vertex &a, vertex &b, vertex &c, vertex &d,
                const double a1, const double a2, const double a3, const double a4);


template<std::size_t N, std::size_t M>
double fit(box &B, std::array<point, N> &x, std::array<vertex, M> &ix, int fudge);

template<std::size_t M, std::size_t N>
long long inness(std::array<vertex, M> &P, std::array<vertex, N> &Q);

template<std::size_t N>
void Simple_Circle_Primitive(std::array<point, N> &verts, point center, float radius, int direction);

template<std::size_t N>
void Multiply_Pixel_Overlap(std::array<point, N> &xy, float gain, float boundary, float *grid, int *strides);
}

#endif // DETECTOR_BANK_HPP
