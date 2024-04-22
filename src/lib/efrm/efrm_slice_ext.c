/* SPDX-License-Identifier: GPL-2.0 */
/* X-SPDX-Copyright-Text: (c) Copyright 2020 Xilinx, Inc. */
#include <ci/efrm/private.h>
#include <ci/efrm/slice_ext.h>
#include <ci/driver/resource/linux_efhw_nic.h>
#include <ci/efhw/ef100.h>
#include "efrm_internal.h"

#if ! CI_HAVE_SFC
static int _dummy_ef100_nic_ext(struct efhw_nic *nic, ...)
{
	return -EOPNOTSUPP;
}

#define ef100_nic_ext_alloc _dummy_ef100_nic_ext
#define ef100_nic_ext_free _dummy_ef100_nic_ext
#define ef100_nic_ext_get_meta_global _dummy_ef100_nic_ext
#define ef100_nic_ext_get_meta_msg _dummy_ef100_nic_ext
#define ef100_nic_ext_msg _dummy_ef100_nic_ext
#endif /* ! CI_HAVE_SFC */


static bool check_ef100(const struct efrm_resource *rs)
{
	if (rs->rs_client->nic->devtype.arch != EFHW_ARCH_EF100) {
		EFRM_TRACE("%s: Only EF100 NIC supports slice extensions."
		           " Expected arch=%d but got %d\n", __FUNCTION__,
		           EFHW_ARCH_EF100, rs->rs_client->nic->devtype.arch);
		return false;
	}
	return true;
}


int efrm_ext_alloc(struct efrm_resource *rs,
                   const unsigned char* service_guid, uint32_t* out_mc_id)
{
	if (!check_ef100(rs))
		return -EOPNOTSUPP;
	return ef100_nic_ext_alloc(rs->rs_client->nic, service_guid, out_mc_id);
}
EXPORT_SYMBOL(efrm_ext_alloc);


int efrm_ext_free(struct efrm_resource *rs, uint32_t mc_id)
{
	if (!check_ef100(rs))
		return -EOPNOTSUPP;
	return ef100_nic_ext_free(rs->rs_client->nic, mc_id);
}
EXPORT_SYMBOL(efrm_ext_free);


int efrm_ext_get_meta_global(struct efrm_resource *rs, uint32_t mc_handle,
                             struct efrm_ext_svc_meta *out)
{
	if (!check_ef100(rs))
		return -EOPNOTSUPP;
	return ef100_nic_ext_get_meta_global(rs->rs_client->nic, mc_handle,
	                                     out->uuid, &out->minor_ver,
	                                     &out->patch_ver, &out->nmsgs,
	                                     &out->nrsrc_classes,
	                                     &out->mapped_csr_offset,
	                                     &out->mapped_csr_size,
	                                     &out->mapped_csr_flags);
}
EXPORT_SYMBOL(efrm_ext_get_meta_global);


int efrm_ext_get_meta_rc(struct efrm_resource *rs, uint32_t mc_handle,
                         uint32_t clas, struct efrm_ext_rc_meta *out)
{
	if (!check_ef100(rs))
		return -EOPNOTSUPP;
	return ef100_nic_ext_get_meta_rc(rs->rs_client->nic, mc_handle, clas,
	                                 &out->max, &out->kern_extra);
}
EXPORT_SYMBOL(efrm_ext_get_meta_rc);


int efrm_ext_get_meta_msg(struct efrm_resource *rs, uint32_t mc_handle,
                          uint32_t msg_id, struct efrm_ext_msg_meta *out)
{
	if (!check_ef100(rs))
		return -EOPNOTSUPP;
	out->id = msg_id;
	return ef100_nic_ext_get_meta_msg(rs->rs_client->nic, mc_handle, msg_id,
	                                  &out->ix, out->name, sizeof(out->name),
	                                  &out->ef_vi_param_size,
	                                  &out->mcdi_param_size, &out->ninsns);
}
EXPORT_SYMBOL(efrm_ext_get_meta_msg);


int efrm_ext_get_meta_msg_prog(struct efrm_resource *rs, uint32_t mc_handle,
                               uint32_t msg_id, void* prog, size_t prog_bytes)
{
	if (!check_ef100(rs))
		return -EOPNOTSUPP;
	return ef100_nic_ext_get_meta_msg_prog(rs->rs_client->nic, mc_handle,
	                                       msg_id, prog, prog_bytes);
}
EXPORT_SYMBOL(efrm_ext_get_meta_msg_prog);


int efrm_ext_msg(struct efrm_resource *rs, uint32_t mc_handle,
                 uint32_t msg_id, void* buf, size_t len)
{
	if (!check_ef100(rs))
		return -EOPNOTSUPP;
	return ef100_nic_ext_msg(rs->rs_client->nic, mc_handle, msg_id, buf, len);
}
EXPORT_SYMBOL(efrm_ext_msg);


int efrm_ext_destroy_rsrc(struct efrm_resource *rs, uint32_t mc_handle,
                          uint32_t clas, uint32_t id)
{
	if (!check_ef100(rs))
		return -EOPNOTSUPP;
	return ef100_nic_ext_destroy_rsrc(rs->rs_client->nic, mc_handle, clas, id);
}
EXPORT_SYMBOL(efrm_ext_destroy_rsrc);
