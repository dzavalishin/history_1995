#ifndef fstream_h
#define fstream_h

#include <iostream.h>

//#pragma pack(4)

// a stream buffer for strings
class  stringbuf : public streambuf
    {     
    protected:
        string &s_v;
           //static const int openprot ; /* default protection for open */
    public:
        stringbuf(string &s) : s_v(s) {}
        ~stringbuf() ;
        
   public: /* virtuals */
           virtual int     overflow(int=EOF);
           virtual int     underflow();
           virtual int     sync() ;
           virtual streampos
                           seekoff(streamoff,ios::seek_dir,int) ;
           virtual streambuf*
                           setbuf(char*  p, int len) ;
   };

class string_streambase : virtual public ios
    {
    public:
        string_streambase(string &s) : buf(s) {}
        ~string_streambase() ;
        //void            setbuf(char*  p, int l) ;
        //filebuf*        rdbuf() { return &buf ; }
    private:
        stringbuf         buf ;
    protected:
        void            verify(int) ;
    } ;

   class isstream : public fstreambase, public istream {
   public:
                           ifstream() ;
                           ifstream(const char* name,
                                           int mode=ios::in,
                                           int prot=filebuf::openprot) ;
                           ifstream(int fd) ;
                           ifstream(int fd, char*  p, int l) ;
                           ~ifstream() ;

           filebuf*        rdbuf() { return fstreambase::rdbuf(); }
           void            open(const char* name, int mode=ios::in,
                                           int prot=filebuf::openprot) ;
   } ;

   class ofstream : public fstreambase, public ostream {
   public:
                           ofstream() ;
                           ofstream(const char* name,
                                           int mode=ios::out,
                                           int prot=filebuf::openprot) ;
                           ofstream(int fd) ;
                           ofstream(int fd, char*  p, int l) ;
                           ~ofstream() ;

           filebuf*        rdbuf() { return fstreambase::rdbuf(); }
           void            open(const char* name, int mode=ios::out,
                                           int prot=filebuf::openprot) ;
   } ;

   class fstream : public fstreambase, public iostream {
   public:
                           fstream() ;

                           fstream(const char* name,
                                           int mode,
                                           int prot=filebuf::openprot) ;
                           fstream(int fd) ;
                           fstream(int fd, char*  p, int l) ;
                           ~fstream() ;
           filebuf*        rdbuf() { return fstreambase::rdbuf(); }
           void            open(const char* name, int mode,
                                   int prot=filebuf::openprot) ;
   } ;

   #pragma pack()

#endif

#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( restore )
#endif
#pragma info( restore )

