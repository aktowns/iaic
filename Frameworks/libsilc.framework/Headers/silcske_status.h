/*

  silcske_status.h 

  Author: Pekka Riikonen <priikone@silcnet.org>

  Copyright (C) 2000 - 2002 Pekka Riikonen

  The contents of this file are subject to one of the Licenses specified 
  in the COPYING file;  You may not use this file except in compliance 
  with the License.

  The software distributed under the License is distributed on an "AS IS"
  basis, in the hope that it will be useful, but WITHOUT WARRANTY OF ANY
  KIND, either expressed or implied.  See the COPYING file for more
  information.

*/

/****h* silcske/SKE Status Types
 *
 * DESCRIPTION
 *
 * Defines the SilcSKEStatus type, that is returned by all SKE routines,
 * and a utility interface for handling the status.
 *
 ***/

#ifndef SILCSKE_STATUS_H
#define SILCSKE_STATUS_H

/****d* silcske/SilcSKEStatuses/SilcSKEStatus
 *
 * NAME
 * 
 *    typedef enum { ... } SilcSKEStatus;
 *
 * DESCRIPTION
 *
 *    Status types returned by all SKE routines. This tell the status of
 *    the SKE session, and if an error occurred. 
 *
 * SOURCE
 */
typedef enum {
  /* These are defined by the protocol */
  SILC_SKE_STATUS_OK                     = 0,  /* No error */
  SILC_SKE_STATUS_ERROR                  = 1,  /* Unknown error */
  SILC_SKE_STATUS_BAD_PAYLOAD            = 2,  /* Malformed payload */
  SILC_SKE_STATUS_UNKNOWN_GROUP          = 3,  /* Unsupported DH group */
  SILC_SKE_STATUS_UNKNOWN_CIPHER         = 4,  /* Unsupported cipher */
  SILC_SKE_STATUS_UNKNOWN_PKCS           = 5,  /* Unsupported PKCS algorithm */
  SILC_SKE_STATUS_UNKNOWN_HASH_FUNCTION  = 6,  /* Unsupported hash function */
  SILC_SKE_STATUS_UNKNOWN_HMAC           = 7,  /* Unsupported HMAC */
  SILC_SKE_STATUS_UNSUPPORTED_PUBLIC_KEY = 8,  /* Unsupported/not trusted PK */
  SILC_SKE_STATUS_INCORRECT_SIGNATURE    = 9,  /* Incorrect signature */
  SILC_SKE_STATUS_BAD_VERSION            = 10, /* Unsupported version */
  SILC_SKE_STATUS_INVALID_COOKIE         = 11, /* Cookie was modified */

  /* Implementation specific status types */
  SILC_SKE_STATUS_PENDING,		       /* SKE library is pending */
  SILC_SKE_STATUS_PUBLIC_KEY_NOT_PROVIDED,     /* Remote did not send PK */
  SILC_SKE_STATUS_KEY_EXCHANGE_NOT_ACTIVE,     /* SKE is not started */
  SILC_SKE_STATUS_BAD_RESERVED_FIELD,	       /* Reserved field was not 0 */
  SILC_SKE_STATUS_BAD_PAYLOAD_LENGTH,	       /* Payload includes garbage */
  SILC_SKE_STATUS_SIGNATURE_ERROR,	       /* Error computing signature */
  SILC_SKE_STATUS_OUT_OF_MEMORY,	       /* System out of memory */

  /* Other internal status types */
  SILC_SKE_STATUS_FREED,		       /* Internal library status */
} SilcSKEStatus;
/***/

/****f* silcske/SilcSKEStatuses/silc_ske_map_status
 *
 * SYNOPSIS
 *
 *    const char *silc_ske_map_status(SilcSKEStatus status);
 *
 * DESCRIPTION
 *
 *    Utility function to map the `status' into human readable message.
 *
 ***/
const char *silc_ske_map_status(SilcSKEStatus status);

#endif /* SILCSKE_STATUS_H */
