/*

  silccommand.h

  Author: Pekka Riikonen <priikone@silcnet.org>

  Copyright (C) 1997 - 2005 Pekka Riikonen

  The contents of this file are subject to one of the Licenses specified 
  in the COPYING file;  You may not use this file except in compliance 
  with the License.

  The software distributed under the License is distributed on an "AS IS"
  basis, in the hope that it will be useful, but WITHOUT WARRANTY OF ANY
  KIND, either expressed or implied.  See the COPYING file for more
  information.

*/

/****h* silccore/SILC Command Interface
 *
 * DESCRIPTION
 *
 * Implementation of the Command Payload. The Command Payload is used to
 * send commands and also command replies usually between client and
 * server.
 *
 ***/

#ifndef SILCCOMMAND_H
#define SILCCOMMAND_H

/****f* silccore/SilcCommandAPI/SilcCommandCb
 *
 * SYNOPSIS
 *
 *    typedef void (*SilcCommandCb)(void *context, void *context2);
 *
 * DESCRIPTION
 *
 *    Command function callback. The actual command function pointer.
 *    This is generic command callback that the application may choose to
 *    use with its command routines.  However, none of the generic
 *    routines depend on this callback so application may freely define
 *    their own command callback if desired.
 *
 ***/
typedef void (*SilcCommandCb)(void *context, void *context2);

/****s* silccore/SilcCommandAPI/SilcCommandPayload
 *
 * NAME
 *
 *    typedef struct SilcCommandPayloadStruct *SilcCommandPayload;
 *
 * DESCRIPTION
 *
 *    This context is the actual Command Payload and is allocated
 *    by silc_command_payload_parse and given as argument usually to
 *    all silc_command_payload_* functions.  It is freed by the
 *    silc_command_payload_free function.
 *
 ***/
typedef struct SilcCommandPayloadStruct *SilcCommandPayload;

/****d* silccore/SilcCommandAPI/SilcCommandFlags
 *
 * NAME
 *
 *    typedef enum { ... } SilcCommandFlags;
 *
 * DESCRIPTION
 *
 *    Command flags that set how the commands behave on different
 *    situations. These can be OR'es together to set multiple flags.
 *    The application is resoponsible of implementing the behaviour
 *    of these flags. These are here just to define generic flags.
 *    The server usually makes use of these flags.
 *
 * SOURCE
 */
typedef enum {
  SILC_CF_NONE           = 0,

  /* Command may only be used once per (about) 2 seconds. Bursts up
     to 5 commands are allowed though. */
  SILC_CF_LAG            = (1L << 1),

  /* Command may only be used once per (about) 2 seconds. No bursts
     are allowed at all. */
  SILC_CF_LAG_STRICT     = (1L << 2),

  /* Command is available for registered connections (connections
     whose ID has been created. */
  SILC_CF_REG            = (1L << 3),

  /* Command is available only for server operators */
  SILC_CF_OPER           = (1L << 4),

  /* Command is available only for SILC (router) operators. If this
     is set SILC_CF_OPER is not necessary to be set. */
  SILC_CF_SILC_OPER      = (1L << 5),

} SilcCommandFlag;
/***/

/****d* silccore/SilcCommandAPI/SilcCommand
 *
 * NAME
 *
 *    typedef SilcUInt8 SilcCommand;
 *
 * DESCRIPTION
 *
 *    The SilcCommand type definition and the commands. The commands
 *    listed here are the official SILC Commands and they have client
 *    and server counterparts.
 *
 * SOURCE
 */
typedef SilcUInt8 SilcCommand;

/* All SILC commands. These are commands that have client and server
   counterparts. */
#define SILC_COMMAND_NONE               0
#define SILC_COMMAND_WHOIS		1
#define SILC_COMMAND_WHOWAS		2
#define SILC_COMMAND_IDENTIFY           3
#define SILC_COMMAND_NICK		4
#define SILC_COMMAND_LIST		5
#define SILC_COMMAND_TOPIC		6
#define SILC_COMMAND_INVITE		7
#define SILC_COMMAND_QUIT		8
#define SILC_COMMAND_KILL		9
#define SILC_COMMAND_INFO		10
#define SILC_COMMAND_STATS		11
#define SILC_COMMAND_PING		12
#define SILC_COMMAND_OPER		13
#define SILC_COMMAND_JOIN		14
#define SILC_COMMAND_MOTD		15
#define SILC_COMMAND_UMODE		16
#define SILC_COMMAND_CMODE		17
#define SILC_COMMAND_CUMODE		18
#define SILC_COMMAND_KICK		19
#define SILC_COMMAND_BAN		20
#define SILC_COMMAND_DETACH		21
#define SILC_COMMAND_WATCH		22
#define SILC_COMMAND_SILCOPER	        23
#define SILC_COMMAND_LEAVE		24
#define SILC_COMMAND_USERS		25
#define SILC_COMMAND_GETKEY		26
#define SILC_COMMAND_SERVICE		27

/* Private range start */
#define SILC_COMMAND_PRIVATE            200
#define SILC_COMMAND_PRIV_CONNECT       200
#define SILC_COMMAND_PRIV_CLOSE         201
#define SILC_COMMAND_PRIV_SHUTDOWN      202

/* Reserved */
#define SILC_COMMAND_RESERVED           255
/***/

/* Prototypes */

/****f* silccore/SilcCommandAPI/silc_command_payload_parse
 *
 * SYNOPSIS
 *
 *    SilcCommandPayload
 *    silc_command_payload_parse(const unsigned char *payload,
 *                               SilcUInt32 payload_len);
 *
 * DESCRIPTION
 *
 *    Parses command payload returning new command payload structure. The
 *    `buffer' is the raw payload.
 *
 ***/
SilcCommandPayload silc_command_payload_parse(const unsigned char *payload,
					      SilcUInt32 payload_len);

/****f* silccore/SilcCommandAPI/silc_command_payload_encode
 *
 * SYNOPSIS
 *
 *    SilcBuffer silc_command_payload_encode(SilcCommand cmd,
 *                                           SilcUInt32 argc,
 *                                           unsigned char **argv,
 *                                           SilcUInt32 *argv_lens,
 *                                           SilcUInt32 *argv_types,
 *                                           SilcUInt16 ident);
 *
 * DESCRIPTION
 *
 *     Encodes Command Payload returning it to SilcBuffer.
 *
 ***/
SilcBuffer silc_command_payload_encode(SilcCommand cmd,
				       SilcUInt32 argc,
				       unsigned char **argv,
				       SilcUInt32 *argv_lens,
				       SilcUInt32 *argv_types,
				       SilcUInt16 ident);

/****f* silccore/SilcCommandAPI/silc_command_payload_encode_payload
 *
 * SYNOPSIS
 *
 *    SilcBuffer
 *    silc_command_payload_encode_payload(SilcCommandPayload payload);
 *
 * DESCRIPTION
 *
 *    Same as silc_command_payload_encode but encodes the buffer from
 *    SilcCommandPayload structure instead of raw data.
 *
 ***/
SilcBuffer silc_command_payload_encode_payload(SilcCommandPayload payload);

/****f* silccore/SilcCommandAPI/silc_command_payload_encode_va
 *
 * SYNOPSIS
 *
 *    SilcBuffer silc_command_payload_encode_va(SilcCommand cmd,
 *                                              SilcUInt16 ident,
 *                                              SilcUInt32 argc, ...);
 *
 * DESCRIPTION
 *
 *    Encodes Command payload with variable argument list. The arguments
 *    must be: SilcUInt32, unsigned char *, unsigned int, ... One
 *    {SilcUInt32, unsigned char * and unsigned int} forms one argument,
 *    thus `argc' in case when sending one {SilcUInt32, unsigned char *
 *    and SilcUInt32} equals one (1) and when sending two of those it
 *    equals two (2), and so on. This has to be preserved or bad things
 *    will happen. The variable arguments is: {type, data, data_len}.
 *
 ***/
SilcBuffer silc_command_payload_encode_va(SilcCommand cmd,
					  SilcUInt16 ident,
					  SilcUInt32 argc, ...);

/****f* silccore/SilcCommandAPI/silc_command_payload_encode_vap
 *
 * SYNOPSIS
 *
 *    SilcBuffer silc_command_payload_encode_vap(SilcCommand cmd,
 *                                               SilcUInt16 ident,
 *                                               SilcUInt32 argc, va_list ap);
 *
 * DESCRIPTION
 *
 *    This is equivalent to the silc_command_payload_encode_va except
 *    takes the va_list as argument.
 *
 ***/
SilcBuffer silc_command_payload_encode_vap(SilcCommand cmd,
					   SilcUInt16 ident,
					   SilcUInt32 argc, va_list ap);

/****f* silccore/SilcCommandAPI/silc_command_reply_payload_encode_va
 *
 * SYNOPSIS
 *
 *    SilcBuffer
 *    silc_command_reply_payload_encode_va(SilcCommand cmd,
 *                                         SilcStatus status,
 *                                         SilcStatus error,
 *                                         SilcUInt16 ident,
 *                                         SilcUInt32 argc, ...);
 *
 * DESCRIPTION
 *
 *    Same as silc_command_payload_encode_va except that this is used to
 *    encode strictly command reply packets.  The `argc' must not count
 *    `status' and `error' as arguments.  The `status' includes the
 *    command reply status.  If single reply will be sent then it includes
 *    SILC_STATUS_OK if error did not occur.  It includes an error value
 *    if error did occur.  In this case `error' field is ignored.  If
 *    there will be multiple successful command replies then the `status'
 *    includes a list value and `error' is ignored.  If there will
 *    multiple error replies the `status' includes a list value, and
 *    the `error' includes an error value.  Thus, the `error' value is
 *    specified only if there will be list of errors.
 *
 * NOTES
 *
 *    Protocol defines that it is possible to send both list of successful
 *    and list of error replies at the same time, as long as the error
 *    replies are sent after the successful replies.
 *
 ***/
SilcBuffer
silc_command_reply_payload_encode_va(SilcCommand cmd,
				     SilcStatus status,
				     SilcStatus error,
				     SilcUInt16 ident,
				     SilcUInt32 argc, ...);

/****f* silccore/SilcCommandAPI/silc_command_reply_payload_encode_vap
 *
 * SYNOPSIS
 *
 *    SilcBuffer
 *    silc_command_reply_payload_encode_vap(SilcCommand cmd,
 *                                          SilcStatus status,
 *                                          SilcStatus error,
 *                                          SilcUInt16 ident, SilcUInt32 argc,
 *                                          va_list ap);
 *
 * DESCRIPTION
 *
 *    This is equivalent to the silc_command_reply_payload_encode_va except
 *    takes the va_list as argument.
 *
 ***/
SilcBuffer
silc_command_reply_payload_encode_vap(SilcCommand cmd,
				      SilcStatus status,
				      SilcStatus error,
				      SilcUInt16 ident, SilcUInt32 argc,
				      va_list ap);

/****f* silccore/SilcCommandAPI/silc_command_free
 *
 * SYNOPSIS
 *
 *    void silc_command_payload_free(SilcCommandPayload payload);
 *
 * DESCRIPTION
 *
 *    Frees the Command Payload and all data in it.
 *
 ***/
void silc_command_payload_free(SilcCommandPayload payload);

/****f* silccore/SilcCommandAPI/silc_command_get
 *
 * SYNOPSIS
 *
 *    SilcCommand silc_command_get(SilcCommandPayload payload);
 *
 * DESCRIPTION
 *
 *    Return the command from the payload.
 *
 ***/
SilcCommand silc_command_get(SilcCommandPayload payload);

/****f* silccore/SilcCommandAPI/silc_command_get_args
 *
 * SYNOPSIS
 *
 *    SilcArgumentPayload silc_command_get_args(SilcCommandPayload payload);
 *
 * DESCRIPTION
 *
 *    Return the Arguments Payload containing the arguments from the
 *    Command Payload. The caller must not free it.
 *
 ***/
SilcArgumentPayload silc_command_get_args(SilcCommandPayload payload);

/****f* silccore/SilcCommandAPI/silc_command_get_ident
 *
 * SYNOPSIS
 *
 *    SilcUInt16 silc_command_get_ident(SilcCommandPayload payload);
 *
 * DESCRIPTION
 *
 *    Return the command identifier from the payload. The identifier can
 *    be used to identify which command reply belongs to which command.
 *    The client sets the identifier to the payload and server must return
 *    the same identifier in the command reply.
 *
 ***/
SilcUInt16 silc_command_get_ident(SilcCommandPayload payload);

/****f* silccore/SilcCommandAPI/silc_command_get_status
 *
 * SYNOPSIS
 *
 *    bool silc_command_get_status(SilcCommandPayload payload,
 *                                 SilcStatus *status,
 *                                 SilcStatus *error);
 *
 * DESCRIPTION
 *
 *    This function returns the command reply status into `status' and
 *    error status, if error occurred into the `error'.  The function
 *    returns TRUE if command reply status is not error, and FALSE if
 *    error occurred.  In this case the `error' will include the actual
 *    error status.  The `status' can be in this case some list value
 *    which indicates that there will be list of errors.
 *
 ***/
bool silc_command_get_status(SilcCommandPayload payload,
			     SilcStatus *status,
			     SilcStatus *error);

/****f* silccore/SilcCommandAPI/silc_command_set_ident
 *
 * SYNOPSIS
 *
 *    void silc_command_set_ident(SilcCommandPayload payload,
 *                                SilcUInt16 ident);
 *
 * DESCRIPTION
 *
 *    Function to set identifier to already allocated Command Payload. Command
 *    payloads are frequentlly resent in SILC and thusly this makes it easy
 *    to set the identifier without encoding new Command Payload.
 *
 ***/
void silc_command_set_ident(SilcCommandPayload payload, SilcUInt16 ident);

/****f* silccore/SilcCommandAPI/silc_command_set_command
 *
 * SYNOPSIS
 *
 *    void silc_command_set_command(SilcCommandPayload payload,
 *                                  SilcCommand command);
 *
 * DESCRIPTION
 *
 *    Function to set the command to already allocated Command Payload. This
 *    makes it easy to change the command in the payload without encoding new
 *    Command Payload.
 *
 ***/
void silc_command_set_command(SilcCommandPayload payload, SilcCommand command);

#endif
