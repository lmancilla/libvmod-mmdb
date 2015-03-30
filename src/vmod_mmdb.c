#include <stdio.h>
#include <stdlib.h>

#include "vrt.h"
#include "cache/cache.h"

#include "vcc_if.h"

#include <maxminddb.h>

int
init_function(struct vmod_priv *priv, const struct VCL_conf *conf)
{
	return (0);
}

void
freeit(void *data)
{
  MMDB_close(data);
  free(data);
}

VCL_VOID
vmod_load_db(const struct vrt_ctx *ctx, struct vmod_priv *priv, VCL_STRING filename)
{
  priv->priv = (MMDB_s *)calloc(1, sizeof(MMDB_s));
  if (priv->priv == NULL)
    return;

  if(MMDB_open(filename, MMDB_MODE_MMAP, priv->priv) != MMDB_SUCCESS) {
    free(priv->priv);
    return;
  }
  priv->free = freeit;
}

VCL_STRING
vmod_get_code(const struct vrt_ctx *ctx, struct vmod_priv *priv, const struct suckaddr *ip)
{
  const char *lookup_path[] = {"country", "iso_code", NULL};

  char *data = NULL;
  int gai_error, mmdb_error, status;

  // cast IP to STRING (VCL TYPES)
  char *ip_str = VRT_IP_string(ctx, ip);

  MMDB_lookup_result_s result = MMDB_lookup_string(priv->priv, ip_str, &gai_error, &mmdb_error);

  if (gai_error != 0)
    return 0;

  MMDB_entry_data_s entry_data;
  
  if (!result.found_entry)
    return 0;  

  status = MMDB_aget_value(&result.entry, &entry_data, lookup_path);

  if (MMDB_SUCCESS != status || !entry_data.has_data || entry_data.type != MMDB_DATA_TYPE_UTF8_STRING)
    return 0;

  data = strndup(entry_data.utf8_string, entry_data.data_size);
  
  return data;
}
