#include "config.h"
#include <stdio.h>
#include <fcntl.h>
#include <paradox.h>
#include <px_intern.h>
#include <px_memory.h>

/*
 * get the header info from the file
 *	basic header info & field descriptions
 */
pxhead_t *get_px_head(pxdoc_t *pxdoc, FILE *fp)
{
	pxhead_t *pxh;
	TPxHeader pxhead;
	TPxDataHeader pxdatahead;
	TFldInfoRec pxinfo;
	pxfield_t *pfield;
	char dummy[300], c;
	int ret, i, j;

	if((pxh = (pxhead_t *) pxdoc->malloc(pxdoc, sizeof(pxhead_t), _("Couldn't get memory for document header."))) == NULL)
		return NULL;
	if(fseek(fp, 0, 0) < 0)
		return NULL;
	if((ret = fread(&pxhead, sizeof(TPxHeader), 1, fp)) < 0) {
		pxdoc->free(pxdoc, pxh);
		return NULL;
	}

	/* check some header fields for reasonable values */
	if(pxhead.fileType > 8) {
		pxdoc->free(pxdoc, pxh);
		px_error(pxdoc, PX_RuntimeError, _("Paradox file has unknown file type (%d)."), pxhead.fileType);
		return NULL;
	}
	if(pxhead.maxTableSize > 32 || pxhead.maxTableSize < 1) {
		pxdoc->free(pxdoc, pxh);
		px_error(pxdoc, PX_RuntimeError, _("Paradox file has unknown table size (%d)."), pxhead.maxTableSize);
		return NULL;
	}
	if(pxhead.fileVersionID > 15 || pxhead.fileVersionID < 3) {
		pxdoc->free(pxdoc, pxh);
		px_error(pxdoc, PX_RuntimeError, _("Paradox file has unknown file version (0x%X)."), pxhead.fileVersionID);
		return NULL;
	}

	pxh->px_recordsize = get_short_le(&pxhead.recordSize);
	if(pxh->px_recordsize == 0) {
		pxdoc->free(pxdoc, pxh);
		px_error(pxdoc, PX_RuntimeError, _("Paradox file has zero record size."));
		return NULL;
	}
	pxh->px_headersize = get_short_le(&pxhead.headerSize);
	if(pxh->px_headersize == 0) {
		pxdoc->free(pxdoc, pxh);
		px_error(pxdoc, PX_RuntimeError, _("Paradox file has zero header size."));
		return NULL;
	}
	pxh->px_filetype = pxhead.fileType;
	pxh->px_numrecords = get_long_le(&pxhead.numRecords);
	pxh->px_numfields = get_short_le(&pxhead.numFields);
	pxh->px_fileblocks = get_short_le(&pxhead.fileBlocks);
	switch(pxhead.fileVersionID) {
		case 3:
			pxh->px_fileversion = 30;
			break;
		case 4:
			pxh->px_fileversion = 35;
			break;
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			pxh->px_fileversion = 40;
			break;
		case 10:
		case 11:
			pxh->px_fileversion = 50;
			break;
		case 12:
			pxh->px_fileversion = 70;
			break;
		default:
			pxh->px_fileversion = 0;
	}
	pxh->px_indexfieldnumber = pxhead.indexFieldNumber;
	pxh->px_writeprotected = pxhead.writeProtected;
	pxh->px_modifiedflags1 = pxhead.modifiedFlags1;
	pxh->px_modifiedflags2 = pxhead.modifiedFlags2;
	pxh->px_primarykeyfields = get_short_le(&pxhead.primaryKeyFields);

	if(((pxh->px_filetype == 0) ||
		  (pxh->px_filetype == 2) ||
		  (pxh->px_filetype == 3) ||
		  (pxh->px_filetype == 5)) &&
		  (pxh->px_fileversion >= 40)) {
		if((ret = fread(&pxdatahead, sizeof(TPxDataHeader), 1, fp)) < 0) {
			pxdoc->free(pxdoc, pxh);
			return NULL;
		}
		pxh->px_doscodepage = get_short_le(&pxdatahead.dosCodePage);
	}

	pxh->px_maxtablesize = pxhead.maxTableSize;
	pxh->px_sortorder = pxhead.sortOrder;
	pxh->px_refintegrity = pxhead.refIntegrity;
	pxh->px_autoinc = get_long_le(&pxhead.autoInc);

	/* The theoretical number of records is calculated from the number
	 * of data blocks and the number of records that fit into a data
	 * block. The '-6' is decreasing the available space of the data
	 * block due to its header, which takes up 6 Bytes.
	 */
	pxh->px_theonumrecords = pxh->px_fileblocks * (int) ((pxh->px_maxtablesize*0x400-6) / pxh->px_recordsize);

	if((pxh->px_fields = (pxfield_t *) pxdoc->malloc(pxdoc, pxh->px_numfields*sizeof(pxfield_t), _("Could not get memory for field definitions."))) == NULL)
		return NULL;

	pfield = pxh->px_fields;
	for(i=0; i<pxh->px_numfields; i++) {
		if((ret = fread(&pxinfo, sizeof(TFldInfoRec), 1, fp)) < 0) {
			pxdoc->free(pxdoc, pxh->px_fields);
			pxdoc->free(pxdoc, pxh);
			return NULL;
		}
		pfield->px_ftype = pxinfo.fType;
		if(pfield->px_ftype == pxfBCD) {
			pfield->px_flen = 17;
			pfield->px_fdc = pxinfo.fSize;
		} else {
			pfield->px_flen = pxinfo.fSize;
			pfield->px_fdc = 0;
		}
		pfield++;
	}

	/* skip the tableNamePtr */
	if((ret = fread(dummy, sizeof(int), 1, fp)) < 0) {
		pxdoc->free(pxdoc, pxh->px_fields);
		pxdoc->free(pxdoc, pxh);
		return NULL;
	}

	/* skip the tfieldNamePtrArray, not present in index files */
	if(pxhead.fileType == 0 || pxhead.fileType == 2) {
		for(i=0; i<pxh->px_numfields; i++) {
			if((ret = fread(dummy, sizeof(int), 1, fp)) < 0) {
				pxdoc->free(pxdoc, pxh->px_fields);
				pxdoc->free(pxdoc, pxh);
				return NULL;
			}
		}
	}

	/* skip the tableName */
	if(pxh->px_fileversion >= 70)
		ret = fread(dummy, 261, 1, fp);
	else
		ret = fread(dummy, 79, 1, fp);
	if(ret < 0) {
		pxdoc->free(pxdoc, pxh->px_fields);
		pxdoc->free(pxdoc, pxh);
		return NULL;
	}
	pxh->px_tablename = px_strdup(pxdoc, dummy);

	pfield = pxh->px_fields;
	for(i=0; i<pxh->px_numfields; i++) {
		j=0;
		while(((ret = fread(&c, 1, 1, fp)) >= 0) && (c != '\0')) {
			dummy[j++] = c;
		}
		if(ret < 0) {
			pxdoc->free(pxdoc, pxh->px_tablename);
			pxdoc->free(pxdoc, pxh->px_fields);
			pxdoc->free(pxdoc, pxh);
			return NULL;
		}
		dummy[j] = '\0';
		pfield->px_fname = px_strdup(pxdoc, (const char *)dummy);
		pfield++;
	}

	return pxh;
}

