/*

  silcrng.h

  Author: Pekka Riikonen <priikone@silcnet.org>

  Copyright (C) 1997 - 2003 Pekka Riikonen

  The contents of this file are subject to one of the Licenses specified 
  in the COPYING file;  You may not use this file except in compliance 
  with the License.

  The software distributed under the License is distributed on an "AS IS"
  basis, in the hope that it will be useful, but WITHOUT WARRANTY OF ANY
  KIND, either expressed or implied.  See the COPYING file for more
  information.

*/

/****h* silccrypt/SILC RNG Interface
 *
 * DESCRIPTION
 *
 * SILC Random Number Generator is cryptographically strong pseudo random
 * number generator. It is used to generate all the random numbers needed
 * in the SILC sessions. All key material and other sources needing random
 * numbers use this generator.
 *
 * The interface provides functions for retrieving different size of
 * random number and arbitrary length of random data buffers. The interface
 * also defines Global RNG API which makes it possible to call any
 * RNG API function without specific RNG context.
 *
 ***/

#ifndef SILCRNG_H
#define SILCRNG_H

/****s* silccrypt/SilcRNGAPI/SilcRng
 *
 * NAME
 *
 *    typedef struct SilcRngStruct *SilcRng;
 *
 * DESCRIPTION
 *
 *    This context is the actual Random Number Generator and is allocated
 *    by silc_rng_alloc and given as argument usually to all silc_rng_*
 *    functions.  It is freed by the silc_rng_free function.  The RNG is
 *    initialized by calling the silc_rng_init function.
 *
 ***/
typedef struct SilcRngStruct *SilcRng;

/* Prototypes */

/****f* silccrypt/SilcRNGAPI/silc_rng_alloc
 *
 * SYNOPSIS
 *
 *    SilcRng silc_rng_alloc(void);
 *
 * DESCRIPTION
 *
 *    Allocates new SILC random number generator and returns context to
 *    it.  After the RNG is allocated it must be initialized by calling
 *    silc_rng_init before it actually can be used to produce any random
 *    number.  This function returns NULL if RNG could not allocated.
 *
 ***/
SilcRng silc_rng_alloc(void);

/****f* silccrypt/SilcRNGAPI/silc_rng_free
 *
 * SYNOPSIS
 *
 *    void silc_rng_free(SilcRng rng);
 *
 * DESCRIPTION
 *
 *    Frees the random number generator and destroys the random number
 *    pool.
 *
 ***/
void silc_rng_free(SilcRng rng);

/****f* silccrypt/SilcRNGAPI/silc_rng_init
 *
 * SYNOPSIS
 *
 *    void silc_rng_init(SilcRng rng);
 *
 * DESCRIPTION
 *
 *    This function is used to initialize the random number generator.
 *    This is the function that must be called after the RNG is allocated
 *    by calling silc_rng_alloc.  RNG cannot be used before this function
 *    is called.
 *
 * NOTES
 *
 *    This function may be slow since it will acquire secret noise from
 *    the environment in an attempt to set the RNG in unguessable state.
 *
 ***/
void silc_rng_init(SilcRng rng);

/****f* silccrypt/SilcRNGAPI/silc_rng_get_byte
 *
 * SYNOPSIS
 *
 *    SilcUInt8 silc_rng_get_byte(SilcRng rng);
 *
 * DESCRIPTION
 *
 *    Returns one 8-bit random byte from the random number generator.
 *
 ***/
SilcUInt8 silc_rng_get_byte(SilcRng rng);

/****f* silccrypt/SilcRNGAPI/silc_rng_get_byte_fast
 *
 * SYNOPSIS
 *
 *    SilcUInt8 silc_rng_get_byte_fast(SilcRng rng);
 *
 * DESCRIPTION
 *
 *    Returns one 8-bit random byte from the random number generator as
 *    fast as possible.
 *
 * NOTES
 *
 *    This will read the data from /dev/urandom if it is available in the
 *    operating system, since this may be faster than retrieving a byte
 *    from the SILC RNG.  If /dev/urandom is not available this will take
 *    the byte from SILC RNG and is effectively same as silc_rng_get_byte.
 *
 ***/
SilcUInt8 silc_rng_get_byte_fast(SilcRng rng);

/****f* silccrypt/SilcRNGAPI/silc_rng_get_rn16
 *
 * SYNOPSIS
 *
 *    SilcUInt16 silc_rng_get_rn16(SilcRng rng);
 *
 * DESCRIPTION
 *
 *    Returns one 16-bit random number from the random number generator.
 *
 ***/
SilcUInt16 silc_rng_get_rn16(SilcRng rng);

/****f* silccrypt/SilcRNGAPI/silc_rng_get_rn32
 *
 * SYNOPSIS
 *
 *    SilcUInt32 silc_rng_get_rn32(SilcRng rng);
 *
 * DESCRIPTION
 *
 *    Returns one 32-bit random number from the random number generator.
 *
 ***/
SilcUInt32 silc_rng_get_rn32(SilcRng rng);

/****f* silccrypt/SilcRNGAPI/silc_rng_get_rn_string
 *
 * SYNOPSIS
 *
 *    unsigned char *silc_rng_get_rn_string(SilcRng rng, SilcUInt32 len);
 *
 * DESCRIPTION
 *
 *    Returns random string in HEX form of the length of `len' bytes.
 *    The caller must free returned data buffer.  It is guaranteed the
 *    data string goes not include any zero (0x00) bytes.
 *
 ***/
unsigned char *silc_rng_get_rn_string(SilcRng rng, SilcUInt32 len);

/****f* silccrypt/SilcRNGAPI/silc_rng_get_rn_data
 *
 * SYNOPSIS
 *
 *    unsigned char *silc_rng_get_rn_data(SilcRng rng, SilcUInt32 len);
 *
 * DESCRIPTION
 *
 *    Returns random binary data of the length of `len' bytes.  The
 *    caller must free returned data buffer.  It is guaranteed the data
 *    buffer does not include any zero (0x00) bytes.
 *
 ***/
unsigned char *silc_rng_get_rn_data(SilcRng rng, SilcUInt32 len);

/****f* silccrypt/SilcRNGAPI/silc_rng_add_noise
 *
 * SYNOPSIS
 *
 *    void silc_rng_add_noise(SilcRng rng, unsigned char *buffer, SilcUInt32 len);
 *
 * DESCRIPTION
 *
 *    Add the data buffer indicated by `buffer' of length of `len' bytes
 *    as noise to the random number generator.  The random number generator
 *    is restirred (reseeded) when this function is called.
 *
 ***/
void silc_rng_add_noise(SilcRng rng, unsigned char *buffer, SilcUInt32 len);

/****f* silccrypt/SilcRNGAPI/silc_rng_global_init
 *
 * SYNOPSIS
 *
 *    bool silc_rng_global_init(SilcRng rng);
 *
 * DESCRIPTION
 *
 *    This function sets the `rng' if non-NULL as global RNG context.
 *    When any of the silc_rng_global_* functions is called the `rng' is
 *    used as RNG.  If `rng' is NULL this will allocate new RNG as global
 *    RNG.  The application in this case must free it later by calling
 *    silc_rng_global_uninit.  Returns TRUE after Global RNG is initialized.
 *
 * NOTES
 *
 *    If `rng' was non-NULL, the silc_rng_init must have been called for
 *    the `rng' already.
 *
 *    This function can be used to define the `rng' as global RNG and then
 *    use silc_rng_global_* functions easily without need to provide
 *    the RNG as argument.
 *
 ***/
bool silc_rng_global_init(SilcRng rng);

/****f* silccrypt/SilcRNGAPI/silc_rng_global_uninit
 *
 * SYNOPSIS
 *
 *    bool silc_rng_global_uninit(void);
 *
 * DESCRIPTION
 *
 *    Uninitialized the Global RNG object and frees it.  This should not
 *    be called if silc_rng_global_init was called with non-NULL RNG.
 *
 ***/
bool silc_rng_global_uninit(void);

/****f* silccrypt/SilcRNGAPI/silc_rng_global_get_byte
 *
 * SYNOPSIS
 *
 *    SilcUInt8 silc_rng_global_get_byte(void);
 *
 * DESCRIPTION
 *
 *    Returns one 8-bit random byte from the random number generator.
 *
 ***/
SilcUInt8 silc_rng_global_get_byte(void);

/****f* silccrypt/SilcRNGAPI/silc_rng_global_get_byte_fast
 *
 * SYNOPSIS
 *
 *    SilcUInt8 silc_rng_global_get_byte_fast(void);
 *
 * DESCRIPTION
 *
 *    Returns one 8-bit random byte from the random number generator as
 *    fast as possible.
 *
 * NOTES
 *
 *    This will read the data from /dev/urandom if it is available in the
 *    operating system, since this may be faster than retrieving a byte
 *    from the SILC RNG.  If /dev/urandom is not available this will take
 *    the byte from SILC RNG and is effectively same as silc_rng_get_byte.
 *
 ***/
SilcUInt8 silc_rng_global_get_byte_fast(void);

/****f* silccrypt/SilcRNGAPI/silc_rng_global_get_rn16
 *
 * SYNOPSIS
 *
 *    SilcUInt16 silc_rng_global_get_rn16(void);
 *
 * DESCRIPTION
 *
 *    Returns one 16-bit random number from the random number generator.
 *
 ***/
SilcUInt16 silc_rng_global_get_rn16(void);

/****f* silccrypt/SilcRNGAPI/silc_rng_global_get_rn32
 *
 * SYNOPSIS
 *
 *    SilcUInt32 silc_rng_global_get_rn32(void);
 *
 * DESCRIPTION
 *
 *    Returns one 32-bit random number from the random number generator.
 *
 ***/
SilcUInt32 silc_rng_global_get_rn32(void);

/****f* silccrypt/SilcRNGAPI/silc_rng_global_get_rn_string
 *
 * SYNOPSIS
 *
 *    unsigned char *silc_rng_global_get_rn_string(SilcUInt32 len);
 *
 * DESCRIPTION
 *
 *    Returns random string in HEX form of the length of `len' bytes.
 *    The caller must free returned data buffer.  It is guaranteed the
 *    data string goes not include any zero (0x00) bytes.
 *
 ***/
unsigned char *silc_rng_global_get_rn_string(SilcUInt32 len);

/****f* silccrypt/SilcRNGAPI/silc_rng_global_get_rn_data
 *
 * SYNOPSIS
 *
 *    unsigned char *silc_rng_global_get_rn_data(SilcUInt32 len);
 *
 * DESCRIPTION
 *
 *    Returns random binary data of the length of `len' bytes.  The
 *    caller must free returned data buffer.  It is guaranteed the data
 *    buffer does not include any zero (0x00) bytes.
 *
 ***/
unsigned char *silc_rng_global_get_rn_data(SilcUInt32 len);

/****f* silccrypt/SilcRNGAPI/silc_rng_global_add_noise
 *
 * SYNOPSIS
 *
 *    void silc_rng_global_add_noise(unsigned char *buffer, SilcUInt32 len);
 *
 * DESCRIPTION
 *
 *    Add the data buffer indicated by `buffer' of length of `len' bytes
 *    as noise to the random number generator.  The random number generator
 *    is restirred (reseeded) when this function is called.
 *
 ***/

void silc_rng_global_add_noise(unsigned char *buffer, SilcUInt32 len);

#endif
