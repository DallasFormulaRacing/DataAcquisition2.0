#include "mbed.h"
#include "rtos.h"

Thread read_thread;
Ticker write_interrupt;
Timeout read_timeout;
bool terminate_read_flag =  false;

int write_buffer_end_idx = 0;
Mutex buffer_lock;


std::chrono::milliseconds task1_timeout = 1000ms;
std::chrono::milliseconds task2_timeout = 1000ms;
std::chrono::milliseconds task3_timeout = 1000ms;
std::chrono::milliseconds task4_timeout = 1000ms;

void write_sd(){
    read_timeout.detach();
    terminate_read_flag = true;

    // __disable_irq(); //if needs to be completely separate this will prevent from context switching to read_task 
        //SD Card Write.
        //Might need to lock buffer end index, 
    buffer_lock.lock();
    // write_buffer_end_idx value change;
    buffer_lock.unlock();

    //Can make buffer as linkedlist, so no need to worry about index.

    // __enable_irq(); 


}
void read_task1(){
    int read_data_size = 0;
    //Read, set data size
    //Try increasing buff idx. If write in progress discard this data.
    if(!buffer_lock.trylock()) return;
    buffer_lock.unlock();
    write_buffer_end_idx += read_data_size;
}
void read_task2(){
}
void read_task3(){
}
void read_task4(){
}

void read_task_dispatcher();

int main(){
    write_interrupt.attach(&write_sd, 4000ms);
    read_task_dispatcher();
}

void read_task_terminate(){
    terminate_read_flag = true;
}

void read_task_dispatcher(){
    while(true){
        read_timeout.attach(&read_task_terminate, task1_timeout);
        read_thread.start(read_task1);
        // read_thread.join(); Won't work because cannot terminate thread in ISR context.
        while(read_thread.get_state() == rtos::Thread::Running && !terminate_read_flag);
        
        if(terminate_read_flag && read_thread.get_state() == rtos::Thread::Running){
            read_thread.terminate();
        }

        read_timeout.detach();
        terminate_read_flag = false;
        
       

        read_timeout.attach(&read_task_terminate, task2_timeout);
        read_thread.start(read_task2);

        while(read_thread.get_state() == rtos::Thread::Running && !terminate_read_flag);
        if(terminate_read_flag && read_thread.get_state() == rtos::Thread::Running){
            read_thread.terminate();
        }

        read_timeout.detach();
        terminate_read_flag = false;



        read_timeout.attach(&read_task_terminate, task3_timeout);
        read_thread.start(read_task3);

        while(read_thread.get_state() == rtos::Thread::Running && !terminate_read_flag);
        if(terminate_read_flag && read_thread.get_state() == rtos::Thread::Running){
            read_thread.terminate();
        }

        read_timeout.detach();
        terminate_read_flag = false;



        read_timeout.attach(&read_task_terminate, task4_timeout);
        read_thread.start(read_task4);

        while(read_thread.get_state() == rtos::Thread::Running && !terminate_read_flag);
        if(terminate_read_flag && read_thread.get_state() == rtos::Thread::Running){
            read_thread.terminate();
        }

        read_timeout.detach();
        terminate_read_flag = false;
    }
}
