/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

#include "rm_httpd_opts.h"
#include "lwip/def.h"
#include "lwip/apps/fs.h"
#include <string.h>

#if SUPPORT_FSP_RM_FS_W

#include "lfs.h"
#include "lwip/err.h"
#include "rm_lwip_w_fs_parse.h"

#endif

#include HTTPD_FSDATA_FILE

/*-----------------------------------------------------------------------------------*/

#if LWIP_HTTPD_CUSTOM_FILES
int fs_open_custom(struct fs_file *file, const char *name);
void fs_close_custom(struct fs_file *file);
#if LWIP_HTTPD_FS_ASYNC_READ
u8_t fs_canread_custom(struct fs_file *file);
u8_t fs_wait_read_custom(struct fs_file *file, fs_wait_cb callback_fn, void *callback_arg);
int fs_read_async_custom(struct fs_file *file, char *buffer, int count, fs_wait_cb callback_fn, void *callback_arg);
#else /* LWIP_HTTPD_FS_ASYNC_READ */
int fs_read_custom(struct fs_file *file, char *buffer, int count);
#endif /* LWIP_HTTPD_FS_ASYNC_READ */
#endif /* LWIP_HTTPD_CUSTOM_FILES */


#if SUPPORT_FSP_RM_FS_W

/* Extern: your mounted LittleFS context */
lfs_t * p_lfs = &g_rm_littlefs0_lfs;

static err_t fs_validate_path(const char * p_path)
{
    if ((!p_path) || (p_path[0] != '/') || (strlen(p_path) >= 255))
    {
        return LFS_ERR_INVAL;
    }

    return LFS_ERR_OK;
}

err_t user_fs_open(struct lfs_file_data * p_file, const char * p_path)
{
    int offset = 0;
    int length = -1;
    err_t err;
    int to_read;
    uint8_t * p_buf;
    int rlen;
    int rc;

    err = fs_validate_path(p_path);
    if (err != ERR_OK)
    {
        return err;
    }

    rc = lfs_file_open(&g_rm_littlefs0_lfs, &p_file->lfs_file, p_path, LFS_O_RDONLY);
    if (rc < 0)
    {
        return rc;
    }

    lfs_soff_t fsz = lfs_file_size(&g_rm_littlefs0_lfs, &p_file->lfs_file);
    if (fsz < 0)
    {
        lfs_file_close(&g_rm_littlefs0_lfs, &p_file->lfs_file);
        return fsz;
    }

    if (offset > (int)fsz)
    {
        lfs_file_close(&g_rm_littlefs0_lfs, &p_file->lfs_file);
        return LFS_ERR_INVAL;
    }

    lfs_file_seek(&g_rm_littlefs0_lfs, &p_file->lfs_file, offset, LFS_SEEK_SET);
    if (length < 0)
    {
      to_read = (int) fsz - offset;
    }
    else
    {
      to_read = length;
    }

    p_buf = (uint8_t *) pvPortMalloc((size_t) to_read);
    if (!p_buf && (to_read > 0))
    {
        lfs_file_close(&g_rm_littlefs0_lfs, &p_file->lfs_file);
        return ERR_MEM;
    }

    if (to_read > 0)
    {
      rlen = lfs_file_read(&g_rm_littlefs0_lfs, &p_file->lfs_file, p_buf, (lfs_size_t) to_read);
    }
    else
    {
      rlen = 0;
    }

    if (rlen < 0)
    {
        if (p_buf)
        {
            vPortFree(p_buf);
        }

        lfs_file_close(&g_rm_littlefs0_lfs, &p_file->lfs_file);
        return rlen;
    }

    lfs_file_close(&g_rm_littlefs0_lfs, &p_file->lfs_file);
    p_file->len = (int)fsz;
    p_file->index = 0;
    p_file->data = p_buf;
    p_file->flags = 0;
    p_file->pextension = NULL;
    if (p_buf)
    {
        vPortFree(p_buf);
    }

    return ERR_OK;

}

/* Called when httpd wants more bytes */
int user_fs_read(struct lfs_file_data * p_file, char * p_buffer, int count)
{
  int r ;

  if (!p_file)
  {
      return -1;
  }

  r = lfs_file_read(&g_rm_littlefs0_lfs, &p_file->lfs_file, p_buffer, count);
  if (r > 0)
  {
    p_file->index += r;
  }

  return r;
}

/* Close the file */
void user_fs_close(struct lfs_file_data * p_file)
{
  if (!p_file)
  {
    return;
  }

  lfs_file_close(&g_rm_littlefs0_lfs, &p_file->lfs_file);
#if LWIP_HTTPD_FILE_STATE
  fs_state_free(p_file, p_file->state);
#endif
}

#endif  //SUPPORT_FSP_RM_FS_W

/*-----------------------------------------------------------------------------------*/
err_t
fs_open(struct fs_file *file, const char *name)
{
  const struct fsdata_file *f;

  if ((file == NULL) || (name == NULL)) {
    return ERR_ARG;
  }

#if LWIP_HTTPD_CUSTOM_FILES
  if (fs_open_custom(file, name)) {
    file->is_custom_file = 1;
    return ERR_OK;
  }
  file->is_custom_file = 0;
#endif /* LWIP_HTTPD_CUSTOM_FILES */

  for (f = FS_ROOT; f != NULL; f = f->next) {
    if (!strcmp(name, (const char *)f->name)) {
      file->data = (const char *)f->data;
      file->len = f->len;
      file->index = f->len;
      file->pextension = NULL;
      file->flags = f->flags;
#if HTTPD_PRECALCULATED_CHECKSUM
      file->chksum_count = f->chksum_count;
      file->chksum = f->chksum;
#endif /* HTTPD_PRECALCULATED_CHECKSUM */
#if LWIP_HTTPD_FILE_STATE
      file->state = fs_state_init(file, name);
#endif /* #if LWIP_HTTPD_FILE_STATE */
      return ERR_OK;
    }
  }
  /* file not found */
  return ERR_VAL;
}

/*-----------------------------------------------------------------------------------*/
void
fs_close(struct fs_file *file)
{
#if LWIP_HTTPD_CUSTOM_FILES
  if (file->is_custom_file) {
    fs_close_custom(file);
  }
#endif /* LWIP_HTTPD_CUSTOM_FILES */
#if LWIP_HTTPD_FILE_STATE
  fs_state_free(file, file->state);
#endif /* #if LWIP_HTTPD_FILE_STATE */
  LWIP_UNUSED_ARG(file);
}
/*-----------------------------------------------------------------------------------*/
#if LWIP_HTTPD_DYNAMIC_FILE_READ
#if LWIP_HTTPD_FS_ASYNC_READ
int
fs_read_async(struct fs_file *file, char *buffer, int count, fs_wait_cb callback_fn, void *callback_arg)
#else /* LWIP_HTTPD_FS_ASYNC_READ */
int
fs_read(struct fs_file *file, char *buffer, int count)
#endif /* LWIP_HTTPD_FS_ASYNC_READ */
{
  int read;
  if (file->index == file->len) {
    return FS_READ_EOF;
  }
#if LWIP_HTTPD_FS_ASYNC_READ
  LWIP_UNUSED_ARG(callback_fn);
  LWIP_UNUSED_ARG(callback_arg);
#endif /* LWIP_HTTPD_FS_ASYNC_READ */
#if LWIP_HTTPD_CUSTOM_FILES
  if (file->is_custom_file) {
#if LWIP_HTTPD_FS_ASYNC_READ
    return fs_read_async_custom(file, buffer, count, callback_fn, callback_arg);
#else /* LWIP_HTTPD_FS_ASYNC_READ */
    return fs_read_custom(file, buffer, count);
#endif /* LWIP_HTTPD_FS_ASYNC_READ */
  }
#endif /* LWIP_HTTPD_CUSTOM_FILES */

  read = file->len - file->index;
  if (read > count) {
    read = count;
  }

  MEMCPY(buffer, (file->data + file->index), read);
  file->index += read;

  return (read);
}
#endif /* LWIP_HTTPD_DYNAMIC_FILE_READ */
/*-----------------------------------------------------------------------------------*/
#if LWIP_HTTPD_FS_ASYNC_READ
int
fs_is_file_ready(struct fs_file *file, fs_wait_cb callback_fn, void *callback_arg)
{
  if (file != NULL) {
#if LWIP_HTTPD_FS_ASYNC_READ
#if LWIP_HTTPD_CUSTOM_FILES
    if (!fs_canread_custom(file)) {
      if (fs_wait_read_custom(file, callback_fn, callback_arg)) {
        return 0;
      }
    }
#else /* LWIP_HTTPD_CUSTOM_FILES */
    LWIP_UNUSED_ARG(callback_fn);
    LWIP_UNUSED_ARG(callback_arg);
#endif /* LWIP_HTTPD_CUSTOM_FILES */
#endif /* LWIP_HTTPD_FS_ASYNC_READ */
  }
  return 1;
}
#endif /* LWIP_HTTPD_FS_ASYNC_READ */
/*-----------------------------------------------------------------------------------*/
int
fs_bytes_left(struct fs_file *file)
{
  return file->len - file->index;
}
