#ifndef _MEMORY_STREAM_H_
#define _MEMORY_STREAM_H_


#include <vector>
#include <stdio.h>
#include <string>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

class MemoryStream
{
    const static size_t DEFAULT_SIZE = 256;
    public:
        
        MemoryStream():mlength(0)
        {
            mdata.reserve(DEFAULT_SIZE);
        }
        
        MemoryStream(size_t size):mlength(0)
        {
            if(size)
            {
                mdata.reserve(size);
            }
        }

        virtual ~MemoryStream()
        {
            clear();
        }

        virtual void clear()
        {
            mdata.clear();
            mlength = 0;
        }

        bool                empty()     const   { return mdata.empty();}
        int                 size()      const   { return mdata.size(); }
        size_t              length()    const   { return mlength; }

        char*               data()              {return &mdata[0];}

        void resize(const size_t n)
        {
            if(size() < n){
                
                mdata.resize(n);
            }
        }


        void reserve(const size_t size)
        {
            if(size > mdata.size())
            {
                mdata.reserve(size);
            }
        }
        
        template <typename T> void append(T value)
        {
            append((const void*)&value, sizeof(T));
        }

        template<typename T> void append(const T* t, size_t size)
        {
            append((const void*)t, size*sizeof(T));
        }

        void append(const std::string& str)
        {
            append((const void*)str.c_str(), (size_t)(str.size() + 1));
        }

        void append(const char* data, size_t size)
        {
            append((const void*)data, size);
        }

        void append(const void* data, size_t size)
        {
            if(data == NULL)
            {
                return;
            }

            if(mdata.size() < mlength + size)
            {
                mdata.resize(mlength + size);
            }

            memcpy(&mdata[mlength], data, size);
            mlength += size;
        }


        template<typename T> T read(size_t offset) const
        {
            assert(sizeof(T) <= (mlength - offset));

            T t = *((T const *)&mdata[offset]);
            return t;
        }

        char* read(size_t offset)
        {
            if(offset >= mlength)
            {
                return NULL;
            }

            return (char*)&mdata[offset];
        }

    protected:
        std::vector<char> mdata;
        int mlength;

};


#endif
