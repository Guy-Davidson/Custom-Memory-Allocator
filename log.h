#ifndef CUSTOMMEMALLOCATOR_LOG_H
#define CUSTOMMEMALLOCATOR_LOG_H

#define CUSTOMMEMALLOCATOR_PRINT(...) printf(__VA_ARGS__)

#define LOG_ERROR(messege) CUSTOMMEMALLOCATOR_PRINT("Error: %s\n\n", messege)

#ifdef INFO 

#define LOG_INFO(messege) CUSTOMMEMALLOCATOR_PRINT("%s\n\n", messege)

#else

#define LOG_INFO(messege) 

#endif

#ifdef DEBUG

#define LOG_DEBUG(messege) CUSTOMMEMALLOCATOR_PRINT("%s\n\n", messege)

#else

#define LOG_DEBUG(messege)

#endif

#endif //CUSTOMMEMALLOCATOR_LOG_H