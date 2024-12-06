#include "MyVoronoiShader.h"
#include "include/GFinal.h"
#include "include/GPath.h"
#include "include/GPathBuilder.h"
#include "include/GShader.h"
#include <memory>

class MyFinal : public GFinal {
    std::shared_ptr<GShader> createVoronoiShader(const GPoint points[], const GColor colors[],
                                                 int count) override {
        return std::make_shared<MyVoronoiShader>(points, colors, count);
    }

    std::shared_ptr<GPath> strokePolygon(const GPoint points[], int count, float width,
                                         bool isClosed) override {
        GPathBuilder path;

        for (int i = 1; i < count; ++i) {
            path = strokeTo(path, points[i - 1], points[i], width);
        }

        if (isClosed) {
            path = strokeTo(path, points[count - 1], points[0], width);
        }

        return path.detach();
    }
};

std::unique_ptr<GFinal> GCreateFinal() { return std::make_unique<MyFinal>(); }
