/*

  silcauth.h

  Author: Pekka Riikonen <priikone@silcnet.org>

  Copyright (C) 2001 - 2003 Pekka Riikonen

  The contents of this file are subject to one of the Licenses specified 
  in the COPYING file;  You may not use this file except in compliance 
  with the License.

  The software distributed under the License is distributed on an "AS IS"
  basis, in the hope that it will be useful, but WITHOUT WARRANTY OF ANY
  KIND, either expressed or implied.  See the COPYING file for more
  information.

*/

/****h* silccore/SILC Authentication Interface
 *
 * DESCRIPTION
 *
 * Implementations of the SILC Authentication Payload and authentication
 * routines.  The SILC Authentication Payload is used to deliver
 * authentication data usually from client to server in purpose of
 * gaining access to some service.  The Payload and the authentication
 * routines supports both passphrase and public key (signature) based
 * authentication.
 *
 * This interface defines also the SILC Key Agreement Payload that is
 * used by client to agree on key material usually with another client
 * in the network.
 *
 ***/

#ifndef SILCAUTH_H
#define SILCAUTH_H

/****d* silccore/SilcAuthAPI/SilcAuthMethod
 *
 * NAME
 *
 *    typedef SilcUInt16 SilcAuthMethod;
 *
 * DESCRIPTION
 *
 *    Authentication method type definition, the authentication methods
 *    and the authentication status'.  The status defines are used by
 *    all authentication protocols in the SILC.
 *
 * SOURCE
 */
typedef SilcUInt16 SilcAuthMethod;

#define SILC_AUTH_NONE        0	           /* No authentication */
#define SILC_AUTH_PASSWORD    1		   /* Passphrase authentication */
#define SILC_AUTH_PUBLIC_KEY  2		   /* Public key authentication */

/* Authentication protocol status message (used by all authentication
   protocols in the SILC). */
#define SILC_AUTH_OK          0
#define SILC_AUTH_FAILED      1
/***/

/****s* silccore/SilcAuthAPI/SilcAuthPayload
 *
 * NAME
 *
 *    typedef struct SilcAuthPayloadStruct *SilcAuthPayload;
 *
 *
 * DESCRIPTION
 *
 *    This context is the actual Authentication Payload and is allocated
 *    by silc_auth_payload_parse and given as argument usually to all
 *    silc_auth_payload_* functions.  It is freed by silc_auth_payload_free
 *    function.
 *
 ***/
typedef struct SilcAuthPayloadStruct *SilcAuthPayload;

/****f* silccore/SilcAuthAPI/silc_auth_payload_parse
 *
 * SYNOPSIS
 *
 *    SilcAuthPayload silc_auth_payload_parse(const unsigned char *data,
 *                                            SilcUInt32 data_len);
 *
 * DESCRIPTION
 *
 *    Parses and returns Authentication Payload.  The `data' and the
 *    `data_len' are the raw payload buffer.
 *
 ***/
SilcAuthPayload silc_auth_payload_parse(const unsigned char *data,
					SilcUInt32 data_len);

/****f* silccore/SilcAuthAPI/silc_auth_payload_encode
 *
 * SYNOPSIS
 *
 *    SilcBuffer silc_auth_payload_encode(SilcAuthMethod method,
 *                                        const unsigned char *random_data,
 *                                        SilcUInt16 random_len,
 *                                        const unsigned char *auth_data,
 *                                        SilcUInt16 auth_len);
 *
 * DESCRIPTION
 *
 *    Encodes authentication payload into buffer and returns it.
 *    The `random_data' is provided only if doing public key authentication.
 *    The `auth_data' is the actual authentication data.  If the
 *    `method' is SILC_AUTH_PASSWORD the passphase in `auth_data' sent as
 *    argument SHOULD be UTF-8 encoded, if not library will attempt to
 *    encode it.
 *
 ***/
SilcBuffer silc_auth_payload_encode(SilcAuthMethod method,
				    const unsigned char *random_data,
				    SilcUInt16 random_len,
				    const unsigned char *auth_data,
				    SilcUInt16 auth_len);

/****f* silccore/SilcAuthAPI/silc_auth_payload_free
 *
 * SYNOPSIS
 *
 *    void silc_auth_payload_free(SilcAuthPayload payload);
 *
 * DESCRIPTION
 *
 *    Frees authentication payload and all data in it.
 *
 ***/
void silc_auth_payload_free(SilcAuthPayload payload);

/****f* silccore/SilcAuthAPI/silc_auth_get_method
 *
 * SYNOPSIS
 *
 *    SilcAuthMethod silc_auth_get_method(SilcAuthPayload payload);
 *
 * DESCRIPTION
 *
 *    Get authentication method.
 *
 ***/
SilcAuthMethod silc_auth_get_method(SilcAuthPayload payload);

/****f* silccore/SilcAuthAPI/silc_auth_get_public_data
 *
 * SYNOPSIS
 *
 *    unsigned char *silc_auth_get_public_data(SilcAuthPayload payload,
 *                                             SilcUInt32 *pubdata_len);
 *
 * DESCRIPTION
 *
 *    Returns the public data (usually random data) from the payload.
 *    Caller must not free the returned data.
 *
 ***/
unsigned char *silc_auth_get_public_data(SilcAuthPayload payload,
					 SilcUInt32 *pubdata_len);

/****f* silccore/SilcAuthAPI/silc_auth_get_data
 *
 * SYNOPSIS
 *
 *    unsigned char *silc_auth_get_data(SilcAuthPayload payload,
 *                                      SilcUInt32 *auth_len);
 *
 * DESCRIPTION
 *
 *    Get the authentication data. The caller must not free the data.  If
 *    the authentication method is passphrase, then the returned string
 *    is UTF-8 encoded passphrase.
 *
 ***/
unsigned char *silc_auth_get_data(SilcAuthPayload payload,
				  SilcUInt32 *auth_len);

/****f* silccore/SilcAuthAPI/silc_auth_public_key_auth_generate
 *
 * SYNOPSIS
 *
 *    SilcBuffer silc_auth_public_key_auth_generate(SilcPublicKey public_key,
 *                                                  SilcPrivateKey private_key,
 *                                                  SilcRng rng,
 *                                                  SilcHash hash,
 *                                                  const void *id,
 *                                                  SilcIdType type);
 *
 * DESCRIPTION
 *
 *    Generates Authentication Payload with authentication data. This is used
 *    to do public key based authentication. This generates the random data
 *    and the actual authentication data. Returns NULL on error and the
 *    encoded Authentication Payload on success.
 *
 *    The `private_key' is used to sign the payload.  The `public_key', the
 *    and the `id' is encoded in the payload and signed.  If the `rng' is
 *    NULL then global RNG is used, if non-NULL then `rng' is used as
 *    random number generator.  Also random number is encoded in the
 *    payload before signing it with `private_key'.
 *
 ***/
SilcBuffer silc_auth_public_key_auth_generate(SilcPublicKey public_key,
					      SilcPrivateKey private_key,
					      SilcRng rng, SilcHash hash,
					      const void *id, SilcIdType type);

/****f* silccore/SilcAuthAPI/silc_auth_public_key_auth_generate_wpub
 *
 * SYNOPSIS
 *
 *    SilcBuffer
 *    silc_auth_public_key_auth_generate_wpub(SilcPublicKey public_key,
 *                                            SilcPrivateKey private_key,
 *                                            const unsigned char *pubdata,
 *                                            SilcUInt32 pubdata_len,
 *                                            SilcHash hash,
 *                                            const void *id,
 *                                            SilcIdType type);
 *
 * DESCRIPTION
 *
 *    Same as silc_auth_public_key_auth_generate but takes the public data
 *    (usually random data) as argument.  This function can be used when
 *    the public data must be something else than purely random or its
 *    structure mut be set before signing.
 *
 ***/
SilcBuffer
silc_auth_public_key_auth_generate_wpub(SilcPublicKey public_key,
					SilcPrivateKey private_key,
					const unsigned char *pubdata,
					SilcUInt32 pubdata_len,
					SilcHash hash,
					const void *id, SilcIdType type);

/****f* silccore/SilcAuthAPI/silc_auth_public_key_auth_verify
 *
 * SYNOPSIS
 *
 *    bool silc_auth_public_key_auth_verify(SilcAuthPayload payload,
 *                                          SilcPublicKey public_key,
 *                                          SilcHash hash,
 *                                          const void *id, SilcIdType type);
 *
 * DESCRIPTION
 *
 *    Verifies the authentication data. Returns TRUE if authentication was
 *    successful.
 *
 ***/
bool silc_auth_public_key_auth_verify(SilcAuthPayload payload,
				      SilcPublicKey public_key, SilcHash hash,
				      const void *id, SilcIdType type);

/****f* silccore/SilcAuthAPI/silc_auth_public_key_auth_verify_data
 *
 * SYNOPSIS
 *
 *    bool silc_auth_public_key_auth_verify_data(const unsigned char *payload,
 *                                               SilcUInt32 payload_len,
 *                                               SilcPublicKey public_key,
 *                                               SilcHash hash,
 *                                               const void *id,
 *                                               SilcIdType type);
 *
 * DESCRIPTION
 *
 *    Same as silc_auth_public_key_auth_verify but the payload has not
 *    been parsed yet.  This will parse it.  Returns TRUE if authentication
 *    was successful.
 *
 ***/
bool silc_auth_public_key_auth_verify_data(const unsigned char *payload,
					   SilcUInt32 payload_len,
					   SilcPublicKey public_key,
					   SilcHash hash,
					   const void *id, SilcIdType type);

/****f* silccore/SilcAuthAPI/silc_auth_verify
 *
 * SYNOPSIS
 *
 *    bool silc_auth_verify(SilcAuthPayload payload,
 *                          SilcAuthMethod auth_method,
 *                          const void *auth_data, SilcUInt32 auth_data_len,
 *                          SilcHash hash, const void *id, SilcIdType type);
 *
 * DESCRIPTION
 *
 *    Verifies the authentication data directly from the Authentication
 *    Payload. Supports all authentication methods. If the authentication
 *    method is passphrase based then the `auth_data' and `auth_data_len'
 *    are the passphrase and its length.  The passphrase MUST be UTF-8
 *    encoded.  If the method is public key authentication then the
 *    `auth_data' is the SilcPublicKey and the `auth_data_len' is ignored.
 *
 ***/
bool silc_auth_verify(SilcAuthPayload payload, SilcAuthMethod auth_method,
		      const void *auth_data, SilcUInt32 auth_data_len,
		      SilcHash hash, const void *id, SilcIdType type);

/****f* silccore/SilcAuthAPI/silc_auth_verify_data
 *
 * SYNOPSIS
 *
 *    bool silc_auth_verify_data(const unsigned char *payload,
 *                               SilcUInt32 payload_len,
 *                               SilcAuthMethod auth_method,
 *                               const void *auth_data,
 *                               SilcUInt32 auth_data_len, SilcHash hash,
 *                               const void *id, SilcIdType type);
 *
 * DESCRIPTION
 *
 *    Same as silc_auth_verify but the payload has not been parsed yet.
 *    Verifies the authentication data directly from the Authentication
 *    Payload. Supports all authentication methods. If the authentication
 *    method is passphrase based then the `auth_data' and `auth_data_len'
 *    are the passphrase and its length.  The passphrase MUST be UTF-8
 *    encoded.  If the method is public key authentication then the
 *    `auth_data' is the SilcPublicKey and the `auth_data_len' is ignored.
 *
 ***/
bool silc_auth_verify_data(const unsigned char *payload,
			   SilcUInt32 payload_len,
			   SilcAuthMethod auth_method, const void *auth_data,
			   SilcUInt32 auth_data_len, SilcHash hash,
			   const void *id, SilcIdType type);

/****s* silccore/SilcAuthAPI/SilcKeyAgreementPayload
 *
 * NAME
 *
 *    typedef struct SilcKeyAgreementPayloadStruct *SilcKeyAgreementPayload;
 *
 * DESCRIPTION
 *
 *    This context is the actual Key Agreement Payload and is allocated
 *    by silc_key_agreement_payload_parse and given as argument usually to all
 *    silc_key_agreement_* functions.  It is freed by the function
 *    silc_key_agreement_payload_free.
 *
 ***/
typedef struct SilcKeyAgreementPayloadStruct *SilcKeyAgreementPayload;

/****f* silccore/SilcAuthAPI/silc_key_agreement_payload_parse
 *
 * SYNOPSIS
 *
 *    SilcKeyAgreementPayload
 *    silc_key_agreement_payload_parse(const unsigned char *payload,
 *                                     SilcUInt32 payload_len);
 *
 * DESCRIPTION
 *
 *    Parses and returns an allocated Key Agreement payload.
 *
 ***/
SilcKeyAgreementPayload
silc_key_agreement_payload_parse(const unsigned char *payload,
				 SilcUInt32 payload_len);

/****f* silccore/SilcAuthAPI/silc_key_agreement_payload_encode
 *
 * SYNOPSIS
 *
 *    SilcBuffer silc_key_agreement_payload_encode(char *hostname,
 *                                                 SilcUInt32 port);
 *
 * DESCRIPTION
 *
 *    Encodes the Key Agreement protocol and returns the encoded buffer
 *
 ***/
SilcBuffer silc_key_agreement_payload_encode(const char *hostname,
					     SilcUInt32 port);

/****f* silccore/SilcAuthAPI/silc_key_agreement_payload_free
 *
 * SYNOPSIS
 *
 *    void silc_key_agreement_payload_free(SilcKeyAgreementPayload payload);
 *
 * DESCRIPTION
 *
 *    Frees the Key Agreement protocol and all data in it.
 *
 ***/
void silc_key_agreement_payload_free(SilcKeyAgreementPayload payload);

/****f* silccore/SilcAuthAPI/silc_key_agreement_get_hostname
 *
 * SYNOPSIS
 *
 *    char *silc_key_agreement_get_hostname(SilcKeyAgreementPayload payload);
 *
 * DESCRIPTION
 *
 *    Returns the hostname in the payload. Caller must not free it.
 *    The hostname is the host that is able to accept key negotiation
 *    using the SILC Key Exchange protocol.
 *
 ***/
char *silc_key_agreement_get_hostname(SilcKeyAgreementPayload payload);

/****f* silccore/SilcAuthAPI/silc_key_agreement_get_port
 *
 * SYNOPSIS
 *
 *    SilcUInt32 silc_key_agreement_get_port(SilcKeyAgreementPayload payload);
 *
 * DESCRIPTION
 *
 *    Returns the port in the payload.  The port is the port on the
 *    host returned by silc_key_agreement_get_hostname that is running
 *    the SILC Key Exchange protocol.
 *
 ***/
SilcUInt32 silc_key_agreement_get_port(SilcKeyAgreementPayload payload);

#endif
