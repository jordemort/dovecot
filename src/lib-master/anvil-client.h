#ifndef ANVIL_CLIENT_H
#define ANVIL_CLIENT_H

enum anvil_client_flags {
	/* if connect() fails with ENOENT, hide the error */
	ANVIL_CLIENT_FLAG_HIDE_ENOENT	= 0x01
};

/* reply=NULL if query failed */
typedef void anvil_callback_t(const char *reply, void *context);

/* If reconnect_callback is specified, it's called when connection is lost.
   If the callback returns FALSE, reconnection isn't attempted. */
struct anvil_client *
anvil_client_init(const char *path, bool (*reconnect_callback)(void),
		  enum anvil_client_flags flags) ATTR_NULL(2);
void anvil_client_deinit(struct anvil_client **client);

/* Connect to anvil. If retry=TRUE, try connecting for a while */
int anvil_client_connect(struct anvil_client *client, bool retry);

/* Send a query to anvil, expect a one line reply. The returned pointer can be
   used to abort the query later. It becomes invalid when callback is
   called (= the callback must not call it). Returns NULL if the query couldn't
   be sent. */
struct anvil_query *
anvil_client_query(struct anvil_client *client, const char *query,
		   anvil_callback_t *callback, void *context);
void anvil_client_query_abort(struct anvil_client *client,
			      struct anvil_query **query);
/* Send a command to anvil, don't expect any replies. */
void anvil_client_cmd(struct anvil_client *client, const char *cmd);

/* Returns TRUE if anvil is connected to. */
bool anvil_client_is_connected(struct anvil_client *client);

#endif
