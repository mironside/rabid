#ifndef _SNSTRING_H_
#define _SNSTRING_H_

#define INITIAL_BUFFER_SIZE   32

class snString
{
  private:
    unsigned int length;
    char *data;
    unsigned int allocatedSize;
    char initialBuffer[INITIAL_BUFFER_SIZE];
    bool resized;
    
  public:
    snString();
    snString(int size);
    snString(const char *text);
    snString(const snString& str);
    ~snString();

    int Length() const {return length;}
    const char* CString() const {return data;}

    void CheckResize(unsigned int size);
    void Resize(unsigned int newSize);

    void Append(const snString& str);
    void Append(const char c);
    void Append(const char *pChar);

    void Insert(unsigned int index, const char c);
    void Insert(unsigned int index, const char *pChar);
    void Insert(unsigned int index, const snString& str);

    void Remove(unsigned int index);

    snString& operator=(const char *text);
    snString& operator=(const snString& text);

    bool operator==(const snString& text);
    bool operator==(const char *text);

    char& operator[](unsigned int i);
    //char operator[](unsigned int i);
    const char operator[](unsigned int i) const;

};


#endif/*_SNSTRING_H_*/

