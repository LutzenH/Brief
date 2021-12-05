#include "brief_visualization.h"

#include "../lib/stb_image_write.cpp"

#include <stdlib.h>


struct VisualizationScatter {
	u16 resolution_x;
	u16 resolution_y;
	Vector2 bounds_x;
	Vector2 bounds_y;
	u8* pixel_buffer;
};

VisualizationScatter* visualization_scatter_create(VisualizationScatterInfo* info) {
	VisualizationScatter* scatter = calloc(1, sizeof(VisualizationScatter) + (sizeof(u8) * 3 * info->resolution_x * info->resolution_y));
	scatter->resolution_x = info->resolution_x;
	scatter->resolution_y = info->resolution_y;
	scatter->bounds_x = info->bounds_x;
	scatter->bounds_y = info->bounds_y;
	scatter->pixel_buffer = ((u8*) scatter) + sizeof(VisualizationScatter);

	// Set background color
	for (i32 i = 0; i < scatter->resolution_x * scatter->resolution_y; ++i) {
		u8* pixel = scatter->pixel_buffer + (i * 3);

		pixel[0] = info->background_color[0];
		pixel[1] = info->background_color[1];
		pixel[2] = info->background_color[2];
	}

	return scatter;
}

bool visualization_scatter_series_add(VisualizationScatter* visualization, VisualizationScatterSeries* series) {
	for (i32 i = 0; i < series->data_count; ++i) {
		Vector2 position = {
				*((f32*) (((u8*) series->x_axis_data) + (i * series->x_axis_data_stride))),
				*((f32*) (((u8*) series->y_axis_data) + (i * series->y_axis_data_stride))),
		};

		// Check position is within graph bounds
		if (position.x >= visualization->bounds_x.data[0] && position.x <= visualization->bounds_x.data[1] && position.y >= visualization->bounds_y.data[0] && position.y <= visualization->bounds_y.data[1]) {
			f32 normalized_x = ((position.x - visualization->bounds_x.data[0])) / fabsf(visualization->bounds_x.data[0] - visualization->bounds_x.data[1]);
			f32 normalized_y = ((position.y - visualization->bounds_y.data[0])) / fabsf(visualization->bounds_y.data[0] - visualization->bounds_y.data[1]);

			i32 pixel_coord_x = CLAMP(i32(normalized_x * visualization->resolution_x), 0, visualization->resolution_x - 1);
			i32 pixel_coord_y = CLAMP(i32(normalized_y * visualization->resolution_y), 0, visualization->resolution_y - 1);

			u8* pixel = visualization->pixel_buffer + ((pixel_coord_y * visualization->resolution_x + pixel_coord_x) * 3);
			pixel[0] = series->color[0];
			pixel[1] = series->color[1];
			pixel[2] = series->color[2];
		}
	}

	return true;
}

bool visualization_scatter_save(VisualizationScatter* visualization, const char* png_path) {
	return stbi_write_png(png_path, visualization->resolution_x, visualization->resolution_y, 3, visualization->pixel_buffer, i32(visualization->resolution_x) * sizeof(u8) * 3);
}

bool visualization_scatter_destroy(VisualizationScatter* visualization) {
	free(visualization);
	return true;
}
