#ifndef __PX_HEAD_H__
#define __PX_HEAD_H__
pxhead_t *get_px_head(pxdoc_t *pxdoc, FILE *fp);
int put_px_head(pxdoc_t *pxdoc, pxhead_t *pxh, FILE *fp);
#endif
