/*

  protocol.h

  Author: Pekka Riikonen <priikone@silcnet.org>

  Copyright (C) 1997 - 2004 Pekka Riikonen

  The contents of this file are subject to one of the Licenses specified 
  in the COPYING file;  You may not use this file except in compliance 
  with the License.

  The software distributed under the License is distributed on an "AS IS"
  basis, in the hope that it will be useful, but WITHOUT WARRANTY OF ANY
  KIND, either expressed or implied.  See the COPYING file for more
  information.

*/

#ifndef PROTOCOL_H
#define PROTOCOL_H

/* SILC client protocol types */
#define SILC_PROTOCOL_CLIENT_NONE               0
#define SILC_PROTOCOL_CLIENT_CONNECTION_AUTH    1
#define SILC_PROTOCOL_CLIENT_KEY_EXCHANGE       2
#define SILC_PROTOCOL_CLIENT_REKEY              3
/* #define SILC_PROTOCOL_CLIENT_MAX             255 */

/* Internal context for key exchange protocol */
typedef struct {
  void *client;
  SilcSocketConnection sock;
  SilcRng rng;
  int responder;

  void *dest_id;		    /* Destination ID from packet */
  SilcIdType dest_id_type;	    /* Destination ID type */

  SilcTask timeout_task;
  SilcPacketContext *packet;

  SilcSKESendPacketCb send_packet;  /* SKE's packet sending callback */
  SilcSKEVerifyCb verify;	    /* SKE's key verify callback */
  SilcSKE ske;			    /* The SKE object */
  SilcSKEKeyMaterial *keymat;	    /* The negotiated key material */
  void *context;		    /* Internal context */
} SilcClientKEInternalContext;

/* Internal context for connection authentication protocol */
typedef struct {
  void *client;
  SilcSocketConnection sock;
  SilcClientConnectionStatus status;

  /* SKE object from Key Exchange protocol. */
  SilcSKE ske;

  /* Auth method that must be used. This is resolved before this
     connection authentication protocol is started. */
  SilcProtocolAuthMeth auth_meth;

  /* Destinations ID from KE protocol context */
  void *dest_id;
  SilcIdType dest_id_type;

  /* Authentication data if we alreay know it. This is filled before
     starting the protocol if we know the authentication data. Otherwise
     these are and remain NULL. */
  unsigned char *auth_data;
  SilcUInt32 auth_data_len;

  SilcTask timeout_task;
} SilcClientConnAuthInternalContext;

/* Internal context for the rekey protocol */
typedef struct {
  void *client;
  void *context;
  SilcSocketConnection sock;
  bool responder;		    /* TRUE if we are receiving party */
  bool pfs;			    /* TRUE if PFS is to be used */
  SilcSKE ske;			    /* Defined if PFS is used */
  SilcPacketContext *packet;
} SilcClientRekeyInternalContext;

/* Prototypes */
void silc_client_protocols_register(void);
void silc_client_protocols_unregister(void);
void silc_client_protocol_ke_send_packet(SilcSKE ske,
					 SilcBuffer packet,
					 SilcPacketType type,
					 void *context);
void silc_client_protocol_ke_verify_key(SilcSKE ske,
					unsigned char *pk_data,
					SilcUInt32 pk_len,
					SilcSKEPKType pk_type,
					void *context,
					SilcSKEVerifyCbCompletion completion,
					void *completion_context);
void silc_client_protocol_ke_set_keys(SilcSKE ske,
				      SilcSocketConnection sock,
				      SilcSKEKeyMaterial *keymat,
				      SilcCipher cipher,
				      SilcPKCS pkcs,
				      SilcHash hash,
				      SilcHmac hmac,
				      SilcSKEDiffieHellmanGroup group,
				      bool is_responder);

#endif
