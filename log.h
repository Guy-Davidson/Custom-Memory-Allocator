#ifndef CUSTOMMEMALLOCATOR_LOG_H
#define CUSTOMMEMALLOCATOR_LOG_H

#define CUSTOMMEMALLOCATOR_PRINT(...) printf(__VA_ARGS__)

#define LOG_ERROR(messege) CUSTOMMEMALLOCATOR_PRINT("Error: %s\n", messege)

#ifdef INFO 

#define LOG_INFO(messege) CUSTOMMEMALLOCATOR_PRINT("%s\n", messege)

#define LOG_BLOCK_INFO(mode, size, next) CUSTOMMEMALLOCATOR_PRINT("mode:%c, size:%d, next:%d.\n", mode, size, next)

#else

#define LOG_INFO(messege) 

#endif


#endif //CUSTOMMEMALLOCATOR_LOG_H