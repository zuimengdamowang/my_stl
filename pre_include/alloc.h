//
// Created by boyuan on 2022/4/14.
//

#ifndef TINYSTL_ALLOC_H
#define TINYSTL_ALLOC_H
#include <cstdlib>


namespace tt{


    // 内部空间配置器，以字节数为单位分配
    class alloc {
    private:
        enum EALIGN {ALIGN = 8};
        enum EMaxBytes {MAXBYTES = 128};
        enum EFreeListNums {FREELISTNUM = (EMaxBytes::MAXBYTES / EALIGN::ALIGN)};
        enum EObjNums {ADDOBJNUM = 20};

    private:
        //free-lists的节点构造
        union obj {
            union obj *next;
            char client[1];
        };
        // 定义free-lists
        static obj *free_lists[EFreeListNums::FREELISTNUM];

    private:  // 内存池
        static char *start_free;
        static char *end_free;
        static size_t heap_size;

    private:
        //将bytes上调至8的倍数
        static size_t ROUND_UP(size_t bytes) {
            return (bytes + EALIGN::ALIGN - 1) & ~(EALIGN::ALIGN - 1);
        }

        //根据区块大小，决定使用第i号free-list，i从0开始计算
        static size_t FREELIST_INDEX(size_t bytes) {
            return (bytes + EALIGN::ALIGN - 1) / EALIGN::ALIGN - 1;
        }

        //返回一个大小为n的对象，并可能加入大小为n的其他区块到free-list
        static void *refill(size_t bytes);

        //配置一大块空间，可容纳nobjs个大小为size的区块
        static char *chunk_alloc(size_t size, size_t& nobjs);

    public:
        static void *allocate(size_t bytes);
        static void deallocate(void *ptr, size_t bytes);
        static void *reallocate(void *pte, size_t old_size, size_t new_size);

    };


    char* alloc::start_free = nullptr;
    char* alloc::end_free   = nullptr;
    size_t alloc::heap_size = 0;
    alloc::obj* alloc::free_lists[alloc::EFreeListNums::FREELISTNUM] = {0};

    void alloc::deallocate(void *ptr, size_t bytes) {    // 指针，和指针所指内存的大小
        if (bytes > EMaxBytes::MAXBYTES) {
            free(ptr);
            return;
        }
        // 若是二级空间配置器生成的，则直接交还给free-list
        obj **list = free_lists + FREELIST_INDEX(bytes);
        ((obj *)ptr)->next = *list;
        *list = (obj *)ptr;
    }

    void* alloc::reallocate(void *ptr, size_t old_size, size_t new_size) {
        deallocate(ptr, old_size);
        return allocate(new_size);
    }

    void* alloc::allocate(size_t bytes) {
        if (bytes > EMaxBytes::MAXBYTES) {   // 一级分配器
            return malloc(bytes);
        }
        // 二级分配器
        size_t idx = FREELIST_INDEX(bytes);
        obj *list = free_lists[idx];
        if (list) {   // 有剩余的空间
            free_lists[idx] = list->next;
            return list;
        }else {  // 没有剩余的空间，需要重新开辟，然后返回
            return refill(ROUND_UP(bytes));
        }
    }

    void* alloc::refill(size_t bytes) {
        size_t num_objs = EObjNums::ADDOBJNUM;
        // 从内存池里取出num_objs个节点, 每个节点的大小是bytes
        char *chunk = chunk_alloc(bytes, num_objs);
        // chunk 是起始地址，num_objs是大小
        if (num_objs == 1) {   //取出的空间只够一个对象使用
            return chunk;
        } else {  // 如果返回的多于一个，则返回一个，然后把剩余的插入到free_list中
            void *ans = (void *)chunk;
            obj **list = free_lists + FREELIST_INDEX(bytes);
            obj *cur_obj = nullptr, *next_obj = (obj *)(chunk + bytes);
            *list = next_obj;
            for (int i = 1; ; ++i) {
                cur_obj = next_obj;
                next_obj = (obj *)((char *)next_obj + bytes);
                if (num_objs - 1 == i) {
                    cur_obj->next = nullptr;
                    break;
                }else {
                    cur_obj->next = next_obj;
                }
            }
            return ans;
        }

    }

    char* alloc::chunk_alloc(size_t bytes, size_t &nobjs) {
        char *ans = nullptr;

        size_t total_size = bytes * nobjs;
        size_t bytes_left = end_free - start_free;
        if (bytes_left >= total_size) {  //内存池剩余空间完全满足需要
            ans = start_free;
            start_free = start_free + total_size;
            return ans;
        } else if (bytes_left >= bytes) {  //内存池剩余空间不能完全满足需要，但足够供应一个或以上的区块
            nobjs = bytes_left / bytes;  // 计算能够满足几个节点
            size_t total = bytes * nobjs;
            ans = start_free;
            start_free = start_free + total;
            return ans;
        }else {  //内存池剩余空间连一个区块的大小都无法提供
            if (bytes_left > 0) {  // 将剩余内存挂到free_list
                obj **list = free_lists + FREELIST_INDEX(bytes_left);
                ((obj *)start_free)->next = *list;
                *list = (obj *)start_free;
            }
            // 重新给内存池开辟空间
            size_t bytes_to_get = 2 * total_size + ROUND_UP(heap_size >> 4);  // 重新开辟的内存大小
            start_free = (char *) malloc(bytes_to_get);  // 开辟空间
            if (!start_free) {  // 如果开辟空间失败，则看看free-list中有没有比bytes大的空闲块
                for (int i = 0; i <= EMaxBytes::MAXBYTES; i += EALIGN::ALIGN) {
                    obj **list = free_lists + FREELIST_INDEX(i);
                    obj *p = *list;
                    if (p) {
                        *list = p->next;
                        start_free = (char *)p;
                        end_free = start_free + i;
                        return chunk_alloc(bytes, nobjs);
                    }
                }
            }
            heap_size += bytes_to_get;
            end_free = start_free + heap_size;
            return chunk_alloc(bytes, nobjs);
        }
    }


}  // namespace tt



#endif //TINYSTL_ALLOC_H
