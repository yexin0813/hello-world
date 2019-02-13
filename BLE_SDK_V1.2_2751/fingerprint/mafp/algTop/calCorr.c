#include "algorithm.h"
#include <string.h>

const int valList[] = {
	1024,1026,1028,1030,1032,1034,1036,1038,1040,1042,1044,1046,1049,1051,1053,1055,
	1057,1059,1061,1063,1066,1068,1070,1072,1074,1077,1079,1081,1083,1085,1088,1090,
	1092,1095,1097,1099,1101,1104,1106,1108,1111,1113,1116,1118,1120,1123,1125,1128,
	1130,1132,1135,1137,1140,1142,1145,1147,1150,1152,1155,1157,1160,1163,1165,1168,
	1170,1173,1176,1178,1181,1183,1186,1189,1192,1194,1197,1200,1202,1205,1208,1211,
	1214,1216,1219,1222,1225,1228,1231,1234,1237,1239,1242,1245,1248,1251,1254,1257,
	1260,1263,1266,1269,1273,1276,1279,1282,1285,1288,1291,1295,1298,1301,1304,1307,
	1311,1314,1317,1321,1324,1327,1331,1334,1337,1341,1344,1348,1351,1355,1358,1362,
	1365,1369,1372,1376,1380,1383,1387,1391,1394,1398,1402,1406,1409,1413,1417,1421,
	1425,1429,1432,1436,1440,1444,1448,1452,1456,1460,1464,1469,1473,1477,1481,1485,
	1489,1494,1498,1502,1507,1511,1515,1520,1524,1529,1533,1538,1542,1547,1551,1556,
	1560,1565,1570,1574,1579,1584,1589,1594,1598,1603,1608,1613,1618,1623,1628,1633,
	1638,1644,1649,1654,1659,1664,1670,1675,1680,1686,1691,1697,1702,1708,1713,1719,
	1725,1730,1736,1742,1748,1753,1759,1765,1771,1777,1783,1789,1796,1802,1808,1814,
	1820,1827,1833,1840,1846,1853,1859,1866,1872,1879,1886,1893,1900,1907,1913,1920,
	1928,1935,1942,1949,1956,1964,1971,1978,1986,1993,2001,2009,2016,2024,2032,2040
};


int calCorrFull(uint8 *imgBkg, uint8 *imgRaw)
{
	uint8 *pImg1, *pImg2;
	int row,col, i, tmp;
	int sum1, sum2;
	int corr;
	int val[120];

	memset(val, 0, 120*sizeof(int));
	pImg1 = imgBkg;
	pImg2 = imgRaw;
    
	for(col=0;col<120;col++)
	{
		sum1 = 0; 
		sum2 = 0; 
		for(row=0;row<120;row++)
		{
			tmp = (int)valList[(int)pImg2[0]] - (int)valList[(int)pImg1[0]]; //1016
			sum1 += (tmp*tmp); //不超过1016*1016*120 < 2^31-1
			sum2 += tmp; // 不超过120*1016
			pImg1+=120;
			pImg2+=120;
		}
		pImg1-=14399;
		pImg2-=14399;
		tmp = ((sum1>>3)-(sum2>>2)*(sum2>>1)/120); // sum2*sum2 = 1016*120*1016*122 < (2^31-1)*2^3, 
		if(col==0)
		{
			val[0] = tmp;
			continue;
		}
		for(row=0;row<=col;row++)
		{
			if(tmp>=val[row])
			{
				for(i=col;i>row;i--)
				{
					val[i] = val[i-1];
				}
				val[row] = tmp;
				break;
			}
		}
	}
	corr = 0;
	for(i=20;i<120;i++)
	{
		corr += val[i]; // 小于1016*1016*120*100/8 < 2^31-1
	}

	return corr;
}


void calCorrList(uint8 *imgBkg, uint8 *imgRaw, int *corrList)
{
	uint8 *pImg1, *pImg2;
	int row,col, i,j;
	int tmp;
	int sum1, sum2;
	int *corr;

	pImg1 = imgBkg;
	pImg2 = imgRaw;
	corr = corrList;
	for(col=0;col<6;col++)
	{
		for(row=0;row<6;row++)
		{
			sum1 = 0; // 2^20*120
			sum2 = 0; // 2^10*120
			for(i=0;i<20;i++)
			{
				for(j=0;j<5;j++)
				{
					tmp = (int)valList[(int)pImg2[0]] - (int)valList[(int)pImg1[0]];
					sum1 += (tmp*tmp);
					sum2 += tmp;
					tmp = (int)valList[(int)pImg2[1]] - (int)valList[(int)pImg1[1]];
					sum1 += (tmp*tmp);
					sum2 += tmp;
					tmp = (int)valList[(int)pImg2[2]] - (int)valList[(int)pImg1[2]];
					sum1 += (tmp*tmp);
					sum2 += tmp;
					tmp = (int)valList[(int)pImg2[3]] - (int)valList[(int)pImg1[3]];
					sum1 += (tmp*tmp);
					sum2 += tmp;

					pImg1+=4;
					pImg2+=4;
				}
				pImg1+=100;
				pImg2+=100;
			}
			sum2 /= 20; // over flow fixed
			sum2 *= sum2;
			*corr = sum1 - sum2;
			corr++;
		}
		pImg1 -= 14380;
		pImg2 -= 14380;
	}
}


// 1 - pressed, 0 - no press
int checkPress(uint8 *imgBkg, uint8 *imgRaw, int corrFullBkg)
{
	int corrFullRaw;
	corrFullRaw = calCorrFull(imgBkg, imgRaw);
	if( corrFullRaw > (corrFullBkg<<1) + 50000 )
		return 1;
	else
		return 0;
}


// cover area, 0 - 36
int getTouchArea(uint8 *imgBkg, uint8 *imgRaw, int *corrListBkg)
{
	int i;
	int cnt;
	int corrListRaw[36];
	calCorrList(imgBkg, imgRaw, corrListRaw);
	cnt = 0;
	for(i=0;i<36;i++)
	{
		if ( corrListBkg[i]!=0 && ( corrListRaw[i] > (corrListBkg[i]<<1) + 5000 ) )
		{
			cnt++;
		}
	}
	return cnt;
}
