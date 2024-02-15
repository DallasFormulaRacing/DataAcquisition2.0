
#ifndef USB_DATA_LOGGER_H
#define USB_DATA_LOGGER_H

#include "fatfs.h"
#include "string.h"
#include "stdio.h"

namespace application {

class FatFs {

public:

	FatFs(char USBHPath[4], FATFS& USBHFatFS, FIL& USBHFile);

	~FatFs();

	/* mounts the USB*/
	void Mount_USB (void);

	/* unmounts the USB*/
	void Unmount_USB (void);

	/* Start node to be scanned (***also used as work area***) */
	FRESULT Scan_USB (char* pat);

	/* Only supports removing files from home directory. Directory remover to be added soon */
	FRESULT Format_USB (void);

	/* write the data to the file
	 * @ name : is the path to the file*/
	FRESULT Write_File (char *name, char *data);

	/* read data from the file
	 * @ name : is the path to the file*/
	FRESULT Read_File (char *name);

	/* creates the file, if it does not exists
	 * @ name : is the path to the file*/
	FRESULT Create_File (char *name);

	/* Removes the file from the USB
	 * @ name : is the path to the file*/
	FRESULT Remove_File (char *name);

	/* creates a directory
	 * @ name: is the path to the directory
	 */
	FRESULT Create_Dir (char *name);

	/* checks the free space in the USB*/
	void Check_USB_Details (void);

	/* updates the file. write pointer is set to the end of the file
	 * @ name : is the path to the file
	 */
	FRESULT Update_File (char *name, char *data);

private:
	char* USBHPath_;   /* USBH logical drive path */
	FATFS& USBHFatFS_;    /* File system object for USBH logical drive */
	FIL& USBHFile_;       /* File object for USBH */

	FILINFO USBHfno_;
	FRESULT fresult_;  // result
	UINT br_, bw_;  // File read/write count

	/**** capacity related *****/
	FATFS *pUSBHFatFS_;
	DWORD fre_clust_;
	uint32_t total_, free_space_;
};


} // namespace application

#endif // USB_DATA_LOGGER_H
