#ifndef PETRI_H
#define PETRI_H

#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <ctime>


class petriBase {
protected:
    unsigned __int64 chips;
    volatile bool end;
    std::vector<bool> transitionsAccess;
    std::vector<std::thread *> transitions;
    void worker(unsigned char num);
    virtual void prepare(unsigned char num) = 0;
    virtual void action(unsigned char num) = 0;
    virtual void over() = 0;
public:
    petriBase();
    petriBase(unsigned __int64 key);
    virtual ~petriBase();
    void run();
};



class petriOut : public petriBase {
private:
    void (*next)(unsigned __int64, int, char **);
    void (*exit)(int, char **);
    int argc;
    char **argv;
    void prepare(unsigned char num);
    void action(unsigned char num);
    void over();
    bool begin;
    unsigned __int64 nextKey;
    std::mutex nextKeyGuard;
public:
    petriOut();
    ~petriOut();
    petriOut(void (*currentNext)(unsigned __int64, int, char **),
                     void (*currentExit)(int, char **),
                     int currentArgc,
                     char **currentArgv,
                     unsigned __int64 key);
};

class petri {
public:
    static void Main(unsigned __int64 key,
                   void(*currentNext)(unsigned __int64, int, char **),
                   void (*currentExit)(int, char **),
                   int currentArgc,
                   char **currentArgv);
    static void WatchDog();
    static void Check();
    static void Exit();
};



#endif // PETRI_H
