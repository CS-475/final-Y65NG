#include "include/GBitmap.h"
#include "include/GCanvas.h"
#include "include/GColor.h"
#include "include/GMath.h"
#include "include/GMatrix.h"
#include "include/GPoint.h"
#include "include/GShader.h"
#include "utils.h"
#include <cstdlib>
#include <vector>

class MyVoronoiShader : public GShader {
public:
    MyVoronoiShader(const GPoint* points, const GColor* colors, int count)
        : localMatrix(GMatrix()) {

        this->points = std::vector<GPoint>(points, points + count - 1);
        this->colors = std::vector<GColor>(colors, colors + count - 1);
    }

    bool isOpaque() override {
        for (auto color : colors) {
            if (color.a < 1) {
                return false;
            }
        }
        return true;
    }

    bool setContext(const GMatrix& ctm) override {
        auto result = ctm.invert();
        if (result.has_value()) {
            localMatrix = localMatrix * result.value();
            return true;
        }
        return false;
    }

    void shadeRow(int x, int y, int count, GPixel row[]) override {
        float px = localMatrix[0] * (x + 0.5) + localMatrix[2] * (y + 0.5) + localMatrix[4];
        float py = localMatrix[1] * (x + 0.5) + localMatrix[3] * (y + 0.5) + localMatrix[5];

        for (int i = 0; i < count; ++i) {
            auto minDist = (GPoint{px, py} - points[0]).length();
            auto minColor = colors[0];

            for (int j = 1; j < points.size(); ++j) {
                if ((GPoint{px, py} - points[j]).length() < minDist) {
                    minDist = (GPoint{px, py} - points[j]).length();
                    minColor = colors[j];
                }
            }

            row[i] = colorToPixel(minColor);

            px += localMatrix[0];
            py += localMatrix[1];
        }
    }

private:
    std::vector<GPoint> points;
    std::vector<GColor> colors;
    GMatrix localMatrix;
};
