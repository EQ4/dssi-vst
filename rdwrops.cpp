// -*- c-basic-offset: 4 -*-

/*
  dssi-vst: a DSSI plugin wrapper for VST effects and instruments
  Copyright 2004 Chris Cannam
*/

#include "rdwrops.h"

extern void
rdwr_tryRead(int fd, void *buf, size_t count, const char *file, int line)
{
    ssize_t r = 0;

    while ((r = read(fd, buf, count)) < (ssize_t)count) {
	
	if (r < 0) {
	    char message[100];
	    sprintf(message, "Read failed on fd %d at %s:%d", fd, file, line);
	    perror(message);
	    throw RemotePluginClosedException();
	} else if (r == 0) {
	    // end of file
	    throw RemotePluginClosedException();
	}

	buf = (void *)(((char *)buf) + r);
	count -= r;
    }
}

extern void
rdwr_tryWrite(int fd, const void *buf, size_t count, const char *file, int line)
{
    ssize_t w = write(fd, buf, count);

    if (w < 0) {
	char message[100];
	sprintf(message, "Write failed on fd %d at %s:%d", fd, file, line);
	perror(message);
	throw RemotePluginClosedException();
    }

    if (w < (ssize_t)count) {
	fprintf(stderr, "Failed to complete write on fd %d (have %d, put %d) at %s:%d\n",
		fd, count, w, file, line);
	throw RemotePluginClosedException();
    }
}

extern void
rdwr_writeOpcode(int fd, RemotePluginOpcode opcode, const char *file, int line)
{
    rdwr_tryWrite(fd, &opcode, sizeof(RemotePluginOpcode), file, line);
}    

extern void
rdwr_writeString(int fd, const std::string &str, const char *file, int line)
{
    int len = str.length();
    rdwr_tryWrite(fd, &len, sizeof(int), file, line);
    rdwr_tryWrite(fd, str.c_str(), len, file, line);
}

extern std::string
rdwr_readString(int fd, const char *file, int line)
{
    int len;
    static char *buf = 0;
    static int bufLen = 0;
    rdwr_tryRead(fd, &len, sizeof(int), file, line);
    if (len + 1 > bufLen) {
	delete buf;
	buf = new char[len + 1];
	bufLen = len + 1;
    }
    rdwr_tryRead(fd, buf, len, file, line);
    buf[len] = '\0';
    return std::string(buf);
}

extern void
rdwr_writeInt(int fd, int i, const char *file, int line)
{
    rdwr_tryWrite(fd, &i, sizeof(int), file, line);
}

extern int
rdwr_readInt(int fd, const char *file, int line)
{
    int i = 0;
    rdwr_tryRead(fd, &i, sizeof(int), file, line);
    return i;
}

extern void
rdwr_writeFloat(int fd, float f, const char *file, int line)
{
    rdwr_tryWrite(fd, &f, sizeof(float), file, line);
}

extern float
rdwr_readFloat(int fd, const char *file, int line)
{
    float f = 0;
    rdwr_tryRead(fd, &f, sizeof(float), file, line);
    return f;
}

extern unsigned char *
rdwr_readMIDIData(int fd, int &len, const char *file, int line)
{
    static unsigned char *buf = 0;
    static int bufLen = 0;
    rdwr_tryRead(fd, &len, sizeof(int), file, line);
    if (len > bufLen) {
	delete buf;
	buf = new unsigned char[len];
	bufLen = len;
    }
    rdwr_tryRead(fd, buf, len, file, line);
    return buf;
}
