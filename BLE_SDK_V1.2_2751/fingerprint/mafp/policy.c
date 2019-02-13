#include "policy.h"
#include "string.h"
#include "platform.h"
#include "sensor_sal.h"
#include "malib.h"
//#include "algTop.h"


static uint32_t subtpls_start = 0, bkg_start = 0, match_rcrd_start = 0, templates_start = 0;


int32_t policy_load_properties(uint8_t *buf, uint32_t size)
{
	int32_t page_size = platform_fs_page_size();
	int32_t bkg_size = IMAGE_SIZE;
	uint8_t crc = 0;

	bkg_start = ROUND_UP(SENSOR_PROPERTY_ADDR + size, page_size);
	match_rcrd_start = ROUND_UP(bkg_start + bkg_size, page_size);
	templates_start = match_rcrd_start + page_size;
	subtpls_start = templates_start + page_size * 2;

	if (*(uint32_t *)SENSOR_PROPERTY_ADDR == 0xFFFFFFFF) {
		LOGE("Sensor Parameters are not initialized.");
		return -1;
	}

	crc = CRC8((uint8_t *)SENSOR_PROPERTY_ADDR, size);
	if (crc != *(uint8_t *)(SENSOR_PROPERTY_ADDR + size)) {
		LOGE("Properties CRC check failed. %x %x", CRC8((uint8_t *)SENSOR_PROPERTY_ADDR, size)
		     , *(uint8_t *)(SENSOR_PROPERTY_ADDR + size));
		return -2;
	}
	
	platform_fs_read(SENSOR_PROPERTY_ADDR, buf, size);
	
	LOGE("bkg_start %x, rcrd_start %x, templates_start %x, subtpls_start %x",
	     bkg_start, match_rcrd_start, templates_start, subtpls_start);

	if (subtpls_start > SUBTPLS_START_ADDR) {
		LOGE("!!!!!!! Note : Flash overwrite, %x - %x.!!!!!!!!",
		     subtpls_start, SUBTPLS_START_ADDR);
		return -3;
	}

	return 0;
}


int32_t policy_clear_properties(uint32_t key)
{
	platform_fs_erase_page(SENSOR_PROPERTY_ADDR, key);

	return 0;
}


int32_t policy_save_properties(uint8_t *buf, uint32_t size, uint32_t key)
{
	uint8_t dest[sizeof(sensor_property_t) + 4] = {0};

	LOGD("start.");
	
	memcpy(dest, buf, size);
	dest[size] = CRC8(buf, size);

	platform_fs_write(SENSOR_PROPERTY_ADDR, dest, size + 1, key);

	LOGD("end.");
	
	return 0;
}


int32_t policy_load_bkg(uint8_t **buf, uint32_t size)
{
	// NOTE : here is physical address
	*buf = (uint8_t *)(bkg_start);

	LOGD("--> BKG in flash %x, size %d", *buf, size);

	return 0;
}


int32_t policy_save_bkg(uint8_t *buf, uint32_t size, uint32_t key)
{
	platform_fs_write(bkg_start, buf, size, key);

	return 0;
}


int32_t policy_init_matched_record(uint8_t *cntr)
{
	int32_t page_size = platform_fs_page_size();
	match_record_t *ptr = (match_record_t *)match_rcrd_start;
	uint16_t i, n = 0;
	
	for (i = 0; i < page_size * 2 / sizeof(match_record_t); i++) {
		if (ptr->magic == MAGIC_MATCHED_RECORD) {
			n++;
		} else {
			break;
		}
		ptr++;
	}

	*cntr = n;
	LOGE("###### n = %d, cntr = %d", n, *cntr);
	
	return n;
}


int32_t policy_clear_matched_record(uint32_t key)
{
	platform_fs_erase_page(match_rcrd_start, key);

	return 0;
}


int32_t policy_save_matched_fid(uint8_t fid, uint8_t *cntr, uint32_t key)
{
	match_record_t *ptr = (match_record_t *)match_rcrd_start + *cntr;
	match_record_t rcrd = {0};
	uint8_t erased[4] = {0xff, 0xff, 0xff, 0xff};

	LOGD("###### Start, fid = %d, cntr = %d", fid, *cntr);
	
	if (memcmp(ptr, erased, 4)) {
		LOGD("###### page erase %x", ptr);

		platform_fs_erase_page((uint32_t)ptr, key);
	}

	rcrd.magic = MAGIC_MATCHED_RECORD;
	rcrd.rsv = 0;
	rcrd.fid = fid;

	platform_fs_write_dword((uint32_t)ptr, *(uint32_t*)&rcrd, key);

	(*cntr)++;

	LOGD("###### End, fid = %d, cntr = %d", fid, *cntr);
	
	return 0;
}


int32_t policy_load_latest_matched(uint8_t latest_fids[16], uint8_t cntr)
{
	match_record_t *ptr = NULL;
	uint8_t idx, tmp_fid, w = 0;
	uint8_t weight[16] = {100, 76, 57, 42, 32, 25, 20, 16, 13, 10, 8, 6, 5, 4, 3, 2};
	uint32_t sum[16] = {0}, max, tmp;
	int n = 0, i, j;

	memset(latest_fids, 0xff, 16);

	LOGD("###### Start, cntr = %d", cntr);

	for (i = 0; i < 128; i++) {
		ptr = (match_record_t *)match_rcrd_start + (uint8_t)(cntr - 1 - i);
		if (ptr->magic == MAGIC_MATCHED_RECORD) {
			LOGD("###### -->matched[%d] = %d, %d ", i, ptr->fid, weight[i]);

			w = weight[i];
			if (i >= 16) w = 1;

			for (j = 0; j < n; j++) {
				if (ptr->fid == latest_fids[j]) {
					sum[j] += w;
					break;
				}
			}
			if (j == n) {
				latest_fids[n] = ptr->fid;
				sum[n++] = w;
			}

			if (n == 16) break;	// full, done
		} else {
			LOGD("###### no matched %d", (uint8_t)(cntr - 1 - i));
			break;
		}
	}

	for (i = 0; i < n; i++) {
		max = sum[i];
		idx = i;

		for (j = i + 1; j < n; j++) {
			if (sum[j] > max) {
				max = sum[j];
				idx = j;
			}
		}

		if (idx != i) {		// swap position
			tmp      = sum[i];
			tmp_fid  = latest_fids[i];
			
			sum[i]   = sum[idx];
			latest_fids[i] = latest_fids[idx];

			sum[idx] = tmp;
			latest_fids[idx] = tmp_fid;
		}

		LOGD("###### latest[%d] fid = %d, weight %d", i, latest_fids[i], sum[i]);
	}

	LOGD("###### End, n = %d", n);
	
	return n;
}


int32_t policy_init_templates(void)
{
	template_t *saved = (template_t *)templates_start;
	template_t *tpls = &sen_ctx.templates;
	uint8_t fid = 0, crc0 = 0, crc1 = 1;
	int32_t page_size = platform_fs_page_size();

	tpls_mngr_init(&sen_ctx.tpls_mngr);

	// templates integrity judgement
	crc0 = CRC8((uint8_t *)templates_start, sizeof(template_t));
	crc1 = CRC8((uint8_t *)templates_start + page_size, sizeof(template_t));

	LOGD("CRC %x %x, %x %x", crc0, crc1, *(uint8_t *)(templates_start + sizeof(template_t)),
	     *(uint8_t *)(templates_start + page_size + sizeof(template_t)));

	if ((crc0 != crc1) || (crc0 != *(uint8_t *)(templates_start + sizeof(template_t)))
	    || (crc1 != *(uint8_t *)(templates_start + page_size + sizeof(template_t)))) {
		if (crc1 == *(uint8_t *)(templates_start + page_size + sizeof(template_t))) {
			LOGE("load finger templates 1.");
			saved = (template_t *)(templates_start + page_size);
		}
	
		if (crc0 == *(uint8_t *)(templates_start + sizeof(template_t))) {
			LOGE("load finger templates 0.");
			saved = (template_t *)templates_start;
			
			platform_fs_write(templates_start + page_size, (template_t *)templates_start,
					  sizeof(template_t) + 1, GET_KEY_ALL());
		}
	}

        /* Commented by HansHuang 09/25/18
	if ((saved->subtpls_start != subtpls_start)
	    || (saved->subtpls_total != SUBTPLS_TOTAL)
	    || (saved->subtpl_size != TEMPLATE_SIZE)) {

		tpls->subtpls_start = subtpls_start;
		tpls->subtpls_total = SUBTPLS_TOTAL;
		tpls->subtpl_size = TEMPLATE_SIZE;

		memset(tpls->fid_status, 0, sizeof(tpls->fid_status));
	} else {*/
		memcpy(tpls, saved, sizeof(template_t));
	//}

	// clear incomplete enrollment fids
	for (fid = 0; fid < FINGER_NUM; fid++) {
		if (!sensor_fid_is_used(fid) && fid_is_used(&sen_ctx.tpls_mngr, fid)) {
			if (fid_clear(&sen_ctx.tpls_mngr, fid)) {
				LOGE("remove finger id %d failed.", fid);
				return ERROR_REMOVE_FID;
			}
		}
	}

	return 0;
}


int32_t policy_save_templates(uint32_t key)
{
	uint8_t buff[sizeof(template_t) + 1] = {0};
	int32_t page_size = platform_fs_page_size();

	memcpy(buff, &sen_ctx.templates, sizeof(template_t));
	buff[sizeof(template_t)] = CRC8(buff, sizeof(template_t));

    /*
     Added by HansHuang 09/25/18
    debugPrint("subtpls_start:%x\n\r",sen_ctx.templates.subtpls_start);
    timerDelayMs(5);
    debugPrint("subtpls_total:%x\n\r",sen_ctx.templates.subtpls_total);
    timerDelayMs(5);
    debugPrint("subtpl_size:%x\n\r",sen_ctx.templates.subtpl_size);
    timerDelayMs(5);
    debugPrint("fid_status0:%x\n\r",sen_ctx.templates.fid_status[0]);
    timerDelayMs(5);
    debugPrint("fid_status1:%x\n\r",sen_ctx.templates.fid_status[1]);
    timerDelayMs(5);
    debugPrint("sizeof(template_t):%x\n\r",sizeof(template_t));
    timerDelayMs(5);
    */


    
	platform_fs_write(templates_start, buff, sizeof(buff), key);
	platform_fs_write(templates_start + page_size, buff, sizeof(buff), key);

    /*
    Added by HansHuang 09/25/18
    for (i = 0 ; i<sizeof(buff);i++)
        {
        debugPrint("f%x:%x\n\r",templates_start+i,*(uint8_t *)(templates_start+i));
        timerDelayMs(5);
        }
        */


    

	LOGD("CRC %x", buff[sizeof(template_t)]);
	
	return 0;
}


int32_t policy_load_subtpl_header(uint8_t fid, uint8_t idx, subtpl_t *subtpl)
{
	return fidtpl_read_subtpl_header(&sen_ctx.tpls_mngr, fid, idx, subtpl);
}


int32_t policy_load_subtpl_body(uint8_t fid, uint8_t idx, subtpl_t *subtpl)
{
	return fidtpl_read_subtpl_body(&sen_ctx.tpls_mngr, fid, idx, subtpl);
}


int32_t policy_load_subtpl(uint8_t fid, uint8_t idx, subtpl_t *subtpl)
{
	return fidtpl_read_subtpl(&sen_ctx.tpls_mngr, fid, idx, subtpl);
}


int32_t policy_load_latest_modified_subtpl(subtpl_t *subtpl)
{
	return subtpl_load_latest_modified(&sen_ctx.tpls_mngr, subtpl);
}


int32_t policy_save_subtpl(uint8_t fid, uint8_t idx, subtpl_t *subtpl)
{
	 return fidtpl_update_subtpl(&sen_ctx.tpls_mngr, fid, idx, subtpl);
}


int32_t policy_get_subtpls_amount(uint8_t fid)
{
	return fidtpl_get_subtpls_amount(&sen_ctx.tpls_mngr, fid);
}


int32_t policy_get_subtpl_updatetimes(uint8_t fid, uint8_t idx, int32_t *val)
{
	return fidtpl_get_subtpl_updatetimes(&sen_ctx.tpls_mngr, fid, idx, val);
}


int32_t policy_get_subtpl_write_times(uint8_t fid, uint8_t idx, uint16_t *tid, uint32_t *write_times)
{
	return fidtpl_get_subtpl_writetimes(&sen_ctx.tpls_mngr, fid, idx, tid, write_times);
}


int32_t policy_enroll_cancel(uint8_t fid)
{
	return fid_clear(&sen_ctx.tpls_mngr, fid);
}


