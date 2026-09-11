#ifndef VIVIDX_CORE_CANVAS_H
#define VIVIDX_CORE_CANVAS_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct vx_canvas_t vx_canvas_t;

vx_canvas_t* vx_canvas_new_from_SkCanvas(void *p_SkCanvas);

void vx_canvas_free(vx_canvas_t *canvas);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* VIVIDX_CORE_CANVAS_H */
