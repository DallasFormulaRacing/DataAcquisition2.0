#include "fat_fs.hpp"
#include "usart.h"

extern UART_HandleTypeDef huart3;
#define UART &huart3


namespace application {

void Send_Uart (const char *string)
{
	HAL_UART_Transmit(UART, (uint8_t *)string, strlen (string), HAL_MAX_DELAY);
}

FatFs::FatFs(char USBHPath[4], FATFS& USBHFatFS, FIL& USBHFile)
  : USBHPath_(USBHPath), USBHFatFS_(USBHFatFS), USBHFile_(USBHFile) { }

FatFs::~FatFs() {

}

void FatFs::Mount_USB (void) {
	fresult_ = f_mount(&USBHFatFS_, USBHPath_, 1);
	if (fresult_ != FR_OK) Send_Uart ("ERROR!!! in mounting USB ...\n\n");
	else Send_Uart("USB mounted successfully...\n");
}

void FatFs::Unmount_USB (void) {
	fresult_ = f_mount(NULL, USBHPath_, 1);
	if (fresult_ == FR_OK) Send_Uart ("USB UNMOUNTED successfully...\n\n\n");
	else Send_Uart("ERROR!!! in UNMOUNTING USB \n\n\n");
}

FRESULT FatFs::Scan_USB (char* pat) {
	DIR dir;
	UINT i;
	char *path = (char*)malloc(20*sizeof (char));
	sprintf (path, "%s",pat);

	fresult_ = f_opendir(&dir, path); /* Open the directory */

	if (fresult_ == FR_OK)
	{
		for (;;)
		{
			fresult_ = f_readdir(&dir, &USBHfno_);                   /* Read a directory item */
			if (fresult_ != FR_OK || USBHfno_.fname[0] == 0) {
				break;  /* Break on error or end of dir */
			}

			if (USBHfno_.fattrib & AM_DIR)     /* It is a directory */
			{

				if (!(strcmp ("SYSTEM~1", USBHfno_.fname))) {
					continue;
				}


				if (!(strcmp("System Volume Information", USBHfno_.fname))) {
					continue;
				}


				char *buf = (char*)malloc(30*sizeof(char));
				sprintf (buf, "Dir: %s\r\n", USBHfno_.fname);
				Send_Uart(buf);
				free(buf);
				i = strlen(path);
				sprintf(&path[i], "/%s", USBHfno_.fname);
				fresult_ = Scan_USB(path);                     /* Enter the directory */

				if (fresult_ != FR_OK) {
					break;
				}

				path[i] = 0;
			}
			else
			{   /* It is a file. */
			   char *buf = (char*)malloc(30*sizeof(char));
			   sprintf(buf,"File: %s/%s\n", path, USBHfno_.fname);
			   Send_Uart(buf);
			   free(buf);
			}
		}
		f_closedir(&dir);
	}


	free(path);
	return fresult_;

}

FRESULT FatFs::Format_USB (void) {

	DIR dir;
	char *path = (char*)malloc(20*sizeof (char));
	sprintf (path, "%s","/");

	fresult_ = f_opendir(&dir, path);                       /* Open the directory */
	if (fresult_ == FR_OK)
	{
		for (;;)
		{
			fresult_ = f_readdir(&dir, &USBHfno_);                   /* Read a directory item */

			if (fresult_ != FR_OK || USBHfno_.fname[0] == 0) {
				break;  /* Break on error or end of dir */
			}

			if (USBHfno_.fattrib & AM_DIR)     /* It is a directory */
			{
				if (!(strcmp ("SYSTEM~1", USBHfno_.fname))) {
					continue;
				}

				if (!(strcmp("System Volume Information", USBHfno_.fname))) {
					continue;
				}

				fresult_ = f_unlink(USBHfno_.fname);
				if (fresult_ == FR_DENIED) {
					continue;
				}
			}
			else
			{   /* It is a file. */
			   fresult_ = f_unlink(USBHfno_.fname);
			}
		}
		f_closedir(&dir);
	}
	free(path);
	return fresult_;

}

FRESULT FatFs::Write_File (char *name, char *data) {

	/**** check whether the file exists or not ****/
	fresult_ = f_stat (name, &USBHfno_);
	if (fresult_ != FR_OK)
	{
		char *buf = (char*)malloc(100*sizeof(char));
		sprintf (buf, "ERROR!!! *%s* does not exists\n\n", name);
		Send_Uart (buf);
		free(buf);
		return fresult_;
	}

	else
	{
		/* Create a file with read write access and open it */
		fresult_ = f_open(&USBHFile_, name, FA_OPEN_EXISTING | FA_WRITE);
		if (fresult_ != FR_OK)
		{
			char *buf = (char*)malloc(100*sizeof(char));
			sprintf (buf, "ERROR!!! No. %d in opening file *%s*\n\n", fresult_, name);
			Send_Uart(buf);
			free(buf);
			return fresult_;
		}

		else
		{
			char *buf = (char*)malloc(100*sizeof(char));
			sprintf (buf, "Opening file-->  *%s*  To WRITE data in it\n", name);
			Send_Uart(buf);
			free(buf);

			fresult_ = f_write(&USBHFile_, data, strlen(data), &bw_);
			if (fresult_ != FR_OK)
			{
				char *buf = (char*)malloc(100*sizeof(char));
				sprintf (buf, "ERROR!!! No. %d while writing to the FILE *%s*\n\n", fresult_, name);
				Send_Uart(buf);
				free(buf);
			}

			/* Close file */
			fresult_ = f_close(&USBHFile_);
			if (fresult_ != FR_OK)
			{
				char *buf = (char*)malloc(100*sizeof(char));
				sprintf (buf, "ERROR!!! No. %d in closing file *%s* after writing it\n\n", fresult_, name);
				Send_Uart(buf);
				free(buf);
			}
			else
			{
				char *buf = (char*)malloc(100*sizeof(char));
				sprintf (buf, "File *%s* is WRITTEN and CLOSED successfully\n\n", name);
				Send_Uart(buf);
				free(buf);
			}
		}
		return fresult_;
	}
}

FRESULT FatFs::Read_File (char *name) {

	/**** check whether the file exists or not ****/
	fresult_ = f_stat (name, &USBHfno_);
	if (fresult_ != FR_OK)
	{
		char *buf = (char*)malloc(100*sizeof(char));
		sprintf (buf, "ERRROR!!! *%s* does not exists\n\n", name);
		Send_Uart (buf);
		free(buf);
		return fresult_;
	}

	else
	{
		/* Open file to read */
		fresult_ = f_open(&USBHFile_, name, FA_READ);

		if (fresult_ != FR_OK)
		{
			char *buf = (char*)malloc(100*sizeof(char));
			sprintf (buf, "ERROR!!! No. %d in opening file *%s*\n\n", fresult_, name);
			Send_Uart(buf);
			free(buf);
			return fresult_;
		}

		/* Read data from the file
		* see the function details for the arguments */

		char *buf = (char*)malloc(100*sizeof(char));
		sprintf (buf, "Opening file-->  *%s*  To READ data from it\n", name);
		Send_Uart(buf);
		free(buf);

		char *buffer = (char*)malloc(sizeof(f_size(&USBHFile_)));
		fresult_ = f_read (&USBHFile_, buffer, f_size(&USBHFile_), &br_);
		if (fresult_ != FR_OK)
		{
			char *buf = (char*)malloc(100*sizeof(char));
			free(buffer);
			sprintf (buf, "ERROR!!! No. %d in reading file *%s*\n\n", fresult_, name);
			Send_Uart(buffer);
			free(buf);
		}

		else
		{
			Send_Uart(buffer);
			free(buffer);

			/* Close file */
			fresult_ = f_close(&USBHFile_);
			if (fresult_ != FR_OK)
			{
				char *buf = (char*)malloc(100*sizeof(char));
				sprintf (buf, "ERROR!!! No. %d in closing file *%s*\n\n", fresult_, name);
				Send_Uart(buf);
				free(buf);
			}
			else
			{
				char *buf = (char*)malloc(100*sizeof(char));
				sprintf (buf, "File *%s* CLOSED successfully\n\n", name);
				Send_Uart(buf);
				free(buf);
			}
		}
		return fresult_;
	}

}

FRESULT FatFs::Create_File (char *name) {

	fresult_ = f_stat (name, &USBHfno_);
	if (fresult_ == FR_OK)
	{
		char *buf = (char*)malloc(100*sizeof(char));
		sprintf (buf, "ERROR!!! *%s* already exists!!!!\n use Update_File \n\n",name);
		Send_Uart(buf);
		free(buf);
		return fresult_;
	}
	else
	{
		fresult_ = f_open(&USBHFile_, name, FA_CREATE_ALWAYS|FA_READ|FA_WRITE);
		if (fresult_ != FR_OK)
		{
			char *buf = (char*)malloc(100*sizeof(char));
			sprintf (buf, "ERROR!!! No. %d in creating file *%s*\n\n", fresult_, name);
			Send_Uart(buf);
			free(buf);
			return fresult_;
		}
		else
		{
			char *buf = (char*)malloc(100*sizeof(char));
			sprintf (buf, "*%s* created successfully\n Now use Write_File to write data\n",name);
			Send_Uart(buf);
			free(buf);
		}

		fresult_ = f_close(&USBHFile_);
		if (fresult_ != FR_OK)
		{
			char *buf = (char*)malloc(100*sizeof(char));
			sprintf (buf, "ERROR No. %d in closing file *%s*\n\n", fresult_, name);
			Send_Uart(buf);
			free(buf);
		}
		else
		{
			char *buf = (char*)malloc(100*sizeof(char));
			sprintf (buf, "File *%s* CLOSED successfully\n\n", name);
			Send_Uart(buf);
			free(buf);
		}
	}
	return fresult_;
}

FRESULT FatFs::Remove_File (char *name) {

	fresult_ = f_stat (name, &USBHfno_);
	if (fresult_ == FR_OK)
	{
		char *buf = (char*)malloc(100*sizeof(char));
		sprintf (buf, "ERROR!!! *%s* already exists!!!!\n use Update_File \n\n",name);
		Send_Uart(buf);
		free(buf);
		return fresult_;
	}
	else
	{
		fresult_ = f_open(&USBHFile_, name, FA_CREATE_ALWAYS|FA_READ|FA_WRITE);
		if (fresult_ != FR_OK)
		{
			char *buf = (char*)malloc(100*sizeof(char));
			sprintf (buf, "ERROR!!! No. %d in creating file *%s*\n\n", fresult_, name);
			Send_Uart(buf);
			free(buf);
			return fresult_;
		}
		else
		{
			char *buf = (char*)malloc(100*sizeof(char));
			sprintf (buf, "*%s* created successfully\n Now use Write_File to write data\n",name);
			Send_Uart(buf);
			free(buf);
		}

		fresult_ = f_close(&USBHFile_);
		if (fresult_ != FR_OK)
		{
			char *buf = (char*)malloc(100*sizeof(char));
			sprintf (buf, "ERROR No. %d in closing file *%s*\n\n", fresult_, name);
			Send_Uart(buf);
			free(buf);
		}
		else
		{
			char *buf = (char*)malloc(100*sizeof(char));
			sprintf (buf, "File *%s* CLOSED successfully\n\n", name);
			Send_Uart(buf);
			free(buf);
		}
	}
	return fresult_;


}

FRESULT FatFs::Create_Dir (char *name) {

	fresult_ = f_mkdir(name);
	if (fresult_ == FR_OK)
	{
		char *buf = (char*)malloc(100*sizeof(char));
		sprintf (buf, "*%s* has been created successfully\n\n", name);
		Send_Uart (buf);
		free(buf);
	}
	else
	{
		char *buf = (char*)malloc(100*sizeof(char));
		sprintf (buf, "ERROR No. %d in creating directory *%s*\n\n", fresult_,name);
		Send_Uart(buf);
		free(buf);
	}
	return fresult_;

}

void FatFs::Check_USB_Details (void) {

	/* Check free space */
	f_getfree("", &fre_clust_, &pUSBHFatFS_);

	total_ = (uint32_t)((pUSBHFatFS_->n_fatent - 2) * pUSBHFatFS_->csize * 0.5);
	char *buf = (char*)malloc(30*sizeof(char));
	sprintf (buf, "USB  Total Size: \t%lu\n",total_);
	Send_Uart(buf);
	free(buf);
	free_space_ = (uint32_t)(fre_clust_ * pUSBHFatFS_->csize * 0.5);
	buf = (char*)malloc(30*sizeof(char));
	sprintf (buf, "USB Free Space: \t%lu\n",free_space_);
	Send_Uart(buf);
	free(buf);

}

FRESULT FatFs::Update_File (char *name, char *data) {

	/**** check whether the file exists or not ****/
	fresult_ = f_stat (name, &USBHfno_);
	if (fresult_ != FR_OK)
	{
		char *buf = (char*)malloc(100*sizeof(char));
		sprintf (buf, "ERROR!!! *%s* does not exists\n\n", name);
		Send_Uart (buf);
		free(buf);
		return fresult_;
	}

	else
	{
		 /* Create a file with read write access and open it */
		fresult_ = f_open(&USBHFile_, name, FA_OPEN_APPEND | FA_WRITE);
		if (fresult_ != FR_OK)
		{
			char *buf = (char*)malloc(100*sizeof(char));
			sprintf (buf, "ERROR!!! No. %d in opening file *%s*\n\n", fresult_, name);
			Send_Uart(buf);
			free(buf);
			return fresult_;
		}

		char *buf = (char*)malloc(100*sizeof(char));
		sprintf (buf, "Opening file-->  *%s*  To UPDATE data in it\n", name);
		Send_Uart(buf);
		free(buf);

		/* Writing text */
		fresult_ = f_write(&USBHFile_, data, strlen (data), &bw_);
		if (fresult_ != FR_OK)
		{
			char *buf = (char*)malloc(100*sizeof(char));
			sprintf (buf, "ERROR!!! No. %d in writing file *%s*\n\n", fresult_, name);
			Send_Uart(buf);
			free(buf);
		}

		else
		{
			char *buf = (char*)malloc(100*sizeof(char));
			sprintf (buf, "*%s* UPDATED successfully\n", name);
			Send_Uart(buf);
			free(buf);
		}

		/* Close file */
		fresult_ = f_close(&USBHFile_);
		if (fresult_ != FR_OK)
		{
			char *buf = (char*)malloc(100*sizeof(char));
			sprintf (buf, "ERROR!!! No. %d in closing file *%s*\n\n", fresult_, name);
			Send_Uart(buf);
			free(buf);
		}
		else
		{
			char *buf = (char*)malloc(100*sizeof(char));
			sprintf (buf, "File *%s* CLOSED successfully\n\n", name);
			Send_Uart(buf);
			free(buf);
		 }
	}
	return fresult_;


}


} // namespace application
