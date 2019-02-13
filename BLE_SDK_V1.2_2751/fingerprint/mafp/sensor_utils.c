#include "sensor_utils.h"


int32_t sensor_get_grey(uint8_t* buf, uint32_t len)
{
        int32_t sum = 0;
    
        while (len--) sum += buf[len];

        return sum;
}


int32_t sensor_bkg_check(uint8_t* buf, uint32_t len, uint8_t grey_val, uint32_t th)
{
        int cnt = 0;

        while (len--) {
                if (buf[len] < grey_val) cnt++;
        }
        LOGD("cnt = %d", cnt);
        
        return (cnt <= th) ? 0 : -1;
}


uint8_t sensor_calc_greyoffset(uint8_t *buf, uint32_t len, uint8_t default_offset)
{
        int32_t i, num = 0, max = 0;
        uint8_t offset;

        for (i = 0; i < len; i++) {
                if(buf[i] != 255) {
                        num++;
            
                        if (buf[i] >= max) max = buf[i];
                }
        }
    
        if (num == 0)
                offset = default_offset;
        else
                offset = default_offset - (max>>1);

        return offset;
}


int32_t sensor_is_exposed(uint8_t *buf, uint32_t len, uint8_t greyoffset)
{
        int32_t i, ret = 0, num = 0;

        for ( i = 0; i < len; i++ ) {
                if (buf[i] == 255) num++;
        }
        
        if (num - greyoffset > 96) ret = 1;

        return ret;
}


