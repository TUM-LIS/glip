#include "GlipTcp.h"

#include <iostream>

#include <arpa/inet.h>
#include <cerrno>
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <cassert>

using namespace std;

GlipTcp::GlipTcp(int port, int width)
: mPort(port), mNumBytes(width >> 3), mThreadReady(false),
  mConnected(false), mHasCachedRead(false) {
    pthread_create(&mThread, 0, &GlipTcp::thread, this);

    while(!mThreadReady) {}
}

int GlipTcp::reset() {
    return 0;
}

uint32_t GlipTcp::next_cycle() {
    int rv;
    uint32_t state = 0;

    if (!mConnected) {
        return state;
    }

    // Check for new control message
    uint16_t controlmsg;
    rv = read(mSocketControl, &controlmsg, 2);
    if (rv > 0) {
        assert(rv == 2);
        cout << "Received logic reset" << endl;
        state |= 0x1;
    }

    if (mHasCachedRead) {
        state |= 0x2;
    } else {
        rv = read(mSocketData, &mCachedRead, mNumBytes);
        if (rv > 0) {
            assert(rv == mNumBytes);
            mHasCachedRead = true;
            state |= 0x2;
        }
    }

    // Check for pending write
    if (mHasCachedWrite) {
        rv = write(mSocketData, &mCachedWrite, mNumBytes);
        if (rv > 0) {
            assert(rv == mNumBytes);
            mHasCachedWrite = false;
            state |= 0x4;
        }
    } else {
        state |= 0x4;
    }

    return state;
}

uint32_t GlipTcp::control_msg() {
    return 0;
}

uint64_t GlipTcp::readData() {
    assert(mHasCachedRead);
    return mCachedRead;
}

void GlipTcp::readAck() {
    assert(mHasCachedRead);
    mHasCachedRead = false;
}

void GlipTcp::writeData(uint64_t data) {
    assert(!mHasCachedWrite);
    mHasCachedWrite = true;
    mCachedWrite = data;
}

bool GlipTcp::connected() {
    return mConnected;
}

bool GlipTcp::listenSockets() {
    struct sockaddr_in servaddr;
    int rv;

    // Create sockets
    mListenSocketData = socket(AF_INET, SOCK_STREAM, 0);
    if (mListenSocketData < 0) {
        cerr << "Cannot create data socket" << endl;
        return false;
    }

    mListenSocketControl = socket(AF_INET, SOCK_STREAM, 0);
    if (mListenSocketControl < 0) {
        cerr << "Cannot create control socket" << endl;
        return false;
    }

    // Reuse existing sockets
    int enable = 1;
    rv = setsockopt(mListenSocketData, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    if (rv < 0) {
        cerr << "Cannot set data socket properties" << endl;
        return false;
    }
    rv = setsockopt(mListenSocketControl, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    if (rv < 0) {
        cerr << "Cannot set control socket properties" << endl;
        return false;
    }

    // bind address
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(mPort);

    rv = bind(mListenSocketData, (struct sockaddr *) &servaddr, sizeof(servaddr));
    if (rv < 0) {
        cerr << "Cannot bind data port" << endl;
        return false;
    }

    servaddr.sin_port = htons(mPort + 1);
    rv = bind(mListenSocketControl, (struct sockaddr *) &servaddr, sizeof(servaddr));
    if (rv < 0) {
        cerr << "Cannot bind control port" << endl;
        return false;
    }

    // listen for incoming connections
    rv = listen(mListenSocketData, 1);
    if (rv < 0) {
        cerr << "Cannot listen on data port" << endl;
        return false;
    }

    rv = listen(mListenSocketControl, 1);
    if (rv < 0) {
        cerr << "Cannot listen on control port" << endl;
        return false;
    }

    return true;
}

void *GlipTcp::thread(void) {
    mThreadReady = true;

    if (listenSockets()) {
        cout << "Glip TCP DPI listening on port " << mPort <<
                " and " << (mPort + 1) << endl;
    } else {
        return (void*) -1;
    }

    while(true) {
        mSocketData = -1;
        mSocketControl = -1;

        int nmax;
        if (mListenSocketData > mListenSocketControl) {
            nmax = mListenSocketData + 1;
        } else {
            nmax = mListenSocketControl + 1;
        }

        while ((mSocketData == -1) || (mSocketControl == -1)) {
            fd_set waitfds;
            FD_ZERO(&waitfds);
            FD_SET(mListenSocketData, &waitfds);
            FD_SET(mListenSocketControl, &waitfds);

            int activity = select( nmax + 1 , &waitfds , 0 , 0 , 0);

            if ((activity < 0) && (errno != EINTR)) {
                cerr << "Error while waiting for TCP connections" << endl;
                return (void*) -1;
            }

            if (FD_ISSET(mListenSocketData, &waitfds)) {
                mSocketData = accept(mListenSocketData, &mSocketAddressData, (socklen_t*)&mSocketAddressLengthData);
                if (mSocketData < 0) {
                    cerr << "Error while accepting data connection" << endl;
                    return (void*) -1;
                }

                int status = fcntl(mSocketData, F_GETFL, 0);
                status = fcntl(mSocketData, F_SETFL, status | O_NONBLOCK);
            }

            if (FD_ISSET(mListenSocketControl, &waitfds)) {
                mSocketControl = accept(mListenSocketControl, &mSocketAddressControl, (socklen_t*)&mSocketAddressLengthControl);
                if (mSocketControl < 0) {
                    cerr << "Error while accepting control connection" << endl;
                    return (void*) -1;
                }

                int status = fcntl(mSocketControl, F_GETFL, 0);
                status = fcntl(mSocketControl, F_SETFL, status | O_NONBLOCK);
            }
        }

        cout << "Client connected" << endl;
        mConnected = true;

        while(true) {}

    }
    return 0;
}

void* GlipTcp::thread(void* ctx) {
    GlipTcp *c = (GlipTcp *) ctx;
    return c->thread();
}
