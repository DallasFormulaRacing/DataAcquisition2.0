/*
* Mutex Abstract Interface
* Author:   Cristian Cruz
*
* Email:    cris14.alex@gmail.com
*
* (c) 2024 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

#ifndef IMUTEX_H
#define IMUTEX_H



namespace application {

class IMutex {
public:
	virtual ~IMutex() = default;

	virtual void Lock() = 0;

	virtual void Unlock() = 0;
};

} // namespace application

#endif // IMUTEX_H
