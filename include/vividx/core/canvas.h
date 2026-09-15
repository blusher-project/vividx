#ifndef VIVIDX_CORE_CANVAS_H
#define VIVIDX_CORE_CANVAS_H

#include <vividx/core/color.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct vx_canvas_t vx_canvas_t;

vx_canvas_t* vx_canvas_new_from_SkCanvas(void *p_SkCanvas);

void vx_canvas_clear(vx_canvas_t *canvas, vx_color_t color);

void vx_canvas_free(vx_canvas_t *canvas);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* VIVIDX_CORE_CANVAS_H */
