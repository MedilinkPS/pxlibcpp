#ifndef __PX_HEAD_H__
#define __PX_HEAD_H__
pxhead_t *get_px_head(pxdoc_t *pxdoc, pxstream_t *pxs);
int put_px_head(pxdoc_t *pxdoc, pxhead_t *pxh, pxstream_t *pxs);
int put_px_datablock(pxdoc_t *pxdoc, pxhead_t *pxh, int after, pxstream_t *pxs);
int px_add_data_to_block(pxdoc_t *pxdoc, pxhead_t *pxh, int datablocknr, char *data, pxstream_t *pxs);

mbhead_t *get_mb_head(pxblob_t *pxblob, pxstream_t *pxs);
int put_mb_head(pxblob_t *pxblob, mbhead_t *mbh, pxstream_t *pxs);
#endif
