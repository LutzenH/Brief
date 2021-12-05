#ifndef BRIEF_BRIEF_VISUALIZATION_H
#define BRIEF_BRIEF_VISUALIZATION_H

#include "brief_types.h"

typedef struct VisualizationScatter VisualizationScatter;

typedef struct VisualizationScatterInfo VisualizationScatterInfo;
struct VisualizationScatterInfo {
	u16 resolution_x;
	u16 resolution_y;
	Vector2 bounds_x;
	Vector2 bounds_y;
	u8 background_color[3];
};

typedef struct VisualizationScatterSeries VisualizationScatterSeries;
struct VisualizationScatterSeries {
	u8 color[3];
	size_t data_count;
	f32* x_axis_data;
	size_t x_axis_data_stride;
	f32* y_axis_data;
	size_t y_axis_data_stride;
};

VisualizationScatter* visualization_scatter_create(VisualizationScatterInfo* info);
bool visualization_scatter_destroy(VisualizationScatter* visualization);
bool visualization_scatter_series_add(VisualizationScatter* visualization, VisualizationScatterSeries* series);
bool visualization_scatter_save(VisualizationScatter* visualization, const char* png_path);

#endif//BRIEF_BRIEF_VISUALIZATION_H
