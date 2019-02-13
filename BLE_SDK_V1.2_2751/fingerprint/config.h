#ifndef __COMP_CONFIG_H__
#define __COMP_CONFIG_H__


#define Standard_Coating                    1

#define SYNO_Coating                        11

#define YALE_Coating                        21

#define COMPLING_OPTION                     Standard_Coating

#define VERSION_NUM                         {'1','8','1','2','1','8', '0' + COMPLING_OPTION / 10, '0' + COMPLING_OPTION % 10}



#define PROTOCOL_MICROARRAY                     (0x01)
#define PROTOCOL_MICROARRAY_COMPATIBLE_SYNOCHIP (0x02)
#define PROTOCOL_MICROARRAY_COMPATIBLE_YALE     (0x03)


#if (COMPLING_OPTION == Standard_Coating)
	#define PROTOCOL                    PROTOCOL_MICROARRAY
	#define COATING_VERSION             1
	#define DEFAULT_ENROLL_TIMES        4
#elif (COMPLING_OPTION == Standard_Cover)
	#define PROTOCOL                    PROTOCOL_MICROARRAY
	#define COATING_VERSION             0
	#define DEFAULT_ENROLL_TIMES        4

#elif (COMPLING_OPTION == SYNO_Coating)
	#define PROTOCOL                    PROTOCOL_MICROARRAY_COMPATIBLE_SYNOCHIP
	#define COATING_VERSION             1
	#define DEFAULT_ENROLL_TIMES        4

#elif (COMPLING_OPTION == SYNO_Cover)
	#define PROTOCOL                    PROTOCOL_MICROARRAY_COMPATIBLE_SYNOCHIP
	#define COATING_VERSION             0
	#define DEFAULT_ENROLL_TIMES        4

#elif (COMPLING_OPTION == YALE_Coating)
	#define PROTOCOL                    PROTOCOL_MICROARRAY_COMPATIBLE_YALE
	#define COATING_VERSION             1
	#define DEFAULT_ENROLL_TIMES        4

#elif (COMPLING_OPTION == YALE_Cover)
	#define PROTOCOL                    PROTOCOL_MICROARRAY_COMPATIBLE_YALE
	#define COATING_VERSION             0
	#define DEFAULT_ENROLL_TIMES        4

#else
	#error "!!!!! Unknown Configuration !!!!!"

#endif

#endif

