#ifndef JANELIA_HPP
#define JANELIA_HPP

#include <cstdint>
#include <vector>
#include <memory>
#include <optional>

#include "detector_bank.hpp"

namespace janelia {

struct Whisker_Seg {

    Whisker_Seg() = default;

    Whisker_Seg(int n)
            :
            len{n}, x(n), y(n), thick(n), scores(n) {}

    int id{0};
    int time{0};
    int len{0};
    std::vector<float> x{};
    std::vector<float> y{};
    std::vector<float> thick{};
    std::vector<float> scores{};

};

template<typename T>
struct Image {
    int width;
    int height;
    std::vector<T> array;

    Image() {
        width = 0;
        height = 0;
        std::vector<T> array{};
    }

    Image(int w, int h) {
        width = w;
        height = h;
        array = std::vector<T>(h * w, 0);
    }

    Image(int w, int h, std::vector<T> img) {
        width = w;
        height = h;
        array = img;
    }
};

struct Hist {
    uint8_t h; // Histogram
    float th; //Slopes
    float s; // Stats
    bool mask; // Mask of seeds
};

struct Seed {
    int xpnt;
    int ypnt;
    int xdir;
    int ydir;
};

struct Line_Params {
    float offset;
    float angle;
    float width;
    float score;
};

struct seedrecord {
    seedrecord() = default;

    int idx{0};
    float score{0.0f};
};

struct record {
    record() : record(0.0f, 0.0f, 0.0f, 0.0f) {}

    record(float x, float y, float thick, float score) : x(x), y(y), thick(thick), score(score) {}

    float x;
    float y;
    float thick;
    float score;
};

struct Interval {
    double min;
    double max;
};

struct offset_pair {
    offset_pair() : image_ind(0), weight_ind(0) {}

    offset_pair(int i, int w) : image_ind(i), weight_ind(w) {}

    int image_ind;
    int weight_ind;
};

class JaneliaTracker {

public:
    JaneliaTracker();

    std::vector<Whisker_Seg> find_segments(int iFrame, Image<uint8_t> &image, const Image<uint8_t> &bg);

    JaneliaConfig config;
    LineDetector bank;
    HalfSpaceDetector half_space_bank;
private:

    std::vector<offset_pair> pxlist;
    int _snpx {0};
    int _lastp {-1};
    int _last_issmallangle {-1};
    float _trust_thresh {-1.0};
    float _trust_thresh_conservative {-1.0};
    Image<uint8_t> _mask; // Mask for keeping track of seed points
    Image<uint8_t> _histogram; // histogram from compute_seed_from_point_field_windowed_on_contour
    Image<float> _slopes;
    Image<float> _stats;
    Seed _myseed;
    std::vector<record> _ldata;
    std::vector<record> _rdata;

    void compute_seed_from_point_field_on_grid(const Image<uint8_t> &image, Image<uint8_t> &h, Image<float> &th,
                                               Image<float> &s);

    std::optional<Seed> compute_seed_from_point(const Image<uint8_t> &image, int p, int maxr);

    std::optional<Seed>
    compute_seed_from_point_ex(const Image<uint8_t> &image, int p, int maxr, float *out_m, float *out_stat);

    Line_Params line_param_from_seed(const Seed s);

    float eval_line(Line_Params *line, const Image<uint8_t> &image, int p);

    std::pair<float, int> round_anchor_and_offset(const Line_Params line, const int p, const int stride);

    void get_offset_list(const Image<uint8_t> &image, const int support, const float angle, int p, int *npx);

    Whisker_Seg trace_whisker(Seed s, Image<uint8_t> &image);

    float eval_half_space(Line_Params *line, const Image<uint8_t> &image, int p, float *rr, float *ll);

    int move_line(Line_Params *line, const int p, const int stride, const int direction);

    int adjust_line_start(Line_Params *line, const Image<uint8_t> &image, int *pp,
                          Interval *roff, Interval *rang, Interval *rwid);

    bool is_change_too_big(const Line_Params new_line, const Line_Params old, const float alim, const float wlim,
                           const float olim);

    bool is_local_area_trusted(Line_Params *line, Image<uint8_t> &image, int p);

    bool is_local_area_trusted_conservative(Line_Params *line, Image<uint8_t> &image, int p);

};

int threshold_bottom_fraction_uint8(const Image<uint8_t> &im);
float threshold_two_means(uint8_t *array, size_t size);

#define _COMPUTE_SEED_FROM_POINT_HELPER(BEST, BP)                    \
{ int tp = x+cx + image.width * (y+cy);                            \
  uint8_t val = image.array[tp];                                   \
  if(   val <= BEST )                                               \
  { BP = tp;                                                        \
    BEST =  val;                                                    \
  }                                                                 \
}
}

#endif // JANELIA_HPP
