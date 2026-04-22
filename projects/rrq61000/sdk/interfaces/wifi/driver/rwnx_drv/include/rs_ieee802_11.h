/* ${RENESAS_LICENSE_CLAUSE_PLACEHOLDER} */

#ifndef RS_IEEE802_11_H
#define RS_IEEE802_11_H

#include "rwnx_dev.h"
#include "ieee802_11_defs.h"		// Avoiding collisions with supplicant
#include "ieee802_11_common.h"
#include "rs_ieee802_11_def.h"

static inline int ieee80211_is_probe_resp(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_FTYPE | IEEE80211_FCTL_STYPE)) ==
	       cpu_to_le16(IEEE80211_FTYPE_MGMT | IEEE80211_STYPE_PROBE_RESP);
}

static inline int ieee80211_is_deauth(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_FTYPE | IEEE80211_FCTL_STYPE)) ==
	       cpu_to_le16(IEEE80211_FTYPE_MGMT | IEEE80211_STYPE_DEAUTH);
}

static inline int ieee80211_is_disassoc(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_FTYPE | IEEE80211_FCTL_STYPE)) ==
	       cpu_to_le16(IEEE80211_FTYPE_MGMT | IEEE80211_STYPE_DISASSOC);
}

static inline int ieee80211_is_action(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_FTYPE | IEEE80211_FCTL_STYPE)) ==
	       cpu_to_le16(IEEE80211_FTYPE_MGMT | IEEE80211_STYPE_ACTION);
}

static inline unsigned long ieee80211_tu_to_usec(unsigned long tu)
{
	return 1024 * tu;
}

static inline int ieee80211_is_auth(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_FTYPE | IEEE80211_FCTL_STYPE)) ==
	       cpu_to_le16(IEEE80211_FTYPE_MGMT | IEEE80211_STYPE_AUTH);
}

static inline bool ieee80211_is_robust_mgmt_frame(struct ieee80211_hdr *hdr)
{
	if (ieee80211_is_deauth(hdr->frame_control) ||
	    ieee80211_is_disassoc(hdr->frame_control))
		return true;

	if (ieee80211_is_action(hdr->frame_control)) {
		u8 *category;

		if ((hdr->frame_control & cpu_to_le16(IEEE80211_FCTL_PROTECTED)) != 0)
			return true;

		category = ((u8 *) hdr) + 24;
		return *category != WLAN_CATEGORY_PUBLIC &&
			*category != WLAN_CATEGORY_SELF_PROTECTED &&
			*category != WLAN_CATEGORY_HT &&
			*category != WLAN_CATEGORY_VENDOR_SPECIFIC;
	}

	return false;
}
#endif /* RS_IEEE802_11_H */
