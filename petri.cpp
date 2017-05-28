#include "petri.h"

static const unsigned char petri_count = 20;
static unsigned char petri_current_count = 0;
static volatile bool petri_exit = false;
static volatile bool petri_enable = false;
static volatile bool petri_dog_food = false;
std::mutex petri_watch_dog_guard;
std::mutex petri_memory_guard;
static char *petri_memory = NULL;
static unsigned char petri_memory_size = 100;
static const unsigned __int64 petri_init = 0x8080C08080;
static const unsigned __int64 petri_entries[20] = {0xC, 0x92, 0xF0, 0x1,
                                                   0xC00, 0x9200, 0xF000, 0x100,
                                                   0x300000, 0x70000, 0x890000, 0xC00000,
                                                   0xC000000, 0x92000000, 0xF0000000, 0x1000000,
                                                   0xC00000000, 0x9200000000, 0xF000000000, 0x100000000};
static const unsigned __int64 petri_exits[20] = {0x10, 0x4, 0x0, 0x60,
                                             0x1000, 0x400, 0x1, 0x6000,
                                             0x400000, 0xD00000, 0x280000, 0x2,
                                             0x10000000, 0x4000000, 0x4, 0x60000000,
                                             0x1000000000, 0x400000000, 0x8, 0x6000000000};
static const unsigned char petri_transition = 2;

petriBase::petriBase() {
    chips = petri_init;
}

petriBase::petriBase(unsigned __int64 key){
    key &=~(0xFFFFFFF0F0F0F0FF);
    chips = petri_init;
    chips = chips | key;
}

petriBase::~petriBase() {}

void petriBase::run() {
     end = false;
     for (unsigned char i = 0; i < petri_count; i++) {
         transitionsAccess.push_back(false);
         std::thread *t = new std::thread(worker, this, i);
         transitions.push_back(t);
     }
     bool cont = false;
     for (;;) {
         for (unsigned char i = 0; i < petri_count; i++) {
             if ((chips & petri_entries[i]) == petri_entries[i]) {
                transitionsAccess[i] = true;
                while(transitionsAccess[i]) {
                    std::this_thread::sleep_for(std::chrono::microseconds(1));
                }
                cont = true;
                break;
             }
         }
         if (cont) {
             cont = false;
             continue;
         }
         break;
     }
     end = true;
     for (unsigned int i = 0; i < transitions.size(); i++) {
         transitions[i]->join();
         delete transitions[i];
     }
     transitions.clear();
     over();
}

void petriBase::worker(unsigned char num) {
    prepare(num);
    petri_memory_guard.lock();
    petri_current_count++;
    delete [] petri_memory;
    srand(time(NULL));
    petri_memory_size = rand() % 100 + 2;
    petri_memory = new char[petri_memory_size];
    petri_memory_guard.unlock();
    while(!end) {
        if (transitionsAccess[num]) {
            chips &=~(petri_entries[num]);
            chips |= petri_exits[num];
            action(num);
            transitionsAccess[num]= false;
        } else {
           std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
    }
}



petriOut::petriOut() : petriBase() {
    next = NULL;
    exit = NULL;
    argc = 0;
    argv = NULL;
    begin = false;
}

petriOut::~petriOut() {}

petriOut::petriOut(void (*currentNext)(unsigned __int64, int, char **),
                                   void (*currentExit)(int, char **),
                                   int currentArgc,
                                   char **currentArgv,
                                   unsigned __int64 key) : petriBase(key),
                                                                       next(currentNext),
                                                                       exit(currentExit),
                                                                       argc(currentArgc),
                                                                       argv(currentArgv),
                                                                       nextKey(key) {
    begin = false;
}

void petriOut::prepare(unsigned char num) {
    std::lock_guard<std::mutex> locker(nextKeyGuard);
    nextKey += 6533 + (unsigned __int64)(num * 3) - (unsigned __int64)num;
}

void petriOut::action(unsigned char num) {
    if (petri_transition == num) {
        begin = true;
        std::thread dog(petri::WatchDog);
        dog.detach();
        nextKey += (unsigned __int64)(32);
        std::thread t(next, nextKey, argc, argv);
        t.detach();
    }
}

void petriOut::over() {
    if (!begin) {
        std::thread t(exit, argc, argv);
        t.join();
    } else {
        while(!petri_exit) {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    }
}

void petri::Main(unsigned __int64 key,
                      void(*currentNext)(unsigned __int64, int, char **),
                      void (*currentExit)(int, char **),
                      int currentArgc,
                      char **currentArgv) {

    petri_memory = new char [10];
    petri_enable = true;
    petri_exit = false;
    petri_current_count = 0;
    petriOut currentPetri(currentNext, currentExit, currentArgc, currentArgv, key);
    currentPetri.run();
}

void petri::Exit() {
    petri_exit = true;
    delete [] petri_memory;
}

void petri::Check() {
    srand(time(NULL));
    petri_memory[rand() % petri_memory_size] = 1;
    petri_watch_dog_guard.lock();
    petri_dog_food = true;
    unsigned char i = 0;
    while(petri_dog_food) {
        if(i > 100) {
            char *ptr = NULL;
            *ptr = 100;
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        i++;
    }
    petri_watch_dog_guard.unlock();
    petri_memory[rand() % petri_memory_size] = 1;
}

void petri::WatchDog() {
    unsigned int size = petri_memory_size;
    while(!petri_exit) {
        if(petri_count != petri_current_count || !petri_enable || size != petri_memory_size) {
            char *ptr = NULL;
            *ptr = 100;
        }
        if (petri_dog_food) {
            petri_dog_food = false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
