#include "malib.h"
#include <string.h>
#include "tpls_mngr.h"
#include "sensor_sal.h"

#include <stdlib.h>

//#define LOGD(format, ...) debugPrint("[%s:%d] " format "\r\n", __func__, __LINE__, ##__VA_ARGS__ )

static int32_t subtpls_is_valid(uint16_t tid, uint8_t *fid, uint32_t *update_times);
static int32_t subtpl_validation_destroy(uint16_t tid);

static int32_t subtpl_read_header(uint16_t tid, subtpl_t *subtpl);
static int32_t subtpl_read_body(uint16_t tid, subtpl_t *subtpl);
static int32_t subtpl_read(uint16_t tid, subtpl_t *subtpl);
static int32_t subtpl_write(uint16_t tid, subtpl_t *subtpl);

static int32_t subtpls_load(tpls_mngr_t *tpl_mngr);
static int32_t fidtpl_tid_attach(tpls_mngr_t *tpl_mngr, uint8_t fid, uint16_t tid, uint32_t update_times);

static int32_t fids_freq_sort(tpls_mngr_t *tpl_mngr);
static int32_t fidtpl_tids_sort(tpls_mngr_t *tpl_mngr, uint8_t fid);

static int32_t tid_is_used(tpls_mngr_t *tpl_mngr, uint16_t tid);
static int32_t tid_set_used(tpls_mngr_t *tpl_mngr, uint16_t tid);
static int32_t tid_clear_used(tpls_mngr_t *tpl_mngr, uint16_t tid);

static int32_t subtpls_reaper(tpls_mngr_t *tpl_mngr, uint8_t self_fid);
static int32_t subtpl_alloc(tpls_mngr_t *tpl_mngr, uint16_t *tid, uint8_t self_fid);
static int32_t subtpl_free(tpls_mngr_t *tpl_mngr, uint16_t tid);

static int32_t subtpl_load_latest_updated_record(tpls_mngr_t *tpl_mngr);
static int32_t subtpl_save_latest_updated_record(tpls_mngr_t *tpl_mngr, uint16_t tid);

int32_t subtpls_block_is_dead(uint16_t tid)
{
	uint32_t addr = SUBTPL_ATTR(tid);
	subtpl_attr_t *attr = (subtpl_attr_t *)addr;

    //LOGE("addr:%x\n",addr);

	if (attr->valid_flag == 0xFFFFFFFF) {
		platform_fs_write_dword(addr + 4, SUBTPL_VALID_MAGIC, 0);
	}

    //LOGE("addr value:%x\n",*(uint32_t *)(addr+4));

    
	
	if (attr->valid_flag != SUBTPL_VALID_MAGIC) {
		LOGE("TID %d is not valid, %x.", tid, attr->valid_flag);
		return 1;
	}

	LOGD("TID %d is valid %x.", tid, attr->valid_flag);
	
	return 0;
}


int32_t subtpls_blocks_reset(void)
{
	int32_t i;
	uint32_t addr;

	for (i = 0; i < SUBTPLS_TOTAL; i ++) {
		addr = SUBTPL_ATTR(i);
		platform_fs_erase_page(addr, 0);

		LOGD("TID = %d, addr = %x", i, addr);
	}

	return 0;
}


int32_t subtpls_is_valid(uint16_t tid, uint8_t *fid, uint32_t *update_times)
{
	uint32_t addr = SUBTPL_ATTR(tid);
	subtpl_attr_t *ptr = (subtpl_attr_t *)addr;

	LOGD("Start TID=%d, %x", tid, addr);

	if (ptr->magic != SUBTPL_MAGIC) {
		//LOGE("magic is not correct %x.", ptr->magic);
		return -1;
	}
	
	if (ptr->fid >= FINGER_NUM) {
		LOGE("fid %d is invalid.", ptr->fid);
		return -2;
	}

	if ((ptr->rsv != 0x5a5a) || (ptr->valid_flag != SUBTPL_VALID_MAGIC)) {
		LOGD("TID %d is invalid.", tid);
		return -3;
	}

	if (CRC8((uint8_t *)addr, sizeof(subtpl_attr_t) - 1) == ptr->crc) {
		*fid = ptr->fid;
		*update_times = ptr->update_times;

		LOGD("Valid TID %3d, fid %2d , update %3d, write %3d",
		     tid, ptr->fid, ptr->update_times, ptr->write_times);
		return 1;
	}
	
	LOGD("Invalid TID %3d, CRC failed %x, update %3d, write %3d",
	     tid, ptr->crc, ptr->update_times, ptr->write_times);
	
	return 0;
}


// Note: we just clear the CRC part, and the write_time still there
int32_t subtpl_validation_destroy(uint16_t tid)
{
	uint32_t addr = SUBTPL_POS(tid);

	// clear the CRC
	platform_fs_write_dword(addr + sizeof(subtpl_t) - 4, 0, 0);
	
	return 0;
}


int32_t fid_is_used(tpls_mngr_t *tpl_mngr, uint8_t fid)
{
	return tpl_mngr->fids_bitsmap[fid/8] & (1 << (fid & 0x7));
}


int32_t fid_set_used(tpls_mngr_t *tpl_mngr, uint8_t fid)
{
	if (!(tpl_mngr->fids_bitsmap[fid/8] & (1 << (fid & 0x7)))) {
		tpl_mngr->fids_bitsmap[fid/8] |= 1 << (fid & 0x7);
		tpl_mngr->fids_used++;
	}

	return 0;
}


int32_t fid_clear(tpls_mngr_t *tpl_mngr, uint8_t fid)
{
	int32_t i, amount = tpl_mngr->fids_pool[fid].amount;

	//fid_subtpls_state(tpl_mngr, fid);
		
	for (i = 0; i < amount; i++) {
		if (subtpl_free(tpl_mngr, tpl_mngr->fids_pool[fid].subtpls[i])) {
			LOGE("Failed, %d.", i);
			return -1;
		}
	}

	tpl_mngr->fids_pool[fid].update_times = 0;
	
	if (tpl_mngr->fids_bitsmap[fid/8] & (1 << (fid & 0x7))) {
		tpl_mngr->fids_bitsmap[fid/8] &= ~(1 << (fid & 0x7));
		tpl_mngr->fids_used--;
	}

	return 0;	
}


int32_t fids_get_total_used(tpls_mngr_t *tpl_mngr)
{
	return tpl_mngr->fids_used;
}


int32_t tid_is_used(tpls_mngr_t *tpl_mngr, uint16_t tid)
{
	return tpl_mngr->subtpls_bitsmap[tid/8] & (1 << (tid & 0x7));
}


int32_t tid_set_used(tpls_mngr_t *tpl_mngr, uint16_t tid)
{
	if (!(tpl_mngr->subtpls_bitsmap[tid/8] & (1 << (tid & 0x7)))) {
		tpl_mngr->subtpls_bitsmap[tid/8] |= 1 << (tid & 0x7);
		tpl_mngr->subtpls_used++;
	}
	
	return 0;
}


int32_t tid_clear_used(tpls_mngr_t *tpl_mngr, uint16_t tid)
{
	if (tpl_mngr->subtpls_bitsmap[tid/8] & (1 << (tid & 0x7))) {
		tpl_mngr->subtpls_bitsmap[tid/8] &= ~(1 << (tid & 0x7));
		tpl_mngr->subtpls_used--;
	}

	return 0;
}


int32_t fidtpl_tid_attach(tpls_mngr_t *tpl_mngr, uint8_t fid, uint16_t tid, uint32_t update_times)
{
	fidtpl_t *p = &tpl_mngr->fids_pool[fid];
	
	LOGD("Start FID=%d, TID=%d, UT=%d", fid, tid, update_times);

	fid_set_used(tpl_mngr, fid);

	if (p->amount < FID_SUBTPLS_MAX_NUM) {
		p->subtpls[p->amount++] = tid;
	} else {
		LOGE("out of range.");
		return -1;
	}
	
	if (update_times > p->update_times) {
		LOGD("fid[%d].update_times : %d -> %d", fid, p->update_times, update_times);
		p->update_times = update_times;
	}

	tid_set_used(tpl_mngr, tid);

	LOGD("end.");
	
	return p->amount;
}


int32_t fidtpl_update_subtpl(tpls_mngr_t *tpl_mngr, uint8_t fid, uint8_t idx, subtpl_t *subtpl)
{
	fidtpl_t *p = &tpl_mngr->fids_pool[fid];
	uint16_t tid;
	int32_t ret = 0;
	
	LOGD("Start FID=%d, IDX=%d, Amount=%d", fid, idx, p->amount);

	if (idx >= FID_SUBTPLS_MAX_NUM) {
		LOGE("out of range, idx %d, amount %d", idx, p->amount);
		return -1;
	}

	if (idx == p->amount) {
		// allocate new one
		if (subtpl_alloc(tpl_mngr, &tid, fid) < 0) {
			LOGE("subtpl_alloc failed.");
			return -2;
		}
		tid_set_used(tpl_mngr, tid);
		fid_set_used(tpl_mngr, fid);
		p->subtpls[p->amount++] = tid;
	} else if (idx < p->amount) {
		// update existed
		tid = p->subtpls[idx];
	} else {
		LOGE("out of range.");
		return -3;
	}

	p->update_times++;

	subtpl->attr.fid = fid;
	subtpl->attr.update_times = p->update_times;

	LOGD("fid[%d].update_times : %d, tid %d", fid, p->update_times, tid);

	ret = subtpl_write(tid, subtpl);
	if (ret) {
		LOGE("subtpl_write failed.");
		if (ret == -2) {
			// we alloc another one to replace it
			if (subtpl_alloc(tpl_mngr, &tid, fid) < 0) {
				LOGE("subtpl_alloc failed.");
				return -4;
			}
			tid_set_used(tpl_mngr, tid);

			LOGE("subtpl_write failed, TID %d alloced to replace it.", tid);

			p->subtpls[idx] = tid;

			ret = subtpl_write(tid, subtpl);
			if (ret) {
				LOGE("subtpl_write failed.");
				return -5;
			}
		}
	}

	subtpl_save_latest_updated_record(tpl_mngr, tid);

	LOGD("end.");
	
	return p->amount;
}


int32_t fidtpl_read_subtpl_header(tpls_mngr_t *tpl_mngr, uint8_t fid, uint8_t idx, subtpl_t *subtpl)
{
	fidtpl_t *p = &tpl_mngr->fids_pool[fid];
	uint16_t tid;
	
	LOGD("Start FID=%d, IDX=%d, Amount=%d", fid, idx, p->amount);

	if ((fid >= FINGER_NUM) || (idx >= FID_SUBTPLS_MAX_NUM) || (idx >= p->amount)) {
		LOGE("out of range, fid %d, idx %d, amount %d", fid, idx, p->amount);
		return -1;
	}

	tid = p->subtpls[idx];
	if (tid >= SUBTPLS_TOTAL) {
		LOGE("out of range, tid %d", tid);
		return -2;
	}

	return subtpl_read_header(tid, subtpl);
}


int32_t fidtpl_read_subtpl_body(tpls_mngr_t *tpl_mngr, uint8_t fid, uint8_t idx, subtpl_t *subtpl)
{
	fidtpl_t *p = &tpl_mngr->fids_pool[fid];
	uint16_t tid;
	
	LOGD("Start FID=%d, IDX=%d, Amount=%d", fid, idx, p->amount);

	if ((fid >= FINGER_NUM) || (idx >= FID_SUBTPLS_MAX_NUM) || (idx >= p->amount)) {
		LOGE("out of range, fid %d, idx %d, amount %d", fid, idx, p->amount);
		return -1;
	}

	tid = p->subtpls[idx];
	if (tid >= SUBTPLS_TOTAL) {
		LOGE("out of range, tid %d", tid);
		return -2;
	}

	if (subtpl_read_body(tid, subtpl)) {
		LOGE("read body failed");
		return -3;
	}

	return 0;
}


int32_t fidtpl_read_subtpl(tpls_mngr_t *tpl_mngr, uint8_t fid, uint8_t idx, subtpl_t *subtpl)
{
	fidtpl_t *p = &tpl_mngr->fids_pool[fid];
	uint16_t tid;
	
	LOGD("Start FID=%d, IDX=%d, Amount=%d", fid, idx, p->amount);

	if ((fid >= FINGER_NUM) || (idx >= FID_SUBTPLS_MAX_NUM) || (idx >= p->amount)) {
		LOGE("out of range, fid %d, idx %d, amount %d", fid, idx, p->amount);
		return -1;
	}

	tid = p->subtpls[idx];
	if (tid >= SUBTPLS_TOTAL) {
		LOGE("out of range, tid %d", tid);
		return -2;
	}

	return subtpl_read(tid, subtpl);
}


int32_t fidtpl_get_subtpls_amount(tpls_mngr_t *tpl_mngr, uint8_t fid)
{
	fidtpl_t *p = NULL; 
	
	LOGD("Start FID=%d", fid);

	if ((fid >= FINGER_NUM)) {
		LOGE("out of range, fid %d.", fid);
		return -1;
	}

	p = &tpl_mngr->fids_pool[fid];

	return p->amount;
}



int32_t fidtpl_get_subtpl_updatetimes(tpls_mngr_t *tpl_mngr, uint8_t fid, uint8_t idx, int32_t *update_times)
{
	uint16_t tid = 0;
	uint8_t  amount = tpl_mngr->fids_pool[fid].amount;
	subtpl_attr_t *ptr;

	LOGD("Start FID=%d", fid);

	if ((fid >= FINGER_NUM) || (idx >= FID_SUBTPLS_MAX_NUM) || (idx >= amount)) {
		LOGE("out of range, fid %d, idx %d, amount %d", fid, idx, amount);
		return -1;
	}

	tid = tpl_mngr->fids_pool[fid].subtpls[idx];

	ptr = (subtpl_attr_t *)SUBTPL_ATTR(tid);
	if (ptr->magic != SUBTPL_MAGIC) {
		LOGE("TID %d, magic is not correct %x.", tid, ptr->magic);
		return -2;
	}

	LOGD("IDX %d, TID %d, update times %d", idx, tid, ptr->update_times);

	*update_times = ptr->update_times;

	return 0;
}


int32_t fidtpl_get_subtpl_writetimes(tpls_mngr_t *tpl_mngr, uint8_t fid, uint8_t idx,
				     uint16_t *tid, uint32_t *write_times)
{
	fidtpl_t *p = &tpl_mngr->fids_pool[fid];
	int32_t ret = 0;
	
	LOGD("Start FID=%d, IDX=%d, Amount=%d", fid, idx, p->amount);

	if ((fid >= FINGER_NUM) || (idx >= FID_SUBTPLS_MAX_NUM) || (idx >= p->amount)) {
		LOGE("out of range, fid %d, idx %d, amount %d", fid, idx, p->amount);
		return -1;
	}

	*tid = p->subtpls[idx];

	ret = subtpl_get_write_times(*tid);
	if (ret < 0) {
		LOGE("subtpl_write_times_get failed.");
		return -2;
	}

	*write_times = ret;

	return 0;
}


int32_t fidtpl_get_subtpl_tid(tpls_mngr_t *tpl_mngr, uint8_t fid, uint8_t idx, uint16_t *tid)
{
	fidtpl_t *p = &tpl_mngr->fids_pool[fid];
	
	LOGD("Start FID=%d, IDX=%d, Amount=%d", fid, idx, p->amount);

	if ((fid >= FINGER_NUM) || (idx >= FID_SUBTPLS_MAX_NUM) || (idx >= p->amount)) {
		LOGE("out of range, fid %d, idx %d, amount %d", fid, idx, p->amount);
		return -1;
	}

	*tid = p->subtpls[idx];

	return 0;
}


int32_t subtpls_load(tpls_mngr_t *tpl_mngr)
{
	uint32_t update_times, i;
	uint8_t  fid;

	for (i = 0; i < SUBTPLS_TOTAL; i++) {
		if (subtpls_block_is_dead(i)) {
			tid_set_used(tpl_mngr, i);
			tpl_mngr->subtpls_block_dead++;
			continue;
		}
		
		if (subtpls_is_valid(i, &fid, &update_times) <= 0) {
			continue;
		}

		if (fidtpl_tid_attach(tpl_mngr, fid, i, update_times) < 0) {
			LOGE("attatch %d failed.", i);
			return -1;
		}
	}

	return 0;
}


int32_t subtpl_read_header(uint16_t tid, subtpl_t *subtpl)
{
	uint8_t  fid;
	uint32_t addr, update_times;
	
	if (subtpls_is_valid(tid, &fid, &update_times) <= 0) {
		LOGE("invalid.");
		return -1;
	}

	addr = SUBTPL_POS(tid);
	LOGD("TID=%d, TOP Eflash addr=%x", tid, addr);

	if (platform_fs_read(addr, subtpl->data, SUBTPL_HEADER_SIZE)) {
		LOGE("fs read failed.");
		return -2;
	}

	addr = SUBTPL_ATTR(tid);
	if (platform_fs_read(addr, &subtpl->attr, sizeof(subtpl_attr_t))) {
		LOGE("fs read attr failed.");
		return -3;
	}

	return 0;
}


int32_t subtpl_read_body(uint16_t tid, subtpl_t *subtpl)
{
	uint32_t addr;
	
	addr = SUBTPL_POS(tid);
	if (platform_fs_read(addr + SUBTPL_HEADER_SIZE, subtpl->data + SUBTPL_HEADER_SIZE,
			     TEMPLATE_SIZE - SUBTPL_HEADER_SIZE)) {
		LOGE("fs read failed.");
		return -1;
	}
	
	return 0;
}


int32_t subtpl_read(uint16_t tid, subtpl_t *subtpl)
{
	if (tid >= SUBTPLS_TOTAL) {
		LOGE("TID=%d invalid, max %d", tid, SUBTPLS_TOTAL);
		return -1;
	}

	// 512 bytes in Eflash
	if (subtpl_read_header(tid, subtpl)) {
		LOGE("TID %d, invalid.", tid);
		return -2;
	}

	// remainder part, in EFlash OR SPI-Flash
	if (subtpl_read_body(tid, subtpl)) {
		LOGE("TID %d, body failed.", tid);
		return -3;
	}
	
	LOGD("TID %d, write_times %d", tid, subtpl->attr.write_times);

	return 0;
}


int32_t subtpl_write(uint16_t tid, subtpl_t *subtpl)
{
	uint32_t addr;
	subtpl_attr_t attr;
	
	LOGD("TID %d", tid);
	
	if (tid >= SUBTPLS_TOTAL) {
		LOGE("TID=%d invalid, max %d", tid, SUBTPLS_TOTAL);
		return -1;
	}

	// destroy the valid tid content
	subtpl_validation_destroy(tid);

	attr = *(subtpl_attr_t *)SUBTPL_ATTR(tid);

	// Write times still in flash, if it is inited
	if ((attr.magic != SUBTPL_MAGIC) || (attr.write_times == 0xffffff)) {
		attr.write_times = 0;
	}

	// setup new attributions
	subtpl->attr.magic = SUBTPL_MAGIC;
	subtpl->attr.write_times = attr.write_times + 1;
	subtpl->attr.rsv = 0x5a5a;  /// placeholder
	subtpl->attr.valid_flag = SUBTPL_VALID_MAGIC;
	subtpl->attr.crc = CRC8((uint8_t *)&subtpl->attr, sizeof(subtpl_attr_t) - 1);

	LOGD("TID=%d, write_times=%d, CRC=%x", tid, subtpl->attr.write_times, subtpl->attr.crc);

	addr = SUBTPL_POS(tid);
	LOGD("TID=%d, TOP Eflash addr=%x", tid, addr);

	if (platform_fs_write(addr, subtpl, sizeof(*subtpl), 0)) {
		LOGE("fs write failed.");
		if (platform_fs_write(addr, subtpl, sizeof(*subtpl), 0)) {
			LOGE("fs write failed.");

			// set this page as dead
			addr = SUBTPL_ATTR(tid);
			platform_fs_write_dword(addr + 4, 0, 0);
			
			return -2;
		}
	}

	return 0;
}


int32_t subtpls_exchange(tpls_mngr_t *tpl_mngr, uint16_t tid1, uint16_t tid2,
			 subtpl_t *subtpl1, subtpl_t *subtpl2)
{
	uint8_t fid1, fid2, i;
	
	if (subtpl_read(tid1, subtpl1)) {
		LOGE("invalid subtpls");
		return -1;
	}
	fid1 = subtpl1->attr.fid;
	
	if (subtpl_read(tid2, subtpl2)) {
		LOGE("invalid subtpls");
		return -2;
	}
	fid2 = subtpl2->attr.fid;

	//// TID2 content --> TID1
	if (subtpl_write(tid1, subtpl2)) {
		LOGE("write failed");
		return -3;
	}
	
	// Setup TID1 to fid2
	for (i = 0; i < FID_SUBTPLS_MAX_NUM; i++) {
		if (tpl_mngr->fids_pool[fid2].subtpls[i] == tid2) {
			tpl_mngr->fids_pool[fid2].subtpls[i] = tid1;
			break;
		}
	}
	if (i == FID_SUBTPLS_MAX_NUM) {
		LOGE("Panic, something wrong.");
		return -4;
	}

	//// TID1 content --> TID2
	if (subtpl_write(tid2, subtpl1)) {
		LOGE("write failed");
		return -5;
	}
	
	// Setup TID2 to fid1
	for (i = 0; i < FID_SUBTPLS_MAX_NUM; i++) {
		if (tpl_mngr->fids_pool[fid1].subtpls[i] == tid1) {
			tpl_mngr->fids_pool[fid1].subtpls[i] = tid2;
			break;
		}
	}
	if (i == FID_SUBTPLS_MAX_NUM) {
		LOGE("Panic, something wrong.");
		return -6;
	}

	return 0;
}


int32_t subtpl_alloc(tpls_mngr_t *tpl_mngr, uint16_t *tid, uint8_t self_fid)
{
	int32_t i;

	*tid = SUBTPLS_TOTAL;
	
	if (tpl_mngr->subtpls_used >= SUBTPLS_TOTAL) {
		LOGD("out of pool %d.", tpl_mngr->subtpls_used);

		if (subtpls_reaper(tpl_mngr, self_fid) <= 0) {
			LOGE("reaper failed.");
			return -1;
		}
	}
	
	for (i = 0; i < SUBTPLS_TOTAL; i++) {
		if (!tid_is_used(tpl_mngr, i)) {
			*tid = i;
			break;
		}
	}

	if (i == SUBTPLS_TOTAL) {
		LOGE("out of pool.");
		return -2;
	}

	return 0;
}


int32_t subtpl_free(tpls_mngr_t *tpl_mngr, uint16_t tid)
{
	uint8_t  fid, i;
	uint32_t update_times;

	//LOGE("--> Freeing TID %d", tid);

	if (subtpls_is_valid(tid, &fid, &update_times) <= 0) {
		LOGE("invalid subtpl, tid %d.", tid);
		return -1;
	}

	if (subtpl_validation_destroy(tid)) {
		LOGE("validation_destroy failed");
		return -2;
	}

	tid_clear_used(tpl_mngr, tid);

	// fidtpl clear tid
	for (i = 0; i < FID_SUBTPLS_MAX_NUM; i++) {
		if (tpl_mngr->fids_pool[fid].subtpls[i] == tid) {
			tpl_mngr->fids_pool[fid].subtpls[i] = 0xffff;
			tpl_mngr->fids_pool[fid].amount--;
			break;
		}
	}
	if (i == FID_SUBTPLS_MAX_NUM) {
		LOGE("Panic, something wrong.");
		return -3;
	}

	return 0;
}


int32_t fids_freq_sort(tpls_mngr_t *tpl_mngr)
{
	int32_t i, j, max, idx = -1, tids = 0;
	uint8_t src[ FINGER_NUM ] = {0}, total = 0, tmp;
	
	memset(tpl_mngr->sorted_fids_list, 0xFF, FINGER_NUM);
	
	// load src fids data
	for (j = 0; j < FINGER_NUM; j++) {
		if (fid_is_used(tpl_mngr, j))   src[total++] = j;
	}

	if (tpl_mngr->fids_used != total) {
		LOGE("Panic, something wrong !!!!!");
		return -1;
	}

	// select sort
	LOGD("---------------------------- SORTED FIDs list --------------------------");
	for (i = 0; i < total; i++) {
		max = tpl_mngr->fids_pool[src[i]].update_times;
		idx = i;
		
		for (j = i + 1; j < total; j++) {
			if (tpl_mngr->fids_pool[src[j]].update_times > max) {
				max = tpl_mngr->fids_pool[src[j]].update_times;
				idx = j;
			}
		}

		if (idx != i) {		// swap position
			tmp      = src[i];
			src[i]   = src[idx];
			src[idx] = tmp;
		}

		// assign group according to its postion in list
		//group = tpl_mngr->group_index[i];
		//tpl_mngr->fids_pool[src[i]].group = group;

		LOGD("[%2d - FID=%2d, %2d, UT=%4d]", i, src[i],
		     tpl_mngr->fids_pool[src[i]].amount, 
		     tpl_mngr->fids_pool[src[i]].update_times);
		tids += tpl_mngr->fids_pool[src[i]].amount;
	}
	LOGD("Fid USED : %2d, Tid USED : %3d, TOTAL %d", tpl_mngr->fids_used, tids, SUBTPLS_TOTAL);
	LOGD("---------------------------- SORTED list END --------------------------");

	memcpy(tpl_mngr->sorted_fids_list, src, total);
	
	return total;
}


int32_t fidtpl_tids_sort(tpls_mngr_t *tpl_mngr, uint8_t fid)
{
	uint16_t *src = tpl_mngr->fids_pool[fid].subtpls, tmp;
	int32_t  max; 
	uint8_t  amount = tpl_mngr->fids_pool[fid].amount, i, j, idx;
	subtpl_attr_t *ptr;

	LOGD("------------------------ SORTED FID %d TIDs list -----------------------", fid);
	for (i = 0; i < amount; i++) {
		if (src[i] >= SUBTPLS_TOTAL) {
			LOGE("Panic, something wrong.!!!!!");
			return -1;
		}
		
		ptr = (subtpl_attr_t *)SUBTPL_ATTR(src[i]);
		max = ptr->update_times;
		idx = i;
		
		for (j = i; j < amount; j++) {
			ptr = (subtpl_attr_t *)SUBTPL_ATTR(src[j]);

			if (ptr->update_times > max) {
				max = ptr->update_times;
				idx = j;
			}
		}

		if (idx != i) {		// swap position
			tmp      = src[i];
			src[i]   = src[idx];
			src[idx] = tmp;
		}

		LOGD("[%2d - %3d - %3d]", i, src[i], max);
	}
	LOGD("---------------------------- SORTED list END --------------------------");
	
	return 0;
}


int32_t subtpls_reaper(tpls_mngr_t *tpl_mngr, uint8_t self_fid)
{
	int32_t total = tpl_mngr->fids_used, i, j, max, idx, cur;
	uint8_t fid, n = 0, amount, tmp;
	uint8_t *src = tpl_mngr->sorted_fids_list;

	LOGE("Reaper run, Hoong, Hoong, Hoong...");
	
	if (fids_freq_sort(tpl_mngr) < 0) {
		LOGE("fids sort failed.");
		return -1;
	}

	for (i = FINGER_NUM_G0; i < total; i++) {
		max = tpl_mngr->fids_pool[src[i]].update_times / tpl_mngr->fids_pool[src[i]].amount;
		idx = i;
		
		for (j = i + 1; j < total; j++) {
			cur = tpl_mngr->fids_pool[src[j]].update_times / tpl_mngr->fids_pool[src[j]].amount;
			if (cur > max) {
				max = cur;
				idx = j;
			}
		}

		if (idx != i) {		// swap position
			tmp      = src[i];
			src[i]   = src[idx];
			src[idx] = tmp;
		}

		LOGD("[%2d - FID=%2d, %2d, UT=%4d, %d]", i, src[i],
		     tpl_mngr->fids_pool[src[i]].amount, 
		     tpl_mngr->fids_pool[src[i]].update_times,
		     tpl_mngr->fids_pool[src[i]].update_times / tpl_mngr->fids_pool[src[i]].amount);
	}

	LOGD("########################### Total %d #####################", total);

	for (i = total - 1; i >= 0; i--) {
		fid    = tpl_mngr->sorted_fids_list[i];
		if (self_fid == fid) {
			LOGD("skip myself.");
			continue;
		}
		amount = tpl_mngr->fids_pool[fid].amount;
		if (amount <= FID_SUBTPLS_MIN_NUM) continue;

		LOGD("########################### Cut %d, fid : %d ##################", i, fid);
	
		if (fidtpl_tids_sort(tpl_mngr, fid)) {
			LOGE("tids sort failed.");
			return -2;
		}
	
		if (subtpl_free(tpl_mngr, tpl_mngr->fids_pool[fid].subtpls[amount - 1])) {
			LOGE("free failed.");
			return -3;
		}

		n = 1;
		break;
	}

	LOGD("%d TIDS reapered", n);
	
	return n;
}


int32_t subtpls_show_write_times(void)
{
	subtpl_attr_t *ptr;
	int32_t total = 0, i;

	for (i = 0; i < SUBTPLS_TOTAL; i++) {
		ptr = (subtpl_attr_t *)SUBTPL_ATTR(i);
		if (ptr->magic != SUBTPL_MAGIC) {
			LOGE("TID %d, magic is not correct %x.", i, ptr->magic);
			continue;
		}
		
		total += ptr->write_times;
		LOGE("TID %d, write times %d", i, ptr->write_times);
	}
	LOGE("Total write times %d.", total);
	
	return 0;
}


int32_t subtpl_get_write_times(uint16_t tid)
{
	subtpl_attr_t *ptr;
	
	if (tid >= SUBTPLS_TOTAL) {
		LOGE("TID=%d invalid, max %d", tid, SUBTPLS_TOTAL);
		return -1;
	}

	ptr = (subtpl_attr_t *)SUBTPL_ATTR(tid);
	if (ptr->magic != SUBTPL_MAGIC) {
		LOGD("TID %d, magic is not correct %x.", tid, ptr->magic);
		return -2;
	}

	LOGD("TID %d, write times %d", tid, ptr->write_times);

	return ptr->write_times;
}


int32_t subtpls_coolarea_get(void)
{
	subtpl_attr_t *ptr;
	int32_t i, minimum = 0xfffffff, tid = SUBTPLS_TOTAL;
	uint32_t update_times;
	uint8_t  fid;

	for (i = 0; i < SUBTPLS_TOTAL; i++) {
		if (subtpls_is_valid(i, &fid, &update_times) <= 0) {
			continue;
		}

		ptr = (subtpl_attr_t *)SUBTPL_ATTR(i);
		if (minimum > ptr->write_times) {
			minimum = ptr->write_times;
			tid = i;
		}
	}
	
	if (tid == SUBTPLS_TOTAL) {
		LOGD("out of range.");
		return -1;
	}
	
	LOGD("Minimun TID[%d] write times %d.", tid, minimum);
	
	return tid;
}


int32_t fids_get_sorted_list(tpls_mngr_t *tpl_mngr, uint8_t sorted_list[ FINGER_NUM ])
{
	uint8_t used = fids_freq_sort(tpl_mngr);

	memset(sorted_list, 0xff, FINGER_NUM);
	memcpy(sorted_list, tpl_mngr->sorted_fids_list, used);
	
	return used;
}


int32_t tids_get_sorted_list(tpls_mngr_t *tpl_mngr, uint8_t fid,
			     uint16_t sorted_list[ FID_SUBTPLS_MAX_NUM ])
{
	uint8_t amount = tpl_mngr->fids_pool[fid].amount;
	
	fidtpl_tids_sort(tpl_mngr, fid);

	memcpy(sorted_list, tpl_mngr->fids_pool[fid].subtpls, amount);
	
	return amount;
}


void fid_show_status(tpls_mngr_t *tpl_mngr, uint8_t fid)
{
	fidtpl_t * pfid = &tpl_mngr->fids_pool[fid];
	subtpl_attr_t attr;
	uint16_t tid, i;
	
	//fidtpl_tids_sort(tpl_mngr, fid);

	LOGE("All %d, Used %d\r\n", SUBTPLS_TOTAL, tpl_mngr->subtpls_used);

	LOGE("===> FID[%d] amount %d, update_times %d : ", fid, pfid->amount, pfid->update_times);
	for (i = 0; i < pfid->amount; i++) {
		tid = tpl_mngr->fids_pool[fid].subtpls[i];
		attr = *(subtpl_attr_t *)SUBTPL_ATTR(tid);

		LOGE("%2d, TID %3d, fid %2d, update %3d, write %3d ",
		     i, tid, attr.fid, attr.update_times, attr.write_times);
	}
}



int32_t subtpl_load_latest_updated_record(tpls_mngr_t *tpl_mngr)
{
	updated_record_t *ptr = (updated_record_t *)SUBTPLS_START_ADDR;
	uint16_t i, n = 0;

	for (i = 0; i < SUBTPLS_UPDATE_RECORD_TOTAL; i++, ptr++) {
		if (ptr->magic == MAGIC_UPDATED_RECORD) {
			n++;			
		} else {
			break;
		}
	}

	tpl_mngr->updated_cntr = n;
	
	LOGD("###### cntr = %d", n);
	
	return n;
}


int32_t subtpl_save_latest_updated_record(tpls_mngr_t *tpl_mngr, uint16_t tid)
{
	updated_record_t *ptr = (updated_record_t *)SUBTPLS_START_ADDR;
	updated_record_t rcrd = {0};
	uint8_t erased[4] = {0xff, 0xff, 0xff, 0xff};
	uint32_t val = 0;

	LOGD("###### Start, tid = %d, cntr = %d", tid, tpl_mngr->updated_cntr);

	if (tpl_mngr->updated_cntr >= SUBTPLS_UPDATE_RECORD_TOTAL) {
		tpl_mngr->updated_cntr = 0;
	}

	ptr = (updated_record_t *)SUBTPLS_START_ADDR + tpl_mngr->updated_cntr;
	if (memcmp(ptr, erased, 4)) {
		LOGD("###### page erase %x", ptr);

		platform_fs_erase_page((uint32_t)ptr, 0);
		tpl_mngr->updated_cntr = 0;
		ptr = (updated_record_t *)SUBTPLS_START_ADDR;
	}

	rcrd.magic = MAGIC_UPDATED_RECORD;
	rcrd.tid = tid;
	memcpy(&val, &rcrd, 4);
	platform_fs_write_dword((uint32_t)ptr, val, 0);

	tpl_mngr->updated_cntr++;

	LOGD("###### End, cntr = %d, tid = %d", tpl_mngr->updated_cntr, tid);
	
	return 0;
}


int32_t subtpl_load_latest_modified(tpls_mngr_t *tpl_mngr, subtpl_t * subtpl)
{
	updated_record_t *ptr;
	uint32_t i;
	
	LOGD("###### start cntr %d", tpl_mngr->updated_cntr);

	for (i = tpl_mngr->updated_cntr; i > 0; i--) {
		ptr = (updated_record_t *)SUBTPLS_START_ADDR + i - 1;
			
		if (tid_is_used(tpl_mngr, ptr->tid)) {
			LOGD("###### load %d, tid = %d", i, ptr->tid);
			return subtpl_read(ptr->tid, subtpl);
		}
	}
	
	return -1;
}


int32_t tpls_mngr_init(tpls_mngr_t *tpl_mngr)
{
	memset(tpl_mngr, 0, sizeof(*tpl_mngr));

	LOGD("---------------------------- Micros --------------------------");
	LOGD("sizeof(*tpl_mngr) = %d bytes", sizeof(*tpl_mngr));
	LOGD("sizeof(subtpl_t) = %d bytes, TEMPLATE_SIZE = %d", sizeof(subtpl_t), TEMPLATE_SIZE);
	LOGD("Fingers %d, Existed %d", FINGER_NUM, SUBTPLS_TOTAL);
	LOGD("---------------------------- Micros --------------------------\r\n");
	
	// load from flash
	if (subtpls_load(tpl_mngr) < 0) {
		LOGE("load subtpls from flash failed.");
		return -1;
	}

	// sort
	if (fids_freq_sort(tpl_mngr) < 0) {
		LOGE("sort fids failed.");
		return -2;
	}

	subtpl_load_latest_updated_record(tpl_mngr);

	LOGD("fids_used %d, subtpls_used %d", tpl_mngr->fids_used, tpl_mngr->subtpls_used);
	
	return 0;
}


