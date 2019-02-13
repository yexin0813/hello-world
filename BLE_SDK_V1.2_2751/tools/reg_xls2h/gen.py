import reg_xls2h
import os
import sys

xlsname = os.path.normpath(sys.argv[1]) #"E:/Apollo/svn/working_dir/ip/ble/ll/import/reg/audio/rw-ble-core-reg.xls"
blockname = sys.argv[2] #"BLECORE"
regaddr = sys.argv[3] #"0x20100000"
regaccess = sys.argv[4] #"REG_BLE"
regprefix = sys.argv[5] #"BLE"
regformat = sys.argv[6] #"short"
sheetname = sys.argv[7] #"Registers"
output = os.path.normpath(sys.argv[8])
output_dir = os.path.dirname(output)
if(os.path.exists(output_dir)==False):
    os.makedirs(output_dir)
#print(xlsname)
#print(blockname)
#print(regaddr)
#print(regaccess)
#print(regprefix)
#print(regformat)
#print(sheetname)
#print(output)
regs = reg_xls2h.reg_xls2h(xlsname, sheetname, regaccess, regprefix, verbose=False)
regs.read_xls()
regs.gen_header("", regaddr, output, False,  regformat == "long")