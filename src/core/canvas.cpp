#include <vividx/core/canvas.h>

#include "include/core/SkCanvas.h"
#include "include/core/SkColor.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct vx_canvas_t {
    SkCanvas *p;
};

vx_canvas_t* vx_canvas_new_from_SkCanvas(void *p_SkCanvas)
{
    vx_canvas_t *canvas = new vx_canvas_t;

    canvas->p = (SkCanvas*)p_SkCanvas;

    return canvas;
}

void vx_canvas_clear(vx_canvas_t *canvas)
{
    canvas->p->clear(SK_ColorCYAN);
}

void vx_canvas_free(vx_canvas_t *canvas)
{
    delete canvas;
}

#ifdef __cplusplus
}
#endif // __cplusplus