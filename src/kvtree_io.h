/*
* Copyright (c) 2018, Lawrence Livermore National Security, LLC.
* Produced at the Lawrence Livermore National Laboratory.
*
* Copyright (c) 2018, UChicago Argonne LLC,
*   operator of Argonne National Laboratory
*
* LLNL-CODE-745961
* All rights reserved.
*
* This is the license for KVTree.
* For details, see https://github.com/LLNL/KVTree
* Please read the LICENSE file for full license text.
*/

#ifndef KVTREE_IO_H
#define KVTREE_IO_H

#include <config.h>
#include <stdarg.h>
#include <sys/types.h>

/* compute crc32 */
#include <zlib.h>

#ifndef KVTREE_MAX_LINE
#define KVTREE_MAX_LINE (1024)
#endif

/* adds byte swapping routines */
#if defined(__APPLE__)
#include "machine/endian.h"
#else
#include "endian.h"
#endif

#ifdef HAVE_BYTESWAP_H
#include "byteswap.h"
#else
#include "kvtree_byteswap.h"
#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN
#ifdef HAVE_BYTESWAP_H
# define kvtree_ntoh16(x) bswap_16(x)
# define kvtree_ntoh32(x) bswap_32(x)
# define kvtree_ntoh64(x) bswap_64(x)
# define kvtree_hton16(x) bswap_16(x)
# define kvtree_hton32(x) bswap_32(x)
# define kvtree_hton64(x) bswap_64(x)
#else
# define kvtree_ntoh16(x) kvtree_bswap_16(x)
# define kvtree_ntoh32(x) kvtree_bswap_32(x)
# define kvtree_ntoh64(x) kvtree_bswap_64(x)
# define kvtree_hton16(x) kvtree_bswap_16(x)
# define kvtree_hton32(x) kvtree_bswap_32(x)
# define kvtree_hton64(x) kvtree_bswap_64(x)
#endif
#else
# define kvtree_ntoh16(x) (x)
# define kvtree_ntoh32(x) (x)
# define kvtree_ntoh64(x) (x)
# define kvtree_hton16(x) (x)
# define kvtree_hton32(x) (x)
# define kvtree_hton64(x) (x)
#endif

#define KVTREE_OPEN_TRIES (5)
#define KVTREE_OPEN_USLEEP (100)

/*
=========================================
Basic File I/O
=========================================
*/

/* returns user's current mode as determine by his umask */
mode_t kvtree_getmode(int read, int write, int execute);

/* open file with specified flags and mode, retry open a few times on failure */
int kvtree_open(const char* file, int flags, ...);

/* close file with an fsync */
int kvtree_close(const char* file, int fd);

/* get and release file locks */
int kvtree_file_lock_write(const char* file, int fd);
int kvtree_file_unlock(const char* file, int fd);

/* opens specified file and waits on for an exclusive lock before returning the file descriptor */
int kvtree_open_with_lock(const char* file, int flags, mode_t mode);

/* unlocks the specified file descriptor and then closes the file */
int kvtree_close_with_unlock(const char* file, int fd);

/* make a good attempt to read from file (retries, if necessary, return error if fail) */
ssize_t kvtree_read_attempt(const char* file, int fd, void* buf, size_t size);

/* make a good attempt to write to file (retries, if necessary, return error if fail) */
ssize_t kvtree_write_attempt(const char* file, int fd, const void* buf, size_t size);

/* tests whether the file or directory is readable */
int kvtree_file_is_readable(const char* file);

#endif
