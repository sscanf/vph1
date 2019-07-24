#define BUFFER_SIZE  7

#ifdef __cplusplus

class CBuffer
{
public:
	~CBuffer(void);
	CBuffer(void);
	CBuffer(int len);

	void Init (int len = 10);
	unsigned char kbhit();
	void put (char Chr);
	char get();
	int GetLen();

	char operator[] (char pos);
	
protected:

	char *_ptrBuffer;
	int _next_in;
	int _next_out;
};
#endif
