#include <cstdint>
#include <pthread.h>
#include <sys/socket.h>

class GlipTcp {
public:
  GlipTcp(int port, int width);
  
  int reset();

  bool connected();

  uint32_t next_cycle();

  uint32_t control_msg();

  uint64_t readData();
  void readAck();

  void writeData(uint64_t data);

private:
  int mPort;
  int mNumBytes;
  pthread_t mThread;
  volatile bool mThreadReady;
  volatile bool mConnected;

  int mListenSocketData;
  int mListenSocketControl;

  int mSocketData;
  struct sockaddr mSocketAddressData;
  int mSocketAddressLengthData;
  int mSocketControl;
  struct sockaddr mSocketAddressControl;
  int mSocketAddressLengthControl;

  bool mHasCachedRead;
  uint64_t mCachedRead;
  bool mHasCachedWrite;
  uint64_t mCachedWrite;

  bool listenSockets();

  void *thread(void);

  static void* thread(void* ctx);
};
