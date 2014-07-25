#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <tbb/spin_mutex.h>
#include <stdio.h>
#include <vector>
#include <stdarg.h>

using namespace std;
using namespace tbb;


#ifdef _DEBUG
static int CAKE_DebugLevel = 1;
#endif

//#define _DEBUG_LEVEL 0

#ifndef _DEBUG
#define _DEBUG_LEVEL -1 
#else 
#define _DEBUG_LEVEL 1
#endif

class Debug{
  public:
  Debug(int level)
#ifdef _DEBUG
    //:m_output(level <= CAKE_DebugLevel)
    :m_output(level <= _DEBUG_LEVEL)
#endif
    {}  
    void dbg_printf(const char *fmt, ...){   
#ifdef _DEBUG
      if(m_output){   
        p_mutex.lock();
        va_list ap;
        va_start(ap, fmt);
        vfprintf(stderr,fmt, ap);
        va_end(ap);
        p_mutex.unlock();
        //return *this;
      }   
#endif
    }   
    //Changed added static :Ankit
    void dbg_printfv(int s_id, std::vector<unsigned int>& vec){
#ifdef _DEBUG
      if(m_output){   
        p_mutex.lock();
        cout<<"{"<<s_id<<"}";
        //Debug<<"{"<<s_id<<"}";
        cout<<"{";
        for(int i = 0; i < vec.size(); ++i){
          std::cout<<vec[i]<<" ";
          //Debug<<vec[i]<<" ";
        }
        cout<<"}\n";
        p_mutex.unlock();
        //return *this;
      }   
#endif
    }   
  template<typename T>
    Debug& operator<<( T t){   
#ifdef _DEBUG
      if(m_output){   
        //p_mutex.lock();
        std::cout << t;
        //p_mutex.unlock();
        return *this;
      }   
      else
#endif
        return *this;
    }   
  private:
#ifdef _DEBUG
    bool m_output;
    spin_mutex p_mutex;
#endif
};
#endif  //#ifdef _DEBUG_H_
